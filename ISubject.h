#pragma once

#include <vector>
#include <string>

#include "IObserver.h"

class ISubject
{
public:
    virtual ~ISubject() = default;

    virtual void Attach( IObserver* obs );
    virtual void Detach( IObserver* obs );
    virtual void Notify( const std::string& cmd );

protected:
    std::vector<IObserver*> m_obs;
};