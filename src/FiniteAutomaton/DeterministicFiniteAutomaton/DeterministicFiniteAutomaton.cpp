#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicFiniteAutomaton.h"
#include <iostream>

void DeterministicFiniteAutomaton::checkAlphabetValidity(const std::vector<std::string> &inputAlphabet) const {
	for (auto &symbol : inputAlphabet) {
		if (symbol == "") {
			throw InvalidAlphabetException("DFA alphabet can not include epsilon");
		}
	}
}

bool DeterministicFiniteAutomaton::checkTransitionDeterminisim(const std::string &fromStateKey,
                                                               const std::string &input) {
	FAState *fromState = getStateInternal(fromStateKey);

	// in a DFA, we can't have multiple transitions with the same input symbol
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

void DeterministicFiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                                 const std::string &input) {
	validateTransition(fromStateKey, toStateKey, input);

	// Check if the input is in the alphabet
	if (!inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::addTransition(fromStateKey, toStateKey, input);
}

void DeterministicFiniteAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = FATransition::getToStateFromKey(transitionKey);
	
	// Check if the input is in the alphabet
	if (!inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	validateTransition(fromStateKey, toStateKey, input);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::updateTransitionInput(transitionKey, input);
}

void DeterministicFiniteAutomaton::updateTransitionFromState(const std::string &transitionKey,
                                                             const std::string &fromStateKey) {
	std::string input = FATransition::getInputFromKey(transitionKey);
	std::string toStateKey = FATransition::getToStateFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::updateTransitionFromState(transitionKey, fromStateKey);
}

bool DeterministicFiniteAutomaton::processInput(const std::string &input) {
	FAState *simulationCurrentState = getStateInternal(currentState);

	// Loop over the transitions and traverse the match if any
	for (const auto &transition : simulationCurrentState->getTransitions()) {
		if (transition.getInput() == input) {
			// Update state
			simulationCurrentState = getStateInternal(transition.getToStateKey());
			// Break after match is found
			break;
		}
	}

	// Update current state and return whether state is accept or not
	currentState = simulationCurrentState->getKey();
	return simulationCurrentState->getIsAccept();
}

bool DeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	FAState *simulationCurrentState = getStateInternal(getStartState());

	int simulationDepthCounter = 0;

	// Loop over the input and look for matching transitions
	for (const auto &value : input) {
		// Loop over the transitions and traverse the match if any
		for (auto &transition : simulationCurrentState->getTransitions()) {
			if (transition.getInput() == value) {
				// Update state if match is found
				simulationCurrentState = getStateInternal(transition.getToStateKey());
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