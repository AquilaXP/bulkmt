#include <fstream>
#include <iomanip>

int main()
{
    std::ofstream ostr( "test_cmd.txt" );

    for( size_t i = 0; i < 10000000; ++i )
    {
        ostr << std::setw(100) << std::setfill('0') << i << '\n';
    }
    return 0;
}