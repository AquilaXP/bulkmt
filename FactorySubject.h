#pragma once

#include <memory>
#include <string>

#include "ISubject.h"

/*!
    Фабрика
    доступно: Console, ConsoleMT
*/
std::unique_ptr<ISubject> FactorySubject( const std::string& name );
