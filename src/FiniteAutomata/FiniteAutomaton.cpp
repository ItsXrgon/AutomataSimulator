#define AUTOMATASIMULATOR_EXPORTS
#include "FiniteAutomaton.h"
#include "FiniteAutomatonException.h"

FiniteAutomaton::FiniteAutomaton() : startState("") {}

FiniteAutomaton::~FiniteAutomaton() {}

void FiniteAutomaton::addState(const std::string &label) {
	FAState state(label, false);
	// Check if the state already exists
	for (const auto &existingState : getStates()) {
		if (existingState.getKey() == state.getKey()) {
			throw InvalidAutomatonDefinitionException("State with label " + state.getLabel() + " already exists");
		}
	}

	states.push_back(state);
}

void FiniteAutomaton::validateTransition(const std::string &fromStateKey, const std::string &input,
                                         const std::string &toStateKey) {
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

	FAState *fromState = getState(fromStateKey);
	// Check if the transition already exists
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input && transition.getToStateKey() == toStateKey) {
			throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + input + " -> " +
			                                 toStateKey);
		}
	}
}

void FiniteAutomaton::setCurrentState(const std::string &state) {
	getState(state);
	currentState = state;
}

std::string FiniteAutomaton::getCurrentState() const {
	if (currentState.empty() && startState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state and start state are not set");
	}
	if (currentState.empty()) {
		return startState;
	}
	return currentState;
}

std::vector<FAState> FiniteAutomaton::getStates() const {
	return states;
}

FAState *FiniteAutomaton::getState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			return &state;
		}
	}
	throw StateNotFoundException(key);
}

FAState FiniteAutomaton::getState(const std::string &key) const {
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

void FiniteAutomaton::setInputAlphabet(const std::set<std::string> &inputAlphabet) {
	this->inputAlphabet = inputAlphabet;
}

std::set<std::string> FiniteAutomaton::getInputAlphabet() const {
	return inputAlphabet;
}

void FiniteAutomaton::addInputAlphabet(const std::set<std::string> &inputAlphabet) {
	for (const auto &symbol : inputAlphabet) {
		this->inputAlphabet.insert(symbol);
	}
}

void FiniteAutomaton::setStartState(const std::string &key) {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			startState = key;
			return;
		}
	}

	if (currentState.empty()) {
		currentState = key;
	}

	throw StateNotFoundException(key);
}

void FiniteAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                           const std::string &toStateKey) {
	FAState *state = getState(fromStateKey);
	state->addTransitionTo(input, toStateKey);
}

void FiniteAutomaton::removeTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                              const std::string &toStateKey) {
	FAState *state = getState(fromStateKey);
	state->removeTransitionTo(input, toStateKey);
}

void FiniteAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	FAState *fromState = getState(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
}

void FiniteAutomaton::clearStateTransitions(const std::string &key) {
	FAState *state = getState(key);
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

std::vector<FAState> FiniteAutomaton::getAcceptStates() const {
	std::vector<FAState> acceptStates;
	for (const auto &state : states) {
		if (state.getIsAccept()) {
			acceptStates.push_back(state);
		}
	}
	return acceptStates;
}