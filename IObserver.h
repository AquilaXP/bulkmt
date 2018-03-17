#pragma once

#include <string>
#include <vector>

#include "threadsafe_queue.h"

class IObserverMT
{
public:
    virtual ~IObserverMT() = default;
    void Update( const std::string& cmd )
    {
        m_queue_cmd.push( cmd );
    }
    void Stop()
    {

    }
    void Run()
    {
        RunPrivate();
    }
protected:
    virtual void RunPrivate() = 0;
    threadsafe_queue<std::string> m_queue_cmd;
};
