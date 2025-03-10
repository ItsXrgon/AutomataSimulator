#define AUTOMATASIMULATOR_EXPORTS
#include "FiniteAutomaton.h"
#include "FiniteAutomatonException.h"

FiniteAutomaton::FiniteAutomaton() : startState("") {}

FiniteAutomaton::~FiniteAutomaton() {}

void FiniteAutomaton::addState(const std::string &label) {
	State state(label, false);
	// Check if the state already exists
	for (const auto &existingState : getStates()) {
		if (existingState.getKey() == state.getKey()) {
			throw InvalidAutomatonDefinitionException("State with label " + state.getLabel() + " already exists");
		}
	}

	states.push_back(state);
}

void FiniteAutomaton::setCurrentState(const std::string &state) {
	currentState = state;
}

std::string FiniteAutomaton::getCurrentState() const {
	if (currentState.empty()) {
		return getStartState();
	}
	return currentState;
}

std::vector<State> FiniteAutomaton::getStates() const {
	return states;
}

State *FiniteAutomaton::getState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			return &state;
		}
	}
	throw StateNotFoundException(key);
}

State FiniteAutomaton::getState(const std::string &key) const {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			return state;
		}
	}
	throw StateNotFoundException(key);
}

void FiniteAutomaton::removeState(const std::string &key) {
	for (auto it = states.begin(); it != states.end(); ++it) {
		if (it->getKey() == key) {
			states.erase(it);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void FiniteAutomaton::setAlphabet(const std::set<std::string> &alphabet) {
	this->alphabet = alphabet;
}

std::set<std::string> FiniteAutomaton::getAlphabet() const {
	return alphabet;
}

void FiniteAutomaton::addAlphabet(const std::set<std::string> &alphabet) {
	for (const auto &symbol : alphabet) {
		this->alphabet.insert(symbol);
	}
}

void FiniteAutomaton::setStartState(const std::string &key) {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			startState = key;
			return;
		}
	}
	throw StateNotFoundException(key);
}

void FiniteAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                           const std::string &toStateKey) {
	State *state = getState(fromStateKey);
	state->addTransitionTo(input, toStateKey);
}

void FiniteAutomaton::removeTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                              const std::string &toStateKey) {
	State *state = getState(fromStateKey);
	state->removeTransitionTo(input, toStateKey);
}

void FiniteAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	State *fromState = getState(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
}

void FiniteAutomaton::clearStateTransitions(const std::string &key) {
	State *state = getState(key);
	state->clearTransitions();
}

void FiniteAutomaton::clearTransitions() {
	for (auto &state : states) {
		state.clearTransitions();
	}
}

std::string FiniteAutomaton::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void FiniteAutomaton::addAcceptState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			state.setIsAccept(true);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void FiniteAutomaton::removeAcceptState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			state.setIsAccept(false);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void FiniteAutomaton::clearAcceptStates() {
	for (auto &state : states) {
		state.setIsAccept(false);
	}
}

std::vector<State> FiniteAutomaton::getAcceptStates() const {
	std::vector<State> acceptStates;
	for (const auto &state : states) {
		if (state.getIsAccept()) {
			acceptStates.push_back(state);
		}
	}
	return acceptStates;
}