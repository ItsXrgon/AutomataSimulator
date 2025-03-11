#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicPushdownAutomaton.h"
#include "../PushdownAutomatonException.h"

void DeterministicPushdownAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                                          const std::string &toStateKey, const std::string &stackSymbol,
                                                          const std::string &pushSymbol) {
	PDAState *fromState = getState(fromStateKey);

	// Check if the transition is deterministic
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input && transition.getStackSymbol() == stackSymbol) {
			throw InvalidAutomatonDefinitionException("Non-deterministic transition: (" + fromStateKey + ", " + input +
			                                          ", " + stackSymbol + ") -> " + toStateKey + ", " + pushSymbol);
		}
	}

	PushdownAutomaton::addTransitionBetween(fromStateKey, input, toStateKey, stackSymbol, pushSymbol);
}

bool DeterministicPushdownAutomaton::processInput(const std::string &input) {
	// Get the current state
	PDAState *state = getState(currentState);

	// Get the top of the stack
	std::string stackTop = stack.empty() ? "" : stack.top();

	// Find a valid transition for the input and stack top
	for (const auto &transition : state->getTransitions()) {
		if (transition.getInput() == input && transition.getStackSymbol() == stackTop) {
			// Move to the next state
			state = getState(transition.getToStateKey());
			currentState = state->getKey();

			// Update the stack
			if (!transition.getPushSymbol().empty()) {
				if (!stack.empty() && transition.getStackSymbol() == stackTop) {
					stack.pop(); // Pop the stack top if it matches
				}
				stack.push(transition.getPushSymbol()); // Push symbols onto the stack
			}

			// Return whether the new state is an accept state
			return state->getIsAccept();
		}
	}

	// If no valid transition is found, reject the input
	return false;
}

bool DeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input) {
	// Initialize the simulation stack with the initial stack symbol
	std::stack<std::string> simulationStack;
	simulationStack.push(INITIAL_STACK_SYMBOL);

	// Get the initial state
	PDAState *currentState = getState(getStartState());

	// Apply epsilon closure for the starting configuration
	applyEpsilonClosure(currentState, simulationStack);

	// Process each input symbol
	for (const auto &value : input) {
		// If the stack is empty, use an empty string for stackTop
		std::string stackTop = simulationStack.empty() ? "" : simulationStack.top();

		// Explore current state's transitions
		for (const auto &transition : currentState->getTransitions()) {
			// Check for a valid transition: matching input and stack symbol
			if (transition.getInput() == value && transition.getStackSymbol() == stackTop) {
				// Get the next state and validate it
				PDAState *nextState = getState(transition.getToStateKey());
				if (!nextState)
					continue;

				// Update the stack (pop the top if it matches)
				std::stack<std::string> nextStack = simulationStack;
				if (!nextStack.empty() && transition.getStackSymbol() == stackTop) {
					nextStack.pop();
				}

				// Push new symbols onto the stack (in reverse order)
				for (const auto &symbol : transition.getPushSymbol()) {
					nextStack.push(std::string(1, symbol));
				}

				// Update the current state and stack
				currentState = nextState;
				simulationStack = nextStack;

				// Apply epsilon closure to explore any new states
				applyEpsilonClosure(currentState, simulationStack);
				break; // Proceed to the next input
			}
		}
	}

	// Final epsilon closure in case the accept state is reached via epsilon
	applyEpsilonClosure(currentState, simulationStack);

	// Check if the current state is an accept state
	return currentState && currentState->getIsAccept();
}
