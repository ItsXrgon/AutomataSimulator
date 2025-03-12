#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicPushdownAutomaton.h"
#include "../PushdownAutomatonException.h"

bool DeterministicPushdownAutomaton::checkTransitionDeterminisim(const std::string &fromStateKey,
                                                                 const std::string &input,
                                                                 const std::string &stackSymbol) {
	PDAState *fromState = getState(fromStateKey);
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input && transition.getStackSymbol() == stackSymbol) {
			return false;
		}
	}
	return true;
}

void DeterministicPushdownAutomaton::addTransition(const std::string &fromStateKey, const std::string &input,
                                                   const std::string &toStateKey, const std::string &stackSymbol,
                                                   const std::string &pushSymbol) {
	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey + " -> " + stackSymbol + " -> " + pushSymbol);
	}

	PushdownAutomaton::addTransition(fromStateKey, input, toStateKey, stackSymbol, pushSymbol);
}

void DeterministicPushdownAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
		std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey + " -> " + stackSymbol + " -> " + pushSymbol);
	}

	PushdownAutomaton::updateTransitionInput(transitionKey, input);
}

void DeterministicPushdownAutomaton::updateTransitionFromState(const std::string &transitionKey,
                                                               const std::string &fromStateKey) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	PushdownAutomaton::updateTransitionFromState(transitionKey, fromStateKey);
}

void DeterministicPushdownAutomaton::updateTransitionStackSymbol(const std::string &transitionKey,
                                                                 const std::string &stackSymbol) {
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string input = PDATransition::getInputFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
		std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey + " -> " + stackSymbol + " -> " + pushSymbol);
	}

	PushdownAutomaton::updateTransitionStackSymbol(transitionKey, stackSymbol);
}

bool DeterministicPushdownAutomaton::processInput(const std::string &input) {}

bool DeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input,
                                              const int &simulationDepth = DEFAULT_SIMULATION_DEPTH) {}
