#pragma once

#include <cstdint>
#include <string>

class Statistic
{
public:
    void Init( uint32_t min_count )
    {
        m_min_count = min_count;
    }
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
        if( m_lvl_block == 0 && m_current_count_cmd > 0 )
        {
            ++m_count_block;
            m_current_count_cmd = 0;
        }
        ++m_lvl_block;
    }
    void CloseBlock()
    {
        --m_lvl_block;
        if( m_lvl_block == 0 )
        {
            ++m_count_block;
            m_current_count_cmd = 0;
        }
    }
    void Cmd()
    {
        ++m_count_cmd;
        if( m_lvl_block == 0 )
        {
            ++m_current_count_cmd;
            if( m_current_count_cmd >= m_min_count )
            {
                m_current_count_cmd = 0;
                ++m_count_block;
            }
        }
    }
    uint32_t m_current_count_cmd = 0;
    uint32_t m_min_count = 0;
    uint32_t m_lvl_block = 0;
    uint32_t m_count_block = 0;
    uint32_t m_count_cmd = 0;
    uint32_t m_count_line = 0;
};