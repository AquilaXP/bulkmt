#pragma once

#include <iostream>
#include <string>

#include "IObserver.h"

/// Наблюдатель выводящий в консоль
class ConsoleObserver : public IObserver
{
public:
    ConsoleObserver() = default;
    ~ConsoleObserver()
    {
        try{
            PrintStat();
        }
        catch( ... ){}
    }
    void Update(const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms ) override
    {
        PrintPackCmd( std::cout, pack_cmd );
        m_count_block += 1;
        m_count_cmd += pack_cmd.size();
    }
private:
    void PrintStat()
    {
        std::stringstream ss;
        ss << "Console:\n" <<
            "Count block = " << m_count_block << '\n' <<
            "Count cmd = " << m_count_cmd << '\n';
        std::cout << ss.str();
    }

    uint32_t m_count_block = 0;
    uint32_t m_count_cmd = 0;
};