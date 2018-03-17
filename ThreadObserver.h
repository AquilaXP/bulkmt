#pragma once

#include <cassert>
#include <thread>
#include <atomic>

#include "IObserver.h"
#include "threadsafe_queue.h"

class ThreadObserver : public IObserverMT
{
public:
    ThreadObserver( IObserverMT* obs ) : m_obs(obs)
    {}
    void Start()
    {
        if( m_thread.joinable() )
            throw std::runtime_error( "thread runned!" );

        m_stop = false;
        m_thread = std::thread( &ThreadObserver::Run, this );
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
    void Update( const std::string& cmd )
    {
        m_queue_cmd.push( cmd );
    }
private:
    void Run()
    {
        assert( m_obs );
        auto cmd = m_queue_cmd.wait_and_pop();
        if( m_stop == true )
            return;
        m_obs->Update( *cmd );
    }

    IObserverMT* m_obs = nullptr;
    std::atomic_bool m_stop = false;
    std::thread m_thread;
    threadsafe_queue<std::string> m_queue_cmd;
};