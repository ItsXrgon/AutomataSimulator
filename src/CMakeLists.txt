# Collect source files
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# Create the DLL
add_library(AutomataSimulator SHARED ${SOURCES} ${HEADERS})

# Define the export macro
target_compile_definitions(AutomataSimulator PRIVATE AUTOMATASIMULATOR_EXPORTS)

if(MSVC)
    target_compile_options(AutomataSimulator PRIVATE /wd4273)
endif()

# Set output properties for Windows
if(WIN32)
    # Ensure we get a proper .dll file (no lib prefix)
    set_target_properties(AutomataSimulator PROPERTIES 
        PREFIX ""
        OUTPUT_NAME "AutomataSimulator"
    )
    
    # Optional: Create a .def file for explicit exports
    if(MSVC)
        set(DEF_FILE "${CMAKE_CURRENT_BINARY_DIR}/exports.def")
        file(WRITE ${DEF_FILE} "EXPORTS\n")
        # Add your exported functions here
        file(APPEND ${DEF_FILE} "    AutomataSimulator_Create\n")
        file(APPEND ${DEF_FILE} "    AutomataSimulator_Destroy\n")
        # Add all other exported functions...
        
        # Link with the DEF file
        set_target_properties(AutomataSimulator PROPERTIES 
            LINK_FLAGS "/DEF:${DEF_FILE}")
    endif()
endif()

# Installation rules
install(TARGETS AutomataSimulator
    RUNTIME DESTINATION bin  # For .dll files on Windows
    LIBRARY DESTINATION lib  # For .so files on Linux/macOS
    ARCHIVE DESTINATION lib) # For .lib files on Windows