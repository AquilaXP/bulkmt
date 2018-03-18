#include "ISubject.h"

#include <algorithm>

void ISubject::Attach( IObserver* obs )
{
    m_obs.push_back( obs );
}

void ISubject::Detach( IObserver* obs )
{
    m_obs.erase( std::find( m_obs.begin(), m_obs.end(), obs ) );
}

void ISubject::Notify( const std::string& cmd )
{
    for( auto& o : m_obs )
    {
        try
        {
            o->Update( cmd );
        }
        catch( ... )
        {
            // кому-то сообщаем
        }
    }
}

