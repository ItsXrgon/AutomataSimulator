#define AUTOMATASIMULATOR_EXPORTS 
#include "DeterministicFiniteAutomaton.h"
#include "../FiniteAutomatonException.h"

bool DeterministicFiniteAutomaton::checkAlphabetValidity(const std::set<std::string> &alphabet) const {
	// Validate that the alphabet does not contain epsilon
	for (auto &symbol : alphabet) {
		if (symbol == "") {
			throw InvalidAlphabetException("Alphabet cannot contain epsilon");
		}
	}
	return true;
}

void DeterministicFiniteAutomaton::setCurrentState(const std::string &state) {
	currentState = state;
}

std::string DeterministicFiniteAutomaton::getCurrentState() const {
	if (currentState.empty()) {
		return getStartState();
	}
	return currentState;
}

void DeterministicFiniteAutomaton::setAlphabet(const std::set<std::string> &alphabet) {
	checkAlphabetValidity(alphabet);

	FiniteAutomaton::setAlphabet(alphabet);
}

void DeterministicFiniteAutomaton::addAlphabet(const std::set<std::string> &alphabet) {
	checkAlphabetValidity(alphabet);

	FiniteAutomaton::addAlphabet(alphabet);
}

void DeterministicFiniteAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                                        const std::string &toStateKey) {
	// Check if the input is in the alphabet
	if (alphabet.find(input) == alphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	// Check if the "to" state exists
	bool toStateExists = false;
	for (const auto &s : states) {
		if (s.getKey() == toStateKey) {
			toStateExists = true;
			break;
		}
	}
	if (!toStateExists) {
		throw StateNotFoundException(toStateKey);
	}

	// Check if the "from" state exists
	bool fromStateExists = false;
	for (const auto &s : states) {
		if (s.getKey() == fromStateKey) {
			fromStateExists = true;
			break;
		}
	}
	if (!fromStateExists) {
		throw StateNotFoundException(fromStateKey);
	}

	State *fromState = getState(fromStateKey);
	// Check if the transition already exists
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input && transition.getToStateKey() == toStateKey) {
			throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + input + " -> " +
			                                 toStateKey);
		}
	}

	// Check if the transition is deterministic
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input) {
			throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
			                                 " -> " + toStateKey);
		}
	}

	FiniteAutomaton::addTransitionBetween(fromStateKey, input, toStateKey);
}

void DeterministicFiniteAutomaton::reset() {
	currentState = startState;
}

bool DeterministicFiniteAutomaton::processInput(const std::string &input) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	if (alphabet.empty()) {
		throw InvalidAlphabetException("Alphabet is not set");
	}
	State *state = FiniteAutomaton::getState(currentState);
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
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	if (alphabet.empty()) {
		throw InvalidAlphabetException("Alphabet is not set");
	}
	State *currentState = getState(getStartState());

	for (const auto &value : input) {
		for (auto &transition : currentState->getTransitions()) {
			if (transition.getInput() == value) {
				currentState = getState(transition.getToStateKey());
			}
		}
	}
	return currentState->getIsAccept(); // Check if the final state is accepting
}