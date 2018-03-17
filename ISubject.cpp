#include "ISubject.h"

#include <algorithm>

void ISubjectMT::Attach( IObserverMT* obs )
{
    try
    {
        m_obs.insert( { obs, std::thread( &IObserverMT::Run, obs ) } );
    }
    catch( const std::exception& )
    {
        m_obs.erase( obs );
    }      
}

void ISubjectMT::Detach( IObserverMT* obs )
{
    auto o = m_obs.find( obs );
    if( o != m_obs.end() )
    {
        o->first->Update( "" );
        if( o->second.joinable() )
            o->second.join();
        m_obs.erase( o );
    }
}

void ISubjectMT::Notify( const std::string& cmd )
{
    for( auto& o : m_obs )
    {
        try
        {
            o.first->Update( cmd );
        }
        catch( ... )
        {
            // кому-то сообщаем
        }
    }
}

