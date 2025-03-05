#define AUTOMATASIMULATOR_EXPORTS
#include "PushdownAutomata.h"
#include "PushdownAutomataException.h"

PushdownAutomata::PushdownAutomata() : startState("") {
	stack.push(INITIAL_STACK_SYMBOL);
}

PushdownAutomata::~PushdownAutomata() {}

void PushdownAutomata::setCurrentState(const std::string &state) {
	currentState = state;
}

std::string PushdownAutomata::getCurrentState() const {
	if (currentState.empty()) {
		return getStartState();
	}
	return currentState;
}

void PushdownAutomata::addState(const std::string &label) {
	State state(label, false);
	// Check if the state already exists
	for (const auto &existingState : getStates()) {
		if (existingState.getKey() == state.getKey()) {
			throw InvalidAutomatonDefinitionException("State with label " + state.getLabel() + " already exists");
		}
	}

	states.push_back(state);
}

std::vector<State> PushdownAutomata::getStates() const {
	return states;
}

State *PushdownAutomata::getState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			return &state;
		}
	}
	throw StateNotFoundException(key);
}

State PushdownAutomata::getState(const std::string &key) const {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			return state;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomata::removeState(const std::string &key) {
	for (auto it = states.begin(); it != states.end(); ++it) {
		if (it->getKey() == key) {
			states.erase(it);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomata::setAlphabet(const std::set<std::string> &alphabet) {
	this->alphabet = alphabet;
}

std::set<std::string> PushdownAutomata::getAlphabet() const {
	return alphabet;
}

void PushdownAutomata::addAlphabet(const std::set<std::string> &alphabet) {
	for (const auto &symbol : alphabet) {
		this->alphabet.insert(symbol);
	}
}

void PushdownAutomata::setStartState(const std::string &key) {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			startState = key;
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomata::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                            const std::string &toStateKey, const std::string &stackSymbol,
                                            const std::string &pushSymbol) {
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
			throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " +
			                                          input + " -> " + toStateKey);
		}
	}

	State *state = getState(fromStateKey);
	state->addTransitionTo(input, toStateKey, stackSymbol, pushSymbol);
}

void PushdownAutomata::removeTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                               const std::string &toStateKey, const std::string &stackSymbol,
                                               const std::string &pushSymbol) {
	State *state = getState(fromStateKey);
	state->removeTransitionTo(input, toStateKey, stackSymbol, pushSymbol);
}

void PushdownAutomata::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	State *fromState = getState(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
}

void PushdownAutomata::clearStateTransitions(const std::string &key) {
	State *state = getState(key);
	state->clearTransitions();
}

void PushdownAutomata::clearTransitions() {
	for (auto &state : states) {
		state.clearTransitions();
	}
}

std::string PushdownAutomata::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void PushdownAutomata::addAcceptState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			state.setIsAccept(true);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomata::removeAcceptState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			state.setIsAccept(false);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomata::clearAcceptStates() {
	for (auto &state : states) {
		state.setIsAccept(false);
	}
}

std::vector<State> PushdownAutomata::getAcceptStates() const {
	std::vector<State> acceptStates;
	for (const auto &state : states) {
		if (state.getIsAccept()) {
			acceptStates.push_back(state);
		}
	}
	return acceptStates;
}

void PushdownAutomata::reset() {
	currentState = startState;
}

bool PushdownAutomata::processInput(const std::string &input) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	if (alphabet.empty()) {
		throw InvalidAlphabetException("Alphabet is not set");
	}

	// Get the current state
	State *state = getState(currentState);

	// Get the top of the stack
	std::string stackTop = stack.empty() ? "" : stack.top();

	// Iterate through transitions
	for (const auto &transition : state->getTransitions()) {
		// Check if the input and stack top match the transition
		if (transition.getInput() == input && transition.getStackSymbol() == stackTop) {
			// Move to the next state
			state = getState(transition.getToStateKey());
			currentState = state->getKey();

			// Update the stack
			if (!transition.getStackSymbol().empty()) {
				stack.pop();
			}
			if (!transition.getPushSymbol().empty()) {
				stack.push(transition.getPushSymbol()); 
			}

			// Return whether the new state is an accept state
			return state->getIsAccept();
		}
	}

	// If no matching transition is found, reject the input
	return false;
}

bool PushdownAutomata::simulate(const std::vector<std::string> &input) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	if (alphabet.empty()) {
		throw InvalidAlphabetException("Alphabet is not set");
	}

	// Create a stack for the automaton
	std::stack<std::string> stack;
	stack.push(INITIAL_STACK_SYMBOL);


	// Start from the initial state
	State *currentState = getState(getStartState());

	// Process each input symbol
	for (const auto &value : input) {
		std::string stackTop = stack.empty() ? "" : stack.top();

		// Iterate through transitions
		for (const auto &transition : currentState->getTransitions()) {
			// Check if the input and stack top match the transition
			if (transition.getInput() == value && transition.getStackSymbol() == stackTop) {
				// Move to the next state
				currentState = getState(transition.getToStateKey());

				// Update the stack
				if (!transition.getStackSymbol().empty()) {
					stack.pop();
				}
				if (!transition.getPushSymbol().empty()) {
					stack.push(transition.getPushSymbol());
				}
				break;
			}
		}

	}

	// Check if the final state is an accept state
	return currentState->getIsAccept();
}