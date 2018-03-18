#include "IState.h"

#include <chrono>
#include <memory>
#include <cassert>

// ===========================================================
//                  IState
// ===========================================================
IState::IState( AppenderCmd* context )
    : m_context(context)
{
}

inline void IState::UpdateCountCmd( uint32_t add_count )
{
    m_context->m_stat.AddCmd( add_count );
}

void IState::ChangeState( IState* next_state )
{
    m_context->ChangeState( next_state );
}

void IState::SetTime()
{
    using namespace std::chrono;

    m_time_first_cmd_ms =
        duration_cast<milliseconds>( system_clock::now().time_since_epoch() ).count();
}

uint64_t IState::GetTime() const
{
    return m_time_first_cmd_ms;
}
// ===========================================================
//                  AppenderCmd
// ===========================================================
AppenderCmd::AppenderCmd( ISubject* subject, size_t N )
    : m_subject(subject)
{
    m_states.emplace_back( std::unique_ptr<IState>( new StateWaitNCmd( this, N ) ) );
    m_states.emplace_back( std::unique_ptr<IState>( new StateWaitEndBlock( this ) ) );

    m_state = m_states[size_t( STATE_WAIT_N_CMD )].get();
}

void AppenderCmd::AppendCmd( const cmd_t& cmd )
{
    m_stat.AddLine();
    m_state->AppendCmd( cmd );
}

void AppenderCmd::NotifyPackCmd( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms )
{
    m_stat.AddBlock();
    m_subject->Notify( pack_cmd, time_first_cmd_ms );
}

void AppenderCmd::Flush()
{
    m_state->Flush();
}

void AppenderCmd::ChangeState( IState* next_state )
{
    m_state = next_state;
}

IState* AppenderCmd::GetState( State_T id_state )
{
    return m_states[size_t( id_state )].get();
}

const Statistic& AppenderCmd::GetStat() const
{
    return m_stat;
}

// ===========================================================
//                  StateWaitEndBlock
// ===========================================================
StateWaitEndBlock::StateWaitEndBlock( AppenderCmd* context )
    : IState(context)
{
}

void StateWaitEndBlock::AppendCmd( const cmd_t& cmd )
{
    if( cmd == "{" )
    {
        Uplvl();
    }
    else if( cmd == "}" )
    {
        Downlvl();
    }
    else
    {
        CopyCmd( cmd );
    }
}

void StateWaitEndBlock::Uplvl()
{
    ++m_lvl;
}

void StateWaitEndBlock::Downlvl()
{
    --m_lvl;
    if( m_lvl == 0 )
    {
        if( not m_pack_cmd.empty() )
        {
            m_context->NotifyPackCmd( m_pack_cmd, GetTime() );
            m_pack_cmd.clear();
        }
        ChangeState( m_context->GetState(AppenderCmd::STATE_WAIT_N_CMD) );
        m_lvl = 1; // при следующем вызове
    }
}

void StateWaitEndBlock::CopyCmd( const std::string& cmd )
{
    // Пришла первая комманда, запоминаем время
    if( m_pack_cmd.empty() )
        SetTime();
    m_pack_cmd.push_back( cmd );
    UpdateCountCmd();
}

// ===========================================================
//                  StateWaitNCmd
// ===========================================================
StateWaitNCmd::StateWaitNCmd( AppenderCmd* context, size_t N )
    : IState(context), m_N(N)
{
}

StateWaitNCmd::~StateWaitNCmd()
{
    Flush();
}

void StateWaitNCmd::AppendCmd( const cmd_t& cmd )
{
    if( cmd == "{" )
    {
        Flush();
        ChangeState( m_context->GetState(AppenderCmd::STATE_WAIT_END_BLOCK) );
    }
    else
    {
        if( m_pack_cmd.empty() )
            SetTime();

        m_pack_cmd.push_back( cmd );
        UpdateCountCmd();
        if( m_pack_cmd.size() >= m_N )
        {
            Flush();
        }
    }
}

void StateWaitNCmd::Flush()
{
    if( m_pack_cmd.empty() )
        return;

    m_context->NotifyPackCmd( m_pack_cmd, GetTime() );
    m_pack_cmd.clear();
}
