#include <fstream>

int main()
{
    std::ofstream ostr( "test_cmd.txt" );

    for( size_t i = 0; i < 10000000; ++i )
    {
        ostr << i << '\n';
    }
    return 0;
}