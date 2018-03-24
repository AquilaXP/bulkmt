#include <iostream>
#include <string>
#include <sstream>

#include "ISubject.h"
#include "IStreamSubject.h"

#include "FileObserverMT.h"
#include "ConsoleObserver.h"
#include "ThreadObserver.h"

volatile size_t v = 0;
int main( int ac, char* av[] )
{
    std::cout << "start!\n";
    if( ac != 2 )
    {
        std::cout << "Not set size block command!\n";
        return 1;
    }
    int32_t N = std::atoi(av[1]);

    auto burden = []() ->void
    {
        for( size_t i = 0; i < 50000000; ++i )
            v++;
    };

    auto b = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> emain;
    struct guard_time
    {
        guard_time(std::chrono::time_point<std::chrono::steady_clock>& t ):m_t(t)
        {}
        ~guard_time()
        {
            m_t = std::chrono::high_resolution_clock::now();
        }
        std::chrono::time_point<std::chrono::steady_clock>& m_t;
    };
    {
        const size_t count_thread = 1;
        FileObserverMT file_observer( count_thread, count_thread );
        file_observer.SetBurden( burden );
        guard_time gt( emain );
        IStreamSubject s( N );
       // s.Attach( &thread_console_obs );
        s.Attach( &file_observer );
        std::ifstream f( "test_cmd.txt" );
        s.Run( f );
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>( emain - b ).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>( e - b ).count() << std::endl;

    return 0;
}