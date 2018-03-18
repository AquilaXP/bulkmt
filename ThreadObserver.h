#pragma once

#include <cassert>
#include <thread>
#include <atomic>

#include "IObserver.h"
#include "threadsafe_queue.h"

/// Класс для запуска слушатяле в отдельном потоке
class ThreadObserver : public IObserver
{
public:
    ThreadObserver( IObserver* obs ) : m_obs(obs)
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

        m_stop = false;
        m_thread = std::thread( &ThreadObserver::Run, this );
    }
    IObserver* GetObserver() const
    {
        return m_obs;
    }
    void Stop()
    {
        m_stop = true;
        m_queue_cmd.push( { pack_cmd_t(), 0 } );
    }
    void Wait()
    {
        if( m_thread.joinable() )
            m_thread.join();
    }
    void Update( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms ) override
    {
        m_queue_cmd.push( std::make_pair( pack_cmd, time_first_cmd_ms ) );
    }
private:
    void Run()
    {
        try
        {
            assert( m_obs );
            while( true )
            {
                auto cmd = m_queue_cmd.wait_and_pop();
                if( m_stop == true )
                    break;
                m_obs->Update( cmd->first, cmd->second );
            }
        }
        catch( ... )
        {
        }
    }

    IObserver* m_obs = nullptr;
    std::atomic_bool m_stop = { false };
    std::thread m_thread;
    threadsafe_queue<std::pair<pack_cmd_t, uint64_t>> m_queue_cmd;
};