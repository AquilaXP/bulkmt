#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <chrono>

#include "ObserverBase.h"
#include "IState.h"

/// Наблюдатель сохраняющий в файл
class FileObserver : public IObserver
{
public:
    void Update( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms ) override
    {
        std::string name_file;
        name_file += "bulk";
        name_file += std::to_string( time_first_cmd_ms );
        name_file += ".log";

        std::ofstream f( name_file, std::ios::binary );
        PrintPackCmd( f, pack_cmd );
    }
};