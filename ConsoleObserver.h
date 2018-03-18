#pragma once

#include <iostream>
#include <string>

#include "ObserverBase.h"
#include "IState.h"
#include "Statistic.h"

/// Наблюдатель выводящий в консоль
class ConsoleObserver : public ObserverBase
{
public:
    ConsoleObserver( size_t N )
        : m_ac( this, N )
    {}
    ~ConsoleObserver()
    {
        m_ac.Flush();
        try{
            std::stringstream ss;
            ss << "Console:\n" <<
                "Count cmd = " << m_count_cmd << '\n' <<
                "Count block = " << m_count_block << '\n';
            std::cout << ss.str();
        }
        catch( ... ){}
    }
    void Update(const std::string& cmd ) override
    {
        m_ac.AppendCmd( cmd );
    }
    void EventAddCmdToBlock( const std::string& cmd, uint32_t num_cmd ) override
    {

    }
    void AppendPackCmd( const std::string& pack_cmd, uint32_t count_cmd ) override
    {
        std::cout << "bulk: " << pack_cmd << std::endl;
        m_count_block += 1;
        m_count_cmd += count_cmd;
    }
private:
    uint32_t m_count_block = 0;
    uint32_t m_count_cmd = 0;
    AppenderCmd m_ac;
};