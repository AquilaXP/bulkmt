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

    {
        ConsoleObserver co;
        ThreadObserver to( &co );
        to.Start();
        FileObserverMT fo( 2 );
        std::unique_ptr<IStreamSubject> s( new IStreamSubject( N ) );
        s->Attach( &to );
        s->Attach( &fo );
        std::ifstream f( "test_cmd1.txt" );
        s->Run( f );
    }
    return 0;
}