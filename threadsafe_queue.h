#pragma once

#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>

template <class T>
class threadsafe_queue
{
private:
    typedef std::lock_guard<std::mutex> lock_m;
    typedef std::unique_lock<std::mutex> ulock_m;

    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue() = default;
    threadsafe_queue( threadsafe_queue const& other )
    {
        lock_m lk( other.mut );
        data_queue = other.data_queue;
    }

    void push( T new_value )
    {
        lock_m lk( mut );
        data_queue.push( std::move(new_value) );
        data_cond.notify_all();
    }

    void wait_and_pop( T& value )
    {
        ulock_m lk( mut );
        data_cond.wait( lk, [this]{
            std::cout << std::this_thread::get_id() << std::endl;
            return !data_queue.empty();
        } );
        value = data_queue.front();
        data_queue.pop();
    }

    std::unique_ptr<T> wait_and_pop()
    {
        ulock_m lk( mut );
        data_cond.wait( lk, [this]{
            return !data_queue.empty();
        } );
        std::unique_ptr<T> res( std::make_unique<T>( data_queue.front() ) );
        data_queue.pop();
        return res;
    }

    bool try_pop( T& value )
    {
        lock_m lk( mut );
        if( data_queue.empty() )
        {
            return false;
        }

        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::unique_ptr<T> try_pop()
    {
        lock_m lk( mut );
        if( data_queue.empty() )
        {
            return std::unique_ptr<T>();
        }

        std::unique_ptr<T> res( std::make_unique<T>( data_queue.front() ) );
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        lock_m lk( mut );
        return data_queue.empty();
    }
    void clear()
    {
        lock_m lk( mut );
        data_queue.swap( std::queue<T>() );
    }
};
