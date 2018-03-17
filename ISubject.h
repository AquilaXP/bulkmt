#pragma once

#include <map>
#include <string>
#include <thread>

#include "IObserver.h"

class ISubjectMT
{
public:
    virtual ~ISubjectMT() = default;

    void Attach( IObserverMT* obs );
    void Detach( IObserverMT* obs );
    virtual void Notify( const std::string& cmd );

protected:
    std::map<IObserverMT*, std::thread> m_obs;
};