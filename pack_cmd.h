#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ostream>

using cmd_t = std::string;
using pack_cmd_t = std::vector< cmd_t >;

inline void PrintPackCmd( std::ostream& os, const pack_cmd_t& pack_cmd )
{
    os << "bulk: ";
    for( size_t i = 0; i < pack_cmd.size(); ++i )
    {
        os << pack_cmd[i];
        if( i + 1 != pack_cmd.size() )
            os << ", ";
    }
    os << '\n';
}
