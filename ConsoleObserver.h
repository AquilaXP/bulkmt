#pragma once

#include <iostream>
#include <string>

#include "ObserverBase.h"
#include "IState.h"
#include "Statistic.h"

/// Наблюдатель выводящий в консоль
class ConsoleObserver : public ObserverBaseMT
{
public:
    ConsoleObserver( size_t N )
        : m_ac( this, N )
    {}
    void RunPrivate() override
    {
        std::string cmd;
        while( true )
        {
            m_queue_cmd.wait_and_pop( cmd );
            m_ac.AppendCmd( cmd );
            m_Stat.Update( cmd );
        }
    }
    void EventAddCmdToBlock( const std::string& cmd, uint32_t num_cmd ) override
    {

    }
    void AppendPackCmd( const std::string& pack_cmd )
    {
        std::cout << "bulk: " << pack_cmd << std::endl;
    }
private:


    Statistic m_Stat;
    AppenderCmd m_ac;
};