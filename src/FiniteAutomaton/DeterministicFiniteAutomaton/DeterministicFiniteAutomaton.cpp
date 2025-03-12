#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicFiniteAutomaton.h"
#include "../FiniteAutomatonException.h"

void DeterministicFiniteAutomaton::checkAlphabetValidity(const std::vector<std::string> &inputAlphabet) const {
	for (auto &symbol : inputAlphabet) {
		if (symbol == "") {
			throw InvalidTransitionException("DFA transition cannot have epsilon as input");
		}
	}
}

bool DeterministicFiniteAutomaton::checkTransitionDeterminisim(const std::string &fromStateKey,
                                                               const std::string &input) {
	FAState *fromState = getState(fromStateKey);
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input) {
			return false;
		}
	}
	return true;
}

void DeterministicFiniteAutomaton::setInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	checkAlphabetValidity(inputAlphabet);

	FiniteAutomaton::setInputAlphabet(inputAlphabet);
}

void DeterministicFiniteAutomaton::addInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	checkAlphabetValidity(inputAlphabet);

	FiniteAutomaton::addInputAlphabet(inputAlphabet);
}

void DeterministicFiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &input,
                                                 const std::string &toStateKey) {

	// Check if the input is in the alphabet
	if (!FiniteAutomaton::inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::addTransition(fromStateKey, input, toStateKey);
}

void DeterministicFiniteAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		std::string toStateKey = FATransition::getToStateFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::updateTransitionInput(transitionKey, input);
}

void DeterministicFiniteAutomaton::updateTransitionFromState(const std::string &transitionKey,
                                                             const std::string &fromStateKey) {
	std::string input = FATransition::getInputFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		std::string toStateKey = FATransition::getToStateFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::updateTransitionFromState(transitionKey, fromStateKey);
}

bool DeterministicFiniteAutomaton::processInput(const std::string &input) {
	FAState *simulationCurrentState = FiniteAutomaton::getState(currentState);

	// Loop over the transitions and traverse the match if any
	for (const auto &transition : simulationCurrentState->getTransitions()) {
		if (transition.getInput() == input) {
			// Update state
			simulationCurrentState = getState(transition.getToStateKey());
			// Break after match is found
			break;
		}
	}

	// Update current state and return whether state is accept or not
	currentState = simulationCurrentState->getKey();
	return simulationCurrentState->getIsAccept();
}

bool DeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input,
                                            const int &simulationDepth = DEFAULT_SIMULATION_DEPTH) {
	FAState *simulationCurrentState = getState(getStartState());

	int simulationDepthCounter = 0;
	// Loop over the input and look for matching transitions
	for (const auto &value : input) {
		// Loop over the transitions and traverse the match if any
		for (auto &transition : simulationCurrentState->getTransitions()) {
			if (transition.getInput() == value) {
				// Update state if match is found
				simulationCurrentState = getState(transition.getToStateKey());
				simulationDepthCounter++;
				break;
			}
		}
		// Simulation depth exceeded
		if (simulationDepthCounter > simulationDepth) {
			throw SimulationDepthExceededException(simulationDepth);
		}
	}

	return simulationCurrentState->getIsAccept();
}