#pragma once

#include "pack_cmd.h"

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void Update( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms ) = 0;
};
