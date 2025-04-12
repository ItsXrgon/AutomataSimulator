file(GLOB_RECURSE SOURCES 
    "*.cpp"
)

# Create the shared library (DLL)
add_library(AutomataSimulator SHARED ${SOURCES})

add_library(AutomataSimulatorWrappers SHARED
    src/exports/DFAWrapper.cpp
    src/exports/NFAWrapper.cpp)
# Set up include directories

target_include_directories(AutomataSimulator
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Define export macro when building the DLL
target_compile_definitions(AutomataSimulator PRIVATE AUTOMATASIMULATOR_EXPORTS)

# Set up installation
install(TARGETS AutomataSimulator
    EXPORT AutomataSimulatorTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)