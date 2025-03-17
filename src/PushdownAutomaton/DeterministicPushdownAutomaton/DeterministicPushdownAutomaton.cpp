#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicPushdownAutomaton.h"
#include <iostream>

bool DeterministicPushdownAutomaton::checkTransitionDeterminisim(const std::string &fromStateKey,
                                                                 const std::string &input,
                                                                 const std::string &stackSymbol) {

	PDAState *fromState = getStateInternal(fromStateKey);

	// Check if there's already a transition with the same input/stack symbol
	for (auto &transition : fromState->getTransitions()) {
		// Conflicting transitions needs to have same stack symbol
		if (transition.getStackSymbol() != stackSymbol) {
			continue;
		}
		// For a DPDA, we can't have multiple transitions with:
		// 1. Same input symbol (or both epsilon)
		if (transition.getInput() == input) {
			return false;
		}

		// 2. Both a non-empty input and an epsilon transition
		if ((transition.getInput().empty() && !input.empty()) || (!transition.getInput().empty() && input.empty())) {
			return false;
		}
	}
	return true;
}

void DeterministicPushdownAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                                   const std::string &input, const std::string &stackSymbol,
                                                   const std::string &pushSymbol) {
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " +
		                                          toStateKey + "| input: " + input + " | stack symbol: " + stackSymbol +
		                                          " | push symbol: " + pushSymbol);
	}

	PushdownAutomaton::addTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
}

void DeterministicPushdownAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
		std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " +
		                                          toStateKey + "| input: " + input + " | stack symbol: " + stackSymbol +
		                                          " | push symbol: " + pushSymbol);
	}

	PushdownAutomaton::updateTransitionInput(transitionKey, input);
}

void DeterministicPushdownAutomaton::updateTransitionFromState(const std::string &transitionKey,
                                                               const std::string &fromStateKey) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
		std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " +
		                                          toStateKey + "| input: " + input + " | stack symbol: " + stackSymbol +
		                                          " | push symbol: " + pushSymbol);
	}

	PushdownAutomaton::updateTransitionFromState(transitionKey, fromStateKey);
}

void DeterministicPushdownAutomaton::updateTransitionStackSymbol(const std::string &transitionKey,
                                                                 const std::string &stackSymbol) {
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, stackSymbol)) {
		std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
		std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " +
		                                          toStateKey + "| input: " + input + " | stack symbol: " + stackSymbol +
		                                          " | push symbol: " + pushSymbol);
	}

	PushdownAutomaton::updateTransitionStackSymbol(transitionKey, stackSymbol);
}

bool DeterministicPushdownAutomaton::processInput(const std::string &input) {
	const std::vector<PDATransition> &transitions = getStateInternal(currentState)->getTransitions();
	std::string stackTop = stack.empty() ? "" : stack.top();

	for (const auto &transition : transitions) {
		if (transition.getStackSymbol() != stackTop) {
			continue;
		}
		if (transition.getInput() == input) {
			if (!transition.getStackSymbol().empty()) {
				stack.pop();
			}
			std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
			for (const auto &symbol : pushSymbolsVec) {
				stack.push(symbol);
			}
			currentState = transition.getToStateKey();
			continue;
		}
	}

	return getStateInternal(currentState)->getIsAccept();
}

bool DeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	size_t inputIdx = 0;
	size_t currentDepth = 0; // Add a depth counter separate from input index
	std::string simulationCurrentState = getStartState();
	std::stack<std::string> simulationStack;
	simulationStack.push(INITIAL_STACK_SYMBOL);

	while (currentDepth <= simulationDepth) {
		bool transitioned = false;
		const std::vector<PDATransition> &transitions = getStateInternal(simulationCurrentState)->getTransitions();
		std::string stackTop = simulationStack.empty() ? "" : simulationStack.top();

		// Process input transitions
		if (inputIdx < input.size()) {
			std::string currentInput = input[inputIdx];
			for (const auto &transition : transitions) {
				if (transition.getInput() == currentInput && transition.getStackSymbol() == stackTop) {
					// Process stack updates
					if (!transition.getStackSymbol().empty()) {
						simulationStack.pop();
					}
					std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
					for (const auto &symbol : pushSymbolsVec) {
						simulationStack.push(symbol);
					}
					// Move to the next state
					simulationCurrentState = transition.getToStateKey();
					transitioned = true;
				}
			}
		}

		// Process epsilon transitions
		if (!transitioned) {
			for (const auto &transition : transitions) {
				if (transition.getInput().empty() && transition.getStackSymbol() == stackTop) {
					// Process stack updates
					if (!transition.getStackSymbol().empty()) {
						simulationStack.pop();
					}
					std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
					for (const auto &symbol : pushSymbolsVec) {
						simulationStack.push(symbol);
					}

					simulationCurrentState = transition.getToStateKey();
					transitioned = true;
					break;
				}
			}
		}
		currentDepth++;

		// Exit conditions:
		// 1) No transition available and all input processed
		// 2) Simulation depth exceeded
		if (!transitioned) {
			if (inputIdx <= input.size()) {
				inputIdx++;
			} else {
				break;
			}
		}

		if (currentDepth > simulationDepth) {
			throw SimulationDepthExceededException(simulationDepth);
		}
	}

	return getStateInternal(simulationCurrentState)->getIsAccept();
}