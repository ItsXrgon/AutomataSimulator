#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicFiniteAutomaton.h"
#include <iostream>

void DeterministicFiniteAutomaton::checkAlphabetValidity(const std::vector<std::string> &inputAlphabet) const {
	for (auto &symbol : inputAlphabet) {
		if (symbol == "") {
			throw InvalidAlphabetException("DFA alphabet can not include epsilon");
		}
	}
}

bool DeterministicFiniteAutomaton::checkTransitionDeterminisim(const std::string &fromStateKey,
                                                               const std::string &input) {
	FAState *fromState = getStateInternal(fromStateKey);

	// in a DFA, we can't have multiple transitions with the same input symbol
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input) {
			return false;
		}
	}
	return true;
}

void DeterministicFiniteAutomaton::setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict) {
	checkAlphabetValidity(inputAlphabet);

	FiniteAutomaton::setInputAlphabet(inputAlphabet, strict);
}

void DeterministicFiniteAutomaton::addInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	checkAlphabetValidity(inputAlphabet);

	FiniteAutomaton::addInputAlphabet(inputAlphabet);
}

void DeterministicFiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                                 const std::string &input) {
	validateTransition(fromStateKey, toStateKey, input);

	// Check if the input is in the alphabet
	if (!inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::addTransition(fromStateKey, toStateKey, input);
}

void DeterministicFiniteAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = FATransition::getToStateFromKey(transitionKey);

	// Check if the input is in the alphabet
	if (!inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	validateTransition(fromStateKey, toStateKey, input);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::updateTransitionInput(transitionKey, input);
}

void DeterministicFiniteAutomaton::updateTransitionFromState(const std::string &transitionKey,
                                                             const std::string &fromStateKey) {
	std::string input = FATransition::getInputFromKey(transitionKey);
	std::string toStateKey = FATransition::getToStateFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input)) {
		throw InvalidAutomatonDefinitionException("Transition is not deterministic: " + fromStateKey + " -> " + input +
		                                          " -> " + toStateKey);
	}

	FiniteAutomaton::updateTransitionFromState(transitionKey, fromStateKey);
}

bool DeterministicFiniteAutomaton::processInput(const std::string &input) {
	const std::vector<FATransition> &transitions = getStateInternal(currentState)->getTransitions();

	for (const auto &transition : transitions) {
		if (transition.getInput() == input) {
			currentState = transition.getToStateKey();
			break;
		}
	}

	return getStateInternal(currentState)->getIsAccept();
}

bool DeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	size_t inputIdx = 0;
	size_t currentDepth = 0;
	std::string simulationCurrentState = getStartState();

	while (currentDepth <= simulationDepth) {
		bool transitioned = false;
		const std::vector<FATransition> &transitions = getStateInternal(simulationCurrentState)->getTransitions();

		// First try to process the next input symbol if available
		if (inputIdx < input.size()) {
			std::string currentInput = input[inputIdx];

			for (const auto &transition : transitions) {
				if (transition.getInput() == currentInput) {
					// Process transition
					simulationCurrentState = transition.getToStateKey();
					transitioned = true;
					inputIdx++;
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
	}

	// Check if we exceeded simulation depth
	if (currentDepth > simulationDepth) {
		throw SimulationDepthExceededException(simulationDepth);
	}

	return getStateInternal(simulationCurrentState)->getIsAccept();
}