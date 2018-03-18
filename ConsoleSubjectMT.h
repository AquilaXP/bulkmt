#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <memory>

#include "ThreadObserver.h"
#include "Statistic.h"
#include "ISubject.h"

/// Многопоточный издатель
class ConsoleSubjectMT : public ISubject
{
public:
    void Attach( IObserver* obs ) override
    {
        std::unique_ptr<ThreadObserver> to( new ThreadObserver( obs ) );
        to->Start();
        m_obs.push_back( to.get() );
        to.release();
    }
    void Detach( IObserver* obs )
    {
        m_obs.erase( std::find_if( m_obs.begin(), m_obs.end(), [&obs]( const IObserver* o )
        {
            return ( o == static_cast<const ThreadObserver*>( o )->GetObserver() );
        } ) );
    }
    void Run()
    {
        std::string cmd;
        while( std::getline( std::cin, cmd ) )
        {
            Notify( cmd );
            m_stat.Update( cmd );
        }
    }
    ~ConsoleSubjectMT()
    {
        PrintStat();
        for( auto& o : m_obs )
        {
            delete o;
        }
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