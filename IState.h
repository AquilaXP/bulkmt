#pragma once

#include <ciso646>

#include <string>
#include <vector>
#include <memory>

#include "ISubject.h"
#include "Statistic.h"

class AppenderCmd;

/// Базовый класс состояния
class IState
{
public:
    IState( AppenderCmd* context );
    virtual ~IState() = default;

    virtual void AppendCmd( const cmd_t& cmd ) = 0;
    virtual void Flush() = 0;

protected:
    void UpdateCountCmd( uint32_t add_count = 1 );
    void ChangeState( IState* next_state );
    void SetTime();
    uint64_t GetTime() const;

    AppenderCmd* m_context = nullptr;
    pack_cmd_t m_pack_cmd;
    uint64_t m_time_first_cmd_ms = 0;
};

/// Класс для добавления комманд.
/// Может быть в двух состояниях:
/// 1 - накапливать N команд, после отсылать.
/// 2 - накапливать блок команд между символами {}.
class AppenderCmd
{
    friend class IState;
public:
    enum State_T
    {
        STATE_WAIT_N_CMD = 0,
        STATE_WAIT_END_BLOCK = 1
    };

    AppenderCmd( ISubject* subject, size_t N );
    AppenderCmd( const AppenderCmd& ) = delete;
    AppenderCmd& operator = ( const AppenderCmd& ) = delete;

    void AppendCmd( const cmd_t& cmd );
    void Flush();
    const Statistic& GetStat() const;

    void NotifyPackCmd( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms );
    void ChangeState( IState* next_state );
    IState* GetState( State_T id_state );

private:
    std::vector< std::unique_ptr<IState> > m_states;
    IState* m_state = nullptr;
    ISubject* m_subject = nullptr;
    Statistic m_stat;
};

/// Состояние с блоками {} команд
class StateWaitEndBlock : public IState
{
public:
    StateWaitEndBlock( AppenderCmd* context );
    void AppendCmd( const cmd_t& cmd ) override;
    void Flush() override {}
private:
    void Uplvl();
    void Downlvl();
    void CopyCmd( const std::string& cmd );

    int32_t m_lvl = 1;
};

/// Состояние с ожиданием N комманд
class StateWaitNCmd : public IState
{
public:
    StateWaitNCmd( AppenderCmd* context, size_t N );
    ~StateWaitNCmd();
    void AppendCmd( const cmd_t& cmd ) override;
    void Flush() override;
private:
    size_t m_N = 0;
};