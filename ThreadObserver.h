#pragma once

#include <cassert>
#include <thread>
#include <atomic>

#include "IObserver.h"
#include "threadsafe_queue.h"
#include "threadsafe_queue_const.h"

/// Wrapper для запуска слушатяле в отдельном потоке
class ThreadObserver : public IObserver
{
public:
    ThreadObserver( IObserver* obs, size_t depth_queue = (std::numeric_limits<size_t>::max)() )
        : m_obs( obs ), m_queue_cmd( depth_queue )
    {}
    ~ThreadObserver()
    {
        Stop();
        Wait();
    }
    void Start()
    {
        if( m_thread.joinable() )
            throw std::runtime_error( "thread runned!" );

        m_thread = std::thread( &ThreadObserver::Run, this );
    }
    IObserver* GetObserver() const
    {
        return m_obs;
    }
    void Stop()
    {
        m_queue_cmd.wait_and_push( { pack_cmd_t(), 0 } );
    }
    void Wait()
    {
        if( m_thread.joinable() )
            m_thread.join();
    }
    void Update( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms ) override
    {
        m_queue_cmd.wait_and_push( std::make_pair( pack_cmd, time_first_cmd_ms ) );
    }
private:
    void Run()
    {
        try
        {
            assert( m_obs );
            while( true )
            {
                std::pair<pack_cmd_t, uint64_t> cmd;
                m_queue_cmd.wait_and_pop( cmd );
                if( cmd.first.empty() )
                    break;
                m_obs->Update( cmd.first, cmd.second );
            }
        }
        catch( ... )
        {
        }
    }

    IObserver* m_obs = nullptr;
    std::thread m_thread;
    threadsafe_queue_const<std::pair<pack_cmd_t, uint64_t>> m_queue_cmd;
};