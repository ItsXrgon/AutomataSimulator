# GoogleTest Dependency (Download and Link)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# Prevent GoogleTest from overriding standard options like CMAKE_CXX_FLAGS
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "*.cpp")

# Create test executable
add_executable(AutomataTests ${TEST_SOURCES})

# Link test executable with your AutomataSimulator library and GoogleTest
target_link_libraries(AutomataTests
    PRIVATE
        AutomataSimulator
        gtest_main
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(AutomataTests)
