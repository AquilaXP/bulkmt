#pragma once

#include <cstdint>
#include <string>

class Statistic
{
public:
    void AddLine( uint32_t count = 1 )
    {
        m_count_line += count;
    }
    void AddCmd( uint32_t count = 1 )
    {
        m_count_cmd += count;
    }
    void AddBlock( uint32_t count = 1 )
    {
        m_count_block += count;
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

    void Print( std::ostream& os ) const
    {
        os <<
            "Count line = " << GetCountLine() << '\n' <<
            "Count cmd = " << GetCountCmd() << '\n' <<
            "Count block = " << GetCountBlock() << std::endl;
    }
private:
    uint32_t m_count_block = 0;
    uint32_t m_count_cmd = 0;
    uint32_t m_count_line = 0;
};