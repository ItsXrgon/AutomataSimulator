#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicPushdownAutomaton.h"
#include "../PushdownAutomatonException.h"

bool NonDeterministicPushdownAutomaton::processInput(const std::string &input) {}

bool NonDeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input,
                                                 const int &simulationDepth = DEFAULT_SIMULATION_DEPTH) {}
