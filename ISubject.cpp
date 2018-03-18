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

void ISubject::Notify( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms )
{
    for( auto& o : m_obs )
    {
        try
        {
            o->Update( pack_cmd, time_first_cmd_ms );
        }
        catch( ... )
        {
            // кому-то сообщаем
        }
    }
}

