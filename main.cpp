#include <iostream>
#include <string>
#include <sstream>

#include "ISubject.h"

#include "FileObserver.h"
#include "ConsoleObserver.h"

/// Издатель
class ConsoleSubject : public ISubjectMT
{
public:
    void Run()
    {      
        std::string cmd;
        while( std::getline( std::cin, cmd ) )
        {
            if( cmd.empty() )
                return;
            Notify( cmd );
        }
    }
};


int main( int ac, char* av[] )
{
    if( ac != 2 )
    {
        std::cout << "Not set size block command!\n";
        return 1;
    }
    int32_t N = std::atoi(av[1]);

    ConsoleObserver co( N );
    FileObserver fo( N );
    ConsoleSubject cs;
    
    cs.Attach( &co );
    cs.Attach( &fo );
    cs.Run();

    return 0;
}