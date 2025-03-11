# AutomataSimulator Engine
add_library(AutomataSimulator STATIC
    FiniteAutomaton/FiniteAutomaton.cpp
    State/State.cpp
    Transition/FATransition.cpp
)

# Add public headers for external usage
target_include_directories(AutomataSimulator PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
