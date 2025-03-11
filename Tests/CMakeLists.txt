# GoogleTest Dependency (Download and Link)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Tests
add_executable(AutomataTests DFATest.cpp)

# Link GoogleTest and AutomataSimulator
target_link_libraries(AutomataTests PRIVATE AutomataSimulator gtest_main)

# Register tests
include(GoogleTest)
gtest_discover_tests(AutomataTests)
