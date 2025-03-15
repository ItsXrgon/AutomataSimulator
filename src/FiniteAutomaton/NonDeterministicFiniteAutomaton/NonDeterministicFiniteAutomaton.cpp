#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicFiniteAutomaton.h"

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

		FAState *state = getStateInternal(currentStateKey);

		// Find all epsilon transitions from this state
		for (const auto &transition : state->getTransitions()) {
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
	// If empty, return empty string
	if (states.empty()) {
		return "";
	}

	// Randomly choose a state from the set of states
	int randomIndex = rand() % states.size();
	auto it = states.begin();
	std::advance(it, randomIndex);
	return *it;
}

std::unordered_set<std::string>
NonDeterministicFiniteAutomaton::getNextPossibleStates(const std::unordered_set<std::string> &currentStates,
                                                       const std::string &input) {

	// Start with current states and compute their epsilon closure
	std::unordered_set<std::string> epsilonClosureStates = currentStates;
	addEpsilonClosure(epsilonClosureStates);

	// Get states reachable by the input from the epsilon closure
	std::unordered_set<std::string> nextStates;

	for (const auto &stateKey : currentStates) {
		FAState *state = getStateInternal(stateKey);

		for (const auto &transition : state->getTransitions()) {
			if (transition.getInput() == input) {
				nextStates.insert(transition.getToStateKey());
			}
		}
	}

	// Compute epsilon closure of the resulting states
	addEpsilonClosure(nextStates);

	return nextStates;
}

void NonDeterministicFiniteAutomaton::updateStateLabel(const std::string &key, const std::string &label) {
	FiniteAutomaton::updateStateLabel(key, label);
	if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
		possibleCurrentStates.erase(key);
		possibleCurrentStates.insert(label);
		possibleCurrentStatesCacheInvalidated = true;
	}
}

void NonDeterministicFiniteAutomaton::setCurrentState(const std::string &state) {
	FiniteAutomaton::setCurrentState(state);
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(state);
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::removeState(const std::string &key) {
	FiniteAutomaton::removeState(key);

	if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
		possibleCurrentStates.erase(key);
	}
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::removeStates(const std::vector<std::string> &keys) {
	FiniteAutomaton::removeStates(keys);
	for (const auto &key : keys) {
		if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
			possibleCurrentStates.erase(key);
		}
	}
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::clearStates() {
	FiniteAutomaton::clearStates();
	possibleCurrentStates.clear();
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::setStartState(const std::string &key) {
	FiniteAutomaton::setStartState(key);

	if (possibleCurrentStates.empty()) {
		possibleCurrentStates.insert(startState);
		possibleCurrentStatesCacheInvalidated = true;
	}
}

void NonDeterministicFiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                                    const std::string &input) {
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::addTransition(fromStateKey, toStateKey, input);
}

void NonDeterministicFiniteAutomaton::updateTransitionInput(const std::string &transitionKey,
                                                            const std::string &input) {
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::updateTransitionInput(transitionKey, input);
}

std::vector<std::string> NonDeterministicFiniteAutomaton::getPossibleCurrentStates() {
	// if the conversion cache from unordered_set to vector is not valid then recompute
	if (possibleCurrentStatesCacheInvalidated) {
		cachedPossibleCurrentStates.clear();
		for (const auto &state : possibleCurrentStates) {
			cachedPossibleCurrentStates.push_back(state);
		}
		possibleCurrentStatesCacheInvalidated = false;
	}
	return cachedPossibleCurrentStates;
}

void NonDeterministicFiniteAutomaton::reset() {
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(startState);
	addEpsilonClosure(possibleCurrentStates);
	currentState = startState;
	possibleCurrentStatesCacheInvalidated = true;
}

bool NonDeterministicFiniteAutomaton::processInput(const std::string &input) {
	// If input is empty (epsilon), we should follow epsilon transitions
	if (input.empty()) {
		// Compute new states reachable via epsilon transitions from current possible states
		std::unordered_set<std::string> nextStates = possibleCurrentStates;
		addEpsilonClosure(nextStates);

		// Only update if we found new states (this avoids unnecessary updates
		// when epsilon closure doesn't add any new states)
		if (nextStates != possibleCurrentStates) {
			possibleCurrentStates = nextStates;
			possibleCurrentStatesCacheInvalidated = true;

			// Choose a random state from the possible current states
			currentState = decideRandomState(possibleCurrentStates);
		}

		return getStateInternal(currentState)->getIsAccept();
	}

	// Compute the next possible states using our helper method
	std::unordered_set<std::string> nextStates = getNextPossibleStates(possibleCurrentStates, input);

	// If no valid transitions are found, remain in current state
	if (nextStates.empty()) {
		return getStateInternal(currentState)->getIsAccept();
	}

	// Update the possibleCurrentStates
	possibleCurrentStates = nextStates;
	possibleCurrentStatesCacheInvalidated = true;

	// Choose a random state from the possible current states
	currentState = decideRandomState(possibleCurrentStates);

	// Return whether the randomly chosen current state is an accept state
	return getStateInternal(currentState)->getIsAccept();
}

bool NonDeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	// Keep track of all current possible states
	std::unordered_set<std::string> simulationCurrentStates;

	// Start with the initial state and its epsilon closure
	simulationCurrentStates.insert(getStartState());
	addEpsilonClosure(simulationCurrentStates);

	// Process each input symbol
	for (size_t i = 0; i < input.size(); i++) {
		// Use our helper method to get all next possible states
		std::unordered_set<std::string> nextStates = getNextPossibleStates(simulationCurrentStates, input[i]);

		// If no valid transitions are found, input is rejected
		if (nextStates.empty()) {
			return false;
		}

		// Update current states to the next states
		simulationCurrentStates = nextStates;

		// Safety check to avoid infinite loops (e.g., due to epsilon cycles)
		// We compare with i to ensure we're making progress through the input
		if (i > simulationDepth) {
			throw SimulationDepthExceededException(simulationDepth);
		}
	}

	// Check if any of the final states is an accepting state
	for (const auto &stateKey : simulationCurrentStates) {
		if (getStateInternal(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}