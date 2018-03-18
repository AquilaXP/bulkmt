#pragma once

#include <string>
#include <istream>
#include <iostream>
#include <chrono>
#include <thread>

#include "ISubject.h"
#include "Statistic.h"

/// Издатель
class IStreamSubject : public ISubject
{
public:
    void Init( uint32_t count_cmd_by_block )
    {
        m_stat.Init( count_cmd_by_block );
    }
    void Run( std::istream& istr )
    {
        std::string cmd;
        while( std::getline( istr, cmd ) )
        {
            Notify( cmd );
            m_stat.Update( cmd );
        }
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        PrintStat();
    }
private:
    void PrintStat()
    {
        std::stringstream ss;
        ss << "main: \n"
            "Count line = " << m_stat.GetCountLine() << '\n' <<
            "Count cmd = " << m_stat.GetCountCmd() << '\n' <<
            "Count block = " << m_stat.GetCountBlock() << std::endl;
        std::cout << ss.str();
    }

    Statistic m_stat;
};