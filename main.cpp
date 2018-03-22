#include <iostream>
#include <string>
#include <sstream>

#include "ISubject.h"
#include "IStreamSubject.h"

#include "FileObserverMT.h"
#include "ConsoleObserver.h"
#include "ThreadObserver.h"


int main( int ac, char* av[] )
{
    if( ac != 2 )
    {
        std::cout << "Not set size block command!\n";
        return 1;
    }
    int32_t N = std::atoi(av[1]);

    ConsoleObserver console_obs;
    ThreadObserver thread_console_obs( &console_obs, 1 );
    // Запускаем поток коносли
    thread_console_obs.Start();

    FileObserverMT file_observer( 4, 4 );

    IStreamSubject s( N );
    s.Attach( &thread_console_obs );
    s.Attach( &file_observer );
    s.Run( std::cin );

    return 0;
}