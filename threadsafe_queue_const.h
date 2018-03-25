#pragma once

#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>

/// Потоко-безопасная очередь с фиксированным размером
/// #TODO: заменить на boost::circular_buffer
template <class T>
class threadsafe_queue_const
{
private:
    typedef std::lock_guard<std::mutex> lock_m;
    typedef std::unique_lock<std::mutex> ulock_m;

    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    std::condition_variable data_cond_push;
    std::size_t max_size = 0;
public:
    threadsafe_queue_const( size_t max_size ) : max_size(max_size)
    {
    };
    threadsafe_queue_const( threadsafe_queue_const const& other )
    {
        lock_m lk( other.mut );
        data_queue = other.data_queue;
    }
    void set_max_size( size_t size )
    {
        {
            lock_m lk( mut );
            max_size = size;
        }
        data_cond_push.notify_one();
    }
    void wait_and_push( T new_value )
    {
        {
            ulock_m lk( mut );
            data_cond_push.wait( lk, [this]{
                return data_queue.size() < max_size;
            } );
            data_queue.push( std::move( new_value ) );
        }
        data_cond.notify_all();
    }

    bool try_push( T new_value )
    {
        lock_m lk( mut );
        {
            if( data_queue.size() == max_size )
                return false;
            data_queue.push( std::move( new_value ) );
        }
        data_cond.notify_all();
        return true;
    }

    void wait_and_pop( T& value )
    {
        {
            ulock_m lk( mut );
            data_cond.wait( lk, [this]{
                return !data_queue.empty();
            } );
            value = data_queue.front();
            data_queue.pop();
        }
        data_cond_push.notify_one();
    }

    std::unique_ptr<T> wait_and_pop()
    {
        std::unique_ptr<T> res;
        {
            ulock_m lk( mut );
            data_cond.wait( lk, [this]{
                return !data_queue.empty();
            } );
            res = std::unique_ptr<T>( new T( data_queue.front() ) );
            data_queue.pop();
        }
        data_cond_push.notify_one();
        return res;
    }

    bool try_pop( T& value )
    {
        {
            lock_m lk( mut );
            if( data_queue.empty() )
            {
                return false;
            }
            value = data_queue.front();
            data_queue.pop();
        }
        data_cond_push.notify_one();
        return true;
    }

    std::unique_ptr<T> try_pop()
    {
        std::unique_ptr<T> res;
        {
            lock_m lk( mut );
            if( data_queue.empty() )
            {
                return std::unique_ptr<T>();
            }

            res = std::unique_ptr<T>( new T( data_queue.front() ) );
            data_queue.pop();
        }
        data_cond_push.notify_one();
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
