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

void NonDeterministicFiniteAutomaton::removeState(const std::string &key, const bool &strict) {
	FiniteAutomaton::removeState(key, strict);

	if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
		possibleCurrentStates.erase(key);
	}
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::removeStates(const std::vector<std::string> &keys, const bool &strict) {
	FiniteAutomaton::removeStates(keys, strict);
	
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
	// Start with the initial state
	std::unordered_set<std::string> currentStates;
	currentStates.insert(startState);

	// Compute epsilon closure of the initial state
	addEpsilonClosure(currentStates);

	// Process each input symbol
	for (size_t inputIdx = 0; inputIdx < input.size(); inputIdx++) {
		// Get next states for current input symbol
		std::unordered_set<std::string> nextStates;

		for (const auto &stateKey : currentStates) {
			FAState *state = getStateInternal(stateKey);

			// Find all transitions that match the current input
			for (const auto &transition : state->getTransitions()) {
				if (transition.getInput() == input[inputIdx]) {
					nextStates.insert(transition.getToStateKey());
				}
			}
		}

		// Compute epsilon closure of the resulting states
		addEpsilonClosure(nextStates);

		// If no valid transitions are found, input is rejected
		if (nextStates.empty()) {
			return false;
		}

		// Update current states to the next states
		currentStates = nextStates;

		// Check simulation depth to prevent infinite loops
		if (inputIdx >= simulationDepth) {
			throw SimulationDepthExceededException(simulationDepth);
		}
	}

	// After processing all input, check if any of the current states is an accepting state
	for (const auto &stateKey : currentStates) {
		if (getStateInternal(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}