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

bool DeterministicPushdownAutomaton::processInput() {
	if (currentState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state or start state must be set to run process input");
	}

	std::string input = "";
	if (inputHead < this->input.size()) {
		input = this->input[inputHead];
	}

	const std::vector<PDATransition> &transitions = getStateInternal(currentState)->getTransitions();
	const std::string &stackTop = stack.empty() ? "" : stack.top();

	for (const auto &transition : transitions) {
		// Stack symbols have to match
		if (transition.getStackSymbol() != stackTop && !transition.getStackSymbol().empty()) {
			continue;
		}
		// Inputs have to match
		if (transition.getInput() != input && !transition.getInput().empty()) {
			continue;
		}
		if (!transition.getStackSymbol().empty()) {
			stack.pop();
		}
		std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
		for (const auto &symbol : pushSymbolsVec) {
			stack.push(symbol);
		}
		currentState = transition.getToStateKey();
		// Only increment the head if the input is a match and the input head is less than the input size
		const bool &incrementHead = transition.getInput() == input && inputHead < this->input.size();
		if (incrementHead) {
			inputHead++;
		}
		return getStateInternal(currentState)->getIsAccept();
	}

	return false;
}

bool DeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state must be set to run simulate");
	}

	int inputIdx = 0;
	int currentDepth = 0;
	std::string simulationCurrentState = getStartState();
	std::stack<std::string> simulationStack;
	simulationStack.push(INITIAL_STACK_SYMBOL);

	while (currentDepth <= simulationDepth) {
		const std::vector<PDATransition> &transitions = getStateInternal(simulationCurrentState)->getTransitions();

		std::string currentInput = "";
		if (inputIdx < input.size()) {
			currentInput = input[inputIdx];
		}

		std::string stackTop = simulationStack.empty() ? "" : simulationStack.top();

		bool transitionFound = false;
		for (const auto &transition : transitions) {
			// Stack symbols have to match
			if (transition.getStackSymbol() != stackTop && !transition.getStackSymbol().empty()) {
				continue;
			}
			// Inputs have to match
			if (transition.getInput() != currentInput && !transition.getInput().empty()) {
				continue;
			}
			if (!transition.getStackSymbol().empty()) {
				simulationStack.pop();
			}
			std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
			for (const auto &symbol : pushSymbolsVec) {
				simulationStack.push(symbol);
			}
			simulationCurrentState = transition.getToStateKey();
			// Only increment the head if the input is a match and the input head is less than the input size
			const bool &incrementHead = transition.getInput() == currentInput && inputIdx < input.size();
			if (incrementHead) {
				inputIdx++;
			}
			transitionFound = true;
			break;
		}
		if (!transitionFound && !currentInput.empty()) {
			return false;
		}
		currentDepth++;
	}

	return getStateInternal(simulationCurrentState)->getIsAccept();
}