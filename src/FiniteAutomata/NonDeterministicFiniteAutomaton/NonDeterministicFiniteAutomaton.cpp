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

		FAState *state = getState(currentStateKey);

		// Find all epsilon transitions from this state
		for (const auto &transition : state->getTransitions()) {
			// Assuming empty string "" represents epsilon transitions
			if (transition.getInput().empty()) {
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

std::string NonDeterministicFiniteAutomaton::decideRandomState(const std::unordered_set<std::string> &states) {
	// Randomly choose a state from the set of states
	int randomIndex = rand() % states.size();
	auto it = states.begin();
	std::advance(it, randomIndex);
	return *it;
}

void NonDeterministicFiniteAutomaton::setStartState(const std::string &key) {
	FiniteAutomaton::setStartState(key);

	if (possibleCurrentStates.empty()) {
		possibleCurrentStates.insert(startState);
	}
}

void NonDeterministicFiniteAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                                           const std::string &toStateKey) {
	FiniteAutomaton::validateTransition(fromStateKey, input, toStateKey);

	// Check if the input is in the alphabet if not epsilon
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::addTransitionBetween(fromStateKey, input, toStateKey);
}

std::unordered_set<std::string> NonDeterministicFiniteAutomaton::getPossibleCurrentStates() const {
	return possibleCurrentStates;
}

void NonDeterministicFiniteAutomaton::reset() {
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(startState);
	currentState = startState;
}

bool NonDeterministicFiniteAutomaton::processInput(const std::string &input) {
	possibleCurrentStates.clear();
	std::unordered_set<std::string> nextStates;

	FAState *state = getState(currentState);
	for (const auto &transition : state->getTransitions()) {
		if (transition.getInput() == input) {
			nextStates.insert(transition.getToStateKey());
		}
	}

	if (nextStates.empty()) {
		return getState(currentState)->getIsAccept();
	}

	addEpsilonClosure(nextStates);
	possibleCurrentStates = nextStates;

	currentState = decideRandomState(possibleCurrentStates);

	// We're in an accept state if the current state is an accept state
	if (getState(currentState)->getIsAccept()) {
		return true;
	}

	return false;
}

bool NonDeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input) {
	// Keep track of all current possible states
	std::unordered_set<std::string> simulationCurrentStates;

	// Start with the initial state and its epsilon closure
	simulationCurrentStates.insert(getStartState());
	addEpsilonClosure(simulationCurrentStates);

	// Process each input symbol
	for (const auto &value : input) {
		std::unordered_set<std::string> nextStates;

		// Explore transitions from each current state
		for (const auto &stateKey : simulationCurrentStates) {
			FAState *state = getState(stateKey);

			// Process all valid transitions for the current input symbol
			for (const auto &transition : state->getTransitions()) {
				if (transition.getInput() == value) {
					nextStates.insert(transition.getToStateKey());
				}
			}
		}

		// If no valid transitions are found, input is rejected
		if (nextStates.empty()) {
			return false;
		}

		// Update current states to the next states and apply epsilon closure
		simulationCurrentStates = nextStates;
		addEpsilonClosure(simulationCurrentStates);
	}

	// Check if any of the final states is an accepting state
	for (const auto &stateKey : simulationCurrentStates) {
		if (getState(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}
