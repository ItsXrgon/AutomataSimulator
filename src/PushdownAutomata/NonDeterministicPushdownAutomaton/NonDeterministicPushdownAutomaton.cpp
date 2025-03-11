#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicPushdownAutomaton.h"
#include "../PushdownAutomatonException.h"

bool NonDeterministicPushdownAutomaton::processInput(const std::string &input) {
	// Initialize the set of possible current states with the start state
	if (possibleCurrentStates.empty()) {
		possibleCurrentStates.insert(startState);
	}

	std::unordered_set<std::string> nextStates;

	// Process epsilon closures for current states
	addEpsilonClosure(possibleCurrentStates, stack);

	// Process the input and compute all possible next states
	for (const auto &stateKey : possibleCurrentStates) {
		PDAState *state = getState(stateKey);
		std::string stackTop = stack.empty() ? "" : stack.top();

		for (const auto &transition : state->getTransitions()) {
			if (transition.getInput() == input && transition.getStackSymbol() == stackTop) {
				// Move to the next state
				nextStates.insert(transition.getToStateKey());

				// Update the stack
				if (!transition.getPushSymbol().empty()) {
					if (!stack.empty() && transition.getStackSymbol() == stackTop) {
						stack.pop(); // Pop the stack top if it matches
					}
					for (char symbol : transition.getPushSymbol()) {
						stack.push(std::string(1, symbol)); // Push symbols onto the stack
					}
				}
			}
		}
	}

	// Update current states to the next states
	possibleCurrentStates = nextStates;

	// Process epsilon closures for the next states
	addEpsilonClosure(possibleCurrentStates, stack);

	// We're in an accept state if any of the possible current states is an accept state
	for (const auto &stateKey : possibleCurrentStates) {
		if (getState(stateKey)->getIsAccept()) {
			return true;
		}
	}

	return false;
}

bool NonDeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input) {
	std::stack<std::string> simulationStack = std::stack<std::string>();
	simulationStack.push(INITIAL_STACK_SYMBOL);

	// Start with the start state
	std::unordered_set<std::string> currentStates = {startState};

	// Process epsilon closures for the start state
	addEpsilonClosure(currentStates, simulationStack);

	// Process each input
	for (const auto &value : input) {
		std::unordered_set<std::string> nextStates;

		// For each current state, check all transitions
		for (const auto &stateKey : currentStates) {
			PDAState *state = getState(stateKey);
			std::string stackTop = simulationStack.empty() ? "" : simulationStack.top();

			// Check all transitions from this state
			for (const auto &transition : state->getTransitions()) {
				if (transition.getInput() == value && transition.getStackSymbol() == stackTop) {
					// Move to the next state
					nextStates.insert(transition.getToStateKey());

					// Update the stack
					if (!transition.getPushSymbol().empty()) {
						if (!simulationStack.empty() && transition.getStackSymbol() == stackTop) {
							simulationStack.pop(); // Pop the stack top if it matches
						}
						for (char symbol : transition.getPushSymbol()) {
							simulationStack.push(std::string(1, symbol)); // Push symbols onto the stack
						}
					}
				}
			}
		}

		// Update current states to next states
		currentStates = nextStates;

		// Process epsilon closures for the next states
		addEpsilonClosure(currentStates, simulationStack);

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