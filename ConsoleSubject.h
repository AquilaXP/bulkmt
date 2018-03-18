#pragma once

#include <string>
#include <iostream>

#include "ISubject.h"
#include "Statistic.h"

/// Издатель
class ConsoleSubject : public ISubject
{
public:
    void Run()
    {
        std::string cmd;
        while( std::getline( std::cin, cmd ) )
        {
            Notify( cmd );
            m_stat.Update( cmd );
        }
        PrintStat();
    }
private:
    void PrintStat()
    {
        std::cout << "main: Line:"
            << m_stat.GetCountLine() << " "
            << m_stat.GetCountCmd() << " "
            << m_stat.GetCountBlock() << std::endl;
    }

    Statistic m_stat;
};

