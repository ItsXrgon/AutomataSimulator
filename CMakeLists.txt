cmake_minimum_required(VERSION 3.16)
project(AutomataSimulator)
set(CMAKE_CXX_STANDARD 17)

# Define the library type (SHARED for DLL)
option(BUILD_SHARED_LIBS "Build AutomataSimulator as a shared library" ON)

# Include directory for public headers
include_directories(include)

# Add source directory
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(Tests)