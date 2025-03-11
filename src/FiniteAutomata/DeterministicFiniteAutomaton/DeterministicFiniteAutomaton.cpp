#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicFiniteAutomaton.h"
#include "../FiniteAutomatonException.h"

bool DeterministicFiniteAutomaton::checkAlphabetValidity(const std::set<std::string> &inputAlphabet) const {
	// Validate that the alphabet does not contain epsilon
	for (auto &symbol : inputAlphabet) {
		if (symbol == "") {
			throw InvalidAlphabetException("Alphabet cannot contain epsilon");
		}
	}
	return true;
}

void DeterministicFiniteAutomaton::setInputAlphabet(const std::set<std::string> &inputAlphabet) {
	checkAlphabetValidity(inputAlphabet);

	FiniteAutomaton::setInputAlphabet(inputAlphabet);
}

void DeterministicFiniteAutomaton::addInputAlphabet(const std::set<std::string> &inputAlphabet) {
	checkAlphabetValidity(inputAlphabet);

	FiniteAutomaton::addInputAlphabet(inputAlphabet);
}

void DeterministicFiniteAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                                        const std::string &toStateKey) {
	FiniteAutomaton::validateTransition(fromStateKey, input, toStateKey);
	FAState *fromState = getState(fromStateKey);

	// Check if the transition is deterministic
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input) {
			throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " +
			                                          input + " -> " + toStateKey);
		}
	}

	// Check if the input is in the alphabet
	if (inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::addTransitionBetween(fromStateKey, input, toStateKey);
}

void DeterministicFiniteAutomaton::reset() {
	currentState = startState;
}

bool DeterministicFiniteAutomaton::processInput(const std::string &input) {
	FAState *state = FiniteAutomaton::getState(currentState);
	for (const auto &transition : state->getTransitions()) {
		if (transition.getInput() == input) {
			state = getState(transition.getToStateKey());
			currentState = state->getKey();
			return state->getIsAccept();
		}
	}
	return false;
}

bool DeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input) {
	FAState *simulationCurrentState = getState(getStartState());

	for (const auto &value : input) {
		for (auto &transition : simulationCurrentState->getTransitions()) {
			if (transition.getInput() == value) {
				simulationCurrentState = getState(transition.getToStateKey());
			}
		}
	}
	return simulationCurrentState->getIsAccept(); // Check if the final state is accepting
}