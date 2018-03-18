#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <chrono>
#include <atomic>
#include <iostream>

#include "Statistic.h"
#include "threadsafe_queue.h"
#include "ObserverBase.h"
#include "IState.h"

/// Наблюдатель сохраняющий в файл
class FileObserverMT : public ObserverBase
{
public:
    FileObserverMT( size_t N, size_t count_thread )
        : m_ac( this, N )
    {
        std::generate_n( std::back_inserter( m_assistans ),
            count_thread, [this]{return std::thread( &FileObserverMT::RunT, this ); } );
    }
    ~FileObserverMT()
    {
        m_ac.Flush();
        while( not m_pack_cmd.empty() );
        m_stop = true;
        for( auto& a : m_assistans )
        {
            m_pack_cmd.push( std::make_tuple( 0, "", "" ) );
        }
        for( auto& a : m_assistans )
        {
            if( a.joinable() )
                a.join();
        }
    }
    void Update( const std::string& cmd ) override
    {
        m_ac.AppendCmd( cmd );
    }
    void EventAddCmdToBlock( const std::string& cmd, uint32_t num_cmd ) override
    {
        if( num_cmd == 0 )
        {
            namespace sc = std::chrono;
            auto t = sc::system_clock::now();
            m_time_first_cmd =
                std::to_string( sc::duration_cast<sc::seconds>( t.time_since_epoch() ).count());
        }
    }
    void AppendPackCmd( const std::string& pack_cmd, uint32_t count_cmd ) override
    {
        m_pack_cmd.push( std::make_tuple( count_cmd, m_time_first_cmd, pack_cmd ) );
    }

private:
    void RunT()
    {
        try
        {
            uint32_t count_block = 0;
            uint32_t count_cmd = 0;
            std::string name_file;
            std::string thread_id;
            {
                std::stringstream ss;
                ss << std::this_thread::get_id();
                thread_id = ss.str();
            }
            while( true )
            {
                auto p = m_pack_cmd.wait_and_pop();
                if( m_stop )
                    break;

                // Обновляем статистику потока
                count_block += 1;
                count_cmd += std::get<0>( *p );

                // формируем полное имя файла
                name_file.clear();
                name_file += "bulk";
                name_file += ( std::get<1>( *p ) );
                name_file += "_";
                name_file += thread_id;
                name_file += ".log";

                std::ofstream f( name_file, std::ios::binary );
                // записываем пак комманд
                f << "bulk: " << std::get<2>( *p ) << std::endl;
            }

            /// Выводим статистику потока
            std::stringstream ss;
            ss << "Thread file "<< std::this_thread::get_id() <<  ":\n" <<
                "Count cmd = " << count_cmd << '\n' <<
                "Count block = " << count_block << '\n';
            std::cout << ss.str();
        }
        catch( ... )
        {

        }
    }

    std::atomic_bool m_stop = false;
    std::vector<std::thread> m_assistans;
    // храним в очереди: число комманд в паке, время первой комманыд,  пак комманд
    threadsafe_queue< std::tuple<uint32_t,std::string,std::string> > m_pack_cmd;
    std::string m_time_first_cmd;
    AppenderCmd m_ac;
};