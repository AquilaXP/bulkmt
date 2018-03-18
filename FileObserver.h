#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <chrono>

#include "ObserverBase.h"
#include "IState.h"

/// Наблюдатель сохраняющий в файл
class FileObserver : public ObserverBase
{
public:
    FileObserver( size_t N )
        : m_ac( this, N )
    {}
    void Update( const std::string& cmd ) override
    {
        m_ac.AppendCmd( cmd );
    }
    void EventAddCmdToBlock( const std::string& cmd, uint32_t num_cmd ) override
    {
        if( num_cmd == 0 )
        {
            namespace sc = std::chrono;
            auto t = sc::system_clock::now();
            std::stringstream ss;
            ss <<
                "bulk" <<
                sc::duration_cast<sc::seconds>( t.time_since_epoch() ).count() <<
                ".log";
            m_name_file = ss.str();
        }
    }
    void AppendPackCmd( const std::string& pack_cmd, uint32_t count_cmd ) override
    {
        std::ofstream f( m_name_file, std::ios::binary );
        f << "bulk: " << pack_cmd << std::endl;
        m_name_file.clear();
    }
private:
    std::string m_name_file;
    AppenderCmd m_ac;
};