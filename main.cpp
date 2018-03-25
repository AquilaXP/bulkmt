#include <iostream>
#include <string>
#include <sstream>

#include "ISubject.h"
#include "IStreamSubject.h"

#include "FileObserverMT.h"
#include "ConsoleObserver.h"
#include "ThreadObserver.h"

using namespace std::chrono;

struct guard_time
{
    guard_time( time_point<high_resolution_clock>& t )
        : m_t( t )
    {}
    ~guard_time()
    {
        m_t = time_point_cast<milliseconds>( high_resolution_clock::now() );
    }
private:
    time_point<high_resolution_clock>& m_t;
};

volatile size_t v = 0;

int main( int ac, char* av[] )
{
    std::cout << "start!\n";
    if( ac < 3 )
    {
        std::cout << "Not set size block command!\n";
        return 1;
    }
    int32_t N = std::atoi( av[1] );
    int32_t T = std::atoi( av[2] );

    auto burden = []() ->void
    {
        for( size_t i = 0; i < 50000000; ++i )
            v++;
    };

    auto b = high_resolution_clock::now();
    time_point<high_resolution_clock> emain;
    {
        const size_t count_thread = T;
        FileObserverMT file_observer( count_thread, count_thread );
        file_observer.SetBurden( burden );
        guard_time gt( emain );
        IStreamSubject s( N );
        s.Attach( &file_observer );
        std::ifstream f( "test_cmd.txt" );
        s.Run( f );
    }
    auto e = high_resolution_clock::now();
    std::cout << duration_cast<milliseconds>( emain - b ).count() << std::endl;
    std::cout << duration_cast<milliseconds>( e - b ).count() << std::endl;

    return 0;
}