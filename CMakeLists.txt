cmake_minimum_required(VERSION 3.16)

project(AutomataSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include_directories(include)

add_subdirectory(src)

install(DIRECTORY include/ DESTINATION include
        FILES_MATCHING PATTERN "*.h")