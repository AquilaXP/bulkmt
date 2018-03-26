#pragma once

#include <string>
#include <istream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

#include "ISubject.h"
#include "IState.h"

/// Издатель
class IStreamSubject : public ISubject
{
public:
    IStreamSubject( size_t N )
        : m_ac(this, N)
    {
    }
    void Run( std::istream& istr )
    {
        std::string cmd;
        while( std::getline( istr, cmd ) )
        {
            m_ac.AppendCmd( cmd );
        }
        m_ac.Flush();
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        PrintStat();
    }
private:
    void PrintStat()
    {
        std::stringstream ss;
        ss << "main: \n";
        m_ac.GetStat().Print(ss);

        std::cout << ss.str();
    }

    AppenderCmd m_ac;
};