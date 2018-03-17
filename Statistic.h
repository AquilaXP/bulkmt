#pragma once

#include <cstdint>
#include <string>

class Statistic
{
public:    
    void Update( const std::string& cmd )
    {
        ++m_count_line;
        if( cmd == "{" )
            OpenBlock();
        else if( cmd == "}" )
            CloseBlock();
        else
            Cmd();
    }
    uint32_t GetCountCmd() const
    {
        return m_count_cmd;
    }
    uint32_t GetCountBlock() const
    {
        return m_count_block;
    }
    uint32_t GetCountLine() const
    {
        return m_count_line;
    }
private:
    void OpenBlock()
    {
        ++m_lvl_block;
    }
    void CloseBlock()
    {
        --m_lvl_block;
        if( m_lvl_block == 0 )
            ++m_count_block;
    }
    void Cmd()
    {
        ++m_count_cmd;
    }

    uint32_t m_lvl_block = 0;
    uint32_t m_count_block = 0;
    uint32_t m_count_cmd = 0;
    uint32_t m_count_line = 0;
};