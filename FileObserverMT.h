#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <chrono>
#include <atomic>
#include <iostream>
#include <functional>

#include "Statistic.h"
#include "threadsafe_queue.h"
#include "threadsafe_queue_const.h"
#include "IState.h"

/// Наблюдатель сохраняющий в файл
class FileObserverMT : public IObserver
{
public:
    FileObserverMT( size_t count_thread, size_t depth_queue = (std::numeric_limits<size_t>::max)() )
        : m_pack_cmd( depth_queue )
    {
        std::generate_n( std::back_inserter( m_assistans ),
            count_thread, [this]{return std::thread( &FileObserverMT::RunT, this ); } );
    }
    ~FileObserverMT()
    {
        if( m_assistans.empty() )
            return;

        // Ждем пока все потоки не опустошат задания по записи
        while( not m_pack_cmd.empty() )
            std::this_thread::sleep_for( std::chrono::seconds( 0 ) );

        // начинаем остановку всех потоков,
        // каждый поток возьмет по паку команд и остановится
        for( auto& a : m_assistans )
        {
            m_pack_cmd.wait_and_push( std::make_pair( pack_cmd_t(), 0 ) );
        }
        // ждем завершения всех потоков
        for( auto& a : m_assistans )
        {
            if( a.joinable() )
                a.join();
        }
    }
    void Update( const pack_cmd_t& pack_cmd, uint64_t time_first_cmd_ms ) override
    {
        m_pack_cmd.wait_and_push( std::make_pair( pack_cmd, time_first_cmd_ms ) );
    }
    void SetBurden( std::function<void()> b )
    {
        m_burden = b;
    }
private:
    void RunT()
    {
        try
        {
            // Статистика потока
            uint32_t count_block = 0;
            uint32_t count_cmd = 0;

            // Для формирования имени файла
            std::string name_file;
            std::string thread_id;
            uint32_t unique_number = 0;
            {
                std::stringstream ss;
                ss << std::this_thread::get_id();
                thread_id = ss.str();
            }
            while( true )
            {
                std::pair < pack_cmd_t, uint64_t > p;
                m_pack_cmd.wait_and_pop( p );
                if( std::get<0>(p).empty() )
                    break;

                if( m_burden )
                {
                    m_burden();
                }
                // Обновляем статистику потока
                count_block += 1;
                count_cmd += std::get<0>( p ).size();

                // формируем полное имя файла
                name_file.clear();
                name_file += "bulk";
                name_file += std::to_string( std::get<1>( p ) );
                name_file += "_";
                name_file += thread_id;
                name_file += "_" + std::to_string( unique_number );
                name_file += ".log";
                ++unique_number;

                // записываем пак комманд
                std::ofstream f( name_file, std::ios::binary );
                PrintPackCmd( f, std::get<0>( p ) );
            }

            /// Выводим статистику потока
            std::stringstream ss;
            ss << "Thread file " << std::this_thread::get_id() << ":\n" <<
                "Count block = " << count_block << '\n' <<
                "Count cmd = " << count_cmd << '\n';
            std::cout << ss.str();
        }
        catch( ... )
        {
        }
    }

    std::function<void()> m_burden;
    std::vector<std::thread> m_assistans;
    // храним в очереди: число пак комманд, время первой комманыд
    threadsafe_queue_const < std::pair < pack_cmd_t, uint64_t > > m_pack_cmd;
};