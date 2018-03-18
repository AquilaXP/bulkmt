#pragma once

#include <cassert>
#include <thread>
#include <atomic>

#include "IObserver.h"
#include "threadsafe_queue.h"

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
        m_queue_cmd.push( "" );
    }
    void Wait()
    {
        if( m_thread.joinable() )
            m_thread.join();
    }
    void Update( const std::string& cmd ) override
    {
        m_queue_cmd.push( cmd );
    }
private:
    void Run()
    {
        assert( m_obs );
        while( true )
        {
            auto cmd = m_queue_cmd.wait_and_pop();
            if( m_stop == true )
                break;
            m_obs->Update( *cmd );
        }
    }

    IObserver* m_obs = nullptr;
    std::atomic_bool m_stop = false;
    std::thread m_thread;
    threadsafe_queue<std::string> m_queue_cmd;
};