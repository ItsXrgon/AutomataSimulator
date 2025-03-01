#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicFiniteAutomaton.h"
#include "../FiniteAutomatonException.h"

void NonDeterministicFiniteAutomaton::addEpsilonClosure(std::unordered_set<std::string> &states) {
	std::queue<std::string> stateQueue;

	// Initialize queue with all current states
	for (const auto &state : states) {
		stateQueue.push(state);
	}

	// Process queue to find all epsilon transitions
	while (!stateQueue.empty()) {
		std::string currentStateKey = stateQueue.front();
		stateQueue.pop();

		State *state = getState(currentStateKey);

		// Find all epsilon transitions from this state
		for (const auto &transition : state->getTransitions()) {
			// Assuming empty string "" represents epsilon transitions
			if (transition.getInput() == "") {
				std::string nextStateKey = transition.getToStateKey();

				// If this state hasn't been seen before, add it
				if (states.find(nextStateKey) == states.end()) {
					states.insert(nextStateKey);
					stateQueue.push(nextStateKey);
				}
			}
		}
	}
}

void NonDeterministicFiniteAutomaton::setAlphabet(const std::set<std::string> &alphabet) {
	FiniteAutomaton::setAlphabet(alphabet);
}

void NonDeterministicFiniteAutomaton::addAlphabet(const std::set<std::string> &alphabet) {
	FiniteAutomaton::addAlphabet(alphabet);
}

void NonDeterministicFiniteAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                                           const std::string &toStateKey) {
	// Check if the input is in the alphabet
	if (input != "" && alphabet.find(input) == alphabet.end()) {
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

	FiniteAutomaton::addTransitionBetween(fromStateKey, input, toStateKey);
}

void NonDeterministicFiniteAutomaton::reset() {
	currentStates.clear();
	currentStates.insert(startState);
}

bool NonDeterministicFiniteAutomaton::processInput(const std::string &input) {
	// Check if the start state is set
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	// Check if the alphabet is set
	if (alphabet.empty()) {
		throw InvalidAlphabetException("Alphabet is not set");
	}

	if (currentStates.empty()) {
		currentStates.insert(startState);
	}

	std::unordered_set<std::string> nextStates;

	// Process epsilon closures for current states
	addEpsilonClosure(currentStates);

	// Process the input and compute all possible next states
	for (const auto &stateKey : currentStates) {
		State *state = getState(stateKey);
		for (const auto &transition : state->getTransitions()) {
			if (transition.getInput() == input) {
				nextStates.insert(transition.getToStateKey());
			}
		}
	}

	// Update current states to the next states
	currentStates = nextStates;

	// Process epsilon closures for the next states
	addEpsilonClosure(currentStates);

	// We're in an accept state if any of the possible current states is an accept state
	for (const auto &stateKey : currentStates) {
		if (getState(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}

bool NonDeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input) {
	// For NFA, we need to keep track of all possible current states
	std::unordered_set<std::string> currentStates;

	// Start with the start state
	currentStates.insert(getStartState());

	// Process epsilon closures for the start state
	addEpsilonClosure(currentStates);

	// Process each input
	for (const auto &value : input) {
		std::unordered_set<std::string> nextStates;

		// For each current state, check all transitions
		for (const auto &stateKey : currentStates) {
			State *state = getState(stateKey);

			// Check all transitions from this state
			for (const auto &transition : state->getTransitions()) {
				if (transition.getInput() == value) {
					nextStates.insert(transition.getToStateKey());
				}
			}
		}

		// Update current states to next states
		currentStates = nextStates;

		// Process epsilon closures for the next states
		addEpsilonClosure(currentStates);

		// If there are no possible states, the input is rejected
		if (currentStates.empty()) {
			return false;
		}
	}

	// We're in an accept state if any of the possible final states is an accept state
	for (const auto &stateKey : currentStates) {
		if (getState(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}