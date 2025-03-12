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

std::unordered_set<std::string>
NonDeterministicFiniteAutomaton::getNextStatesOnInput(const std::unordered_set<std::string> &currentStates,
                                                      const std::string &input) {
	std::unordered_set<std::string> nextStates;

	for (const auto &stateKey : currentStates) {
		FAState *state = getState(stateKey);

		for (const auto &transition : state->getTransitions()) {
			if (transition.getInput() == input) {
				nextStates.insert(transition.getToStateKey());
			}
		}
	}

	return nextStates;
}

void NonDeterministicFiniteAutomaton::setStartState(const std::string &key) {
	FiniteAutomaton::setStartState(key);

	if (possibleCurrentStates.empty()) {
		possibleCurrentStates.insert(startState);
	}
}

void NonDeterministicFiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &input,
                                                    const std::string &toStateKey) {


	FiniteAutomaton::addTransition(fromStateKey, input, toStateKey);
}

void NonDeterministicFiniteAutomaton::updateTransitionInput(const std::string &transitionKey,
                                                            const std::string &input) {
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::updateTransitionInput(transitionKey, input);
}

std::unordered_set<std::string> NonDeterministicFiniteAutomaton::getPossibleCurrentStates() const {
	return possibleCurrentStates;
}

void NonDeterministicFiniteAutomaton::reset() {
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(startState);
	addEpsilonClosure(possibleCurrentStates);
	currentState = startState;
}

bool NonDeterministicFiniteAutomaton::processInput(const std::string &input) {
	std::unordered_set<std::string> currentStates = possibleCurrentStates;

	// If input is epsilon, just apply epsilon closure
	if (input.empty()) {
		// possibleCurrentStates already includes epsilon closure
		// Choose a random state from the possible current states
		if (!possibleCurrentStates.empty()) {
			currentState = decideRandomState(possibleCurrentStates);
			return getState(currentState)->getIsAccept();
		}
		// Check if the randomly chosen current state is an accept state
		return getState(currentState)->getIsAccept();
	}

	std::unordered_set<std::string> nextStates;

	// Case 1: First try direct transitions with the given input
	nextStates = getNextStatesOnInput(currentStates, input);

	// Apply epsilon closure to these next states
	addEpsilonClosure(nextStates);

	// Try epsilon transitions first, then input transitions
	std::unordered_set<std::string> epsilonTransitionStates;
	for (const auto &stateKey : currentStates) {
		FAState *state = getState(stateKey);

		// Find all states reachable via epsilon transitions
		for (const auto &transition : state->getTransitions()) {
			if (transition.getInput().empty()) {
				epsilonTransitionStates.insert(transition.getToStateKey());
			}
		}
	}

	// Apply input transitions to the epsilon-reached states
	std::unordered_set<std::string> postEpsilonNextStates = getNextStatesOnInput(epsilonTransitionStates, input);

	// Add these states to our next states
	nextStates.insert(postEpsilonNextStates.begin(), postEpsilonNextStates.end());

	// Apply epsilon closure again to the combined set
	addEpsilonClosure(nextStates);

	// If no valid transitions are found
	if (nextStates.empty()) {
		return getState(currentState)->getIsAccept();
	}

	// Update the possibleCurrentStates
	possibleCurrentStates = nextStates;

	// Choose a random state from the possible current states
	currentState = decideRandomState(possibleCurrentStates);

	// Check if the randomly chosen current state is an accept state
	return getState(currentState)->getIsAccept();
}

bool NonDeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input,
                                               const int &simulationDepth = DEFAULT_SIMULATION_DEPTH) {
	// Keep track of all current possible states
	std::unordered_set<std::string> simulationCurrentStates;

	// Start with the initial state and its epsilon closure
	simulationCurrentStates.insert(getStartState());
	addEpsilonClosure(simulationCurrentStates);

	// Track the input processing depth to avoid infinite loops
	int depth = 0;

	// Process each input symbol
	for (const auto &value : input) {
		std::unordered_set<std::string> nextStates;

		// Case 1: First try direct transitions with the given input
		nextStates = getNextStatesOnInput(simulationCurrentStates, value);

		// Apply epsilon closure to these next states
		addEpsilonClosure(nextStates);

		// Case 2: Try epsilon transitions first, then input transitions
		std::unordered_set<std::string> epsilonTransitionStates;
		for (const auto &stateKey : simulationCurrentStates) {
			FAState *state = getState(stateKey);

			// Find all states reachable via epsilon transitions
			for (const auto &transition : state->getTransitions()) {
				if (transition.getInput().empty()) {
					epsilonTransitionStates.insert(transition.getToStateKey());
				}
			}
		}

		// Apply input transitions to the epsilon-reached states
		std::unordered_set<std::string> postEpsilonNextStates = getNextStatesOnInput(epsilonTransitionStates, value);

		// Add these states to our next states
		nextStates.insert(postEpsilonNextStates.begin(), postEpsilonNextStates.end());

		// Apply epsilon closure again to the combined set
		addEpsilonClosure(nextStates);

		// If no valid transitions are found, input is rejected
		if (nextStates.empty()) {
			return false;
		}

		// Update current states to the next states
		simulationCurrentStates = nextStates;

		depth++;

		// Safety check to avoid infinite loops (e.g., due to epsilon cycles)
		// Maximum depth is the number of states times some factor
		if (depth > DEFAULT_SIMULATION_DEPTH * states.size()) {
			throw SimulationDepthExceededException(DEFAULT_SIMULATION_DEPTH);
		}
	}

	// Check if any of the final states is an accepting state
	// For simulation, we check ALL possible end states, unlike processInput
	for (const auto &stateKey : simulationCurrentStates) {
		if (getState(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}