cmake_minimum_required(VERSION 3.2)

set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})
set( DOC_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Doc )
project(bulkmt VERSION 0.0.${PROJECT_VERSION_PATCH})

configure_file(version.h.in version.h @ONLY)

set(SRC
    main.cpp
    IState.cpp
    ISubject.cpp
    ObserverBase.cpp
    )

set(HDR
    IState.h
    IObserver.h
    ISubject.h
    ObserverBase.h
    threadsafe_queue.h
    ConsoleObserver.h
    FileObserver.h
    Statistic.h
    ThreadObserver.h
    ConsoleSubject.h
    ConsoleSubjectMT.h
    FileObserverMT.h
    IStreamSubject.h
    )

#Program bulkmt
add_executable(bulkmt ${SRC} ${HDR})

#Program generator file cmd by test
add_executable(gentest GeneratorFileCmd.cpp )

set_target_properties(bulkmt PROPERTIES
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}
)

#Дополнительный свойста компилятора для всех проектов
set_target_properties( bulkmt PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
   # COMPILE_OPTIONS -Wall
)

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT aquila-2004@yandex.ru)

include(CPack)
