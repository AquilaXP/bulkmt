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
    virtual void Notify( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms );

protected:
    std::vector<IObserver*> m_obs;
};