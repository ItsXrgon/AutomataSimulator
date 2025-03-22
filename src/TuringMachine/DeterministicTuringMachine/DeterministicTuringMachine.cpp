#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicTuringMachine.h"

bool DeterministicTuringMachine::checkTransitionDeterminisim(const std::string &fromStateKey, const std::string &input,
                                                             const std::string &readSymbol) {
	TMState *fromState = getStateInternal(fromStateKey);

	// Check if there's already a transition with the same input/read symbol
	for (auto &transition : fromState->getTransitions()) {
		// For a DTM, we can't have multiple transitions with:
		// 1. Same read symbol (or both epsilon)
		if (transition.getReadSymbol() == input) {
			return false;
		}

		// 2. Both a non-empty input and an epsilon transition
		if ((transition.getReadSymbol().empty() && !input.empty()) ||
		    (!transition.getReadSymbol().empty() && input.empty())) {
			return false;
		}
	}
	return true;
}

void DeterministicTuringMachine::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                               const std::string &readSymbol, const std::string &writeSymbol,
                                               TMDirection direction) {
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, readSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::addTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

void DeterministicTuringMachine::updateTransitionFromState(const std::string &transitionKey,
                                                           const std::string &fromStateKey) {
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, readSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::updateTransitionFromState(transitionKey, fromStateKey);
}

void DeterministicTuringMachine::updateTransitionReadSymbol(const std::string &transitionKey,
                                                            const std::string &readSymbol) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, readSymbol)) {
		std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
		std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
		TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::updateTransitionReadSymbol(transitionKey, readSymbol);
}

bool DeterministicTuringMachine::processInput() {
	if (currentState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state or start state must be set to run process input");
	}

	std::string input = "";
	if (inputHead < this->input.size()) {
		input = this->input[inputHead];
	}

	const std::vector<TMTransition> &transitions = getStateInternal(currentState)->getTransitions();
	std::string tapeValue = tape.read();

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() != tapeValue && transition.getReadSymbol() != tape.getBlankSymbol()) {
			continue;
		}
		if (transition.getInput() == input || transition.getInput().empty()) {
			// If the write symbol is not epsilon then write to the tape
			if (!transition.getWriteSymbol().empty()) {
				tape.write(transition.getWriteSymbol());
			}
			tape.move(transition.getDirection());
			currentState = transition.getToStateKey();
			// Only increment the head if the input is a match and the input head is less than the input size
			const bool &incrementHead = transition.getInput() == input && inputHead < this->input.size();
			if (incrementHead) {
				inputHead++;
			}
			return getStateInternal(currentState)->getIsAccept();
		}
	}

	return false;
}

bool DeterministicTuringMachine::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state must be set to run simulate");
	}

	int inputIdx = 0;
	int currentDepth = 0;
	std::string simulationCurrentState = getStartState();
	TMTape simulationTape;

	while (currentDepth <= simulationDepth && inputIdx < input.size()) {
		const std::vector<TMTransition> &transitions = getStateInternal(simulationCurrentState)->getTransitions();

		std::string currentInput = "";
		if (inputIdx < input.size()) {
			currentInput = input[inputIdx];
		}

		std::string tapeValue = simulationTape.read();

		bool transitionFound = false;
		for (const auto &transition : transitions) {
			if (transition.getReadSymbol() != tapeValue && !transition.getReadSymbol().empty()) {
				continue;
			}
			if (transition.getInput() == currentInput || transition.getInput().empty()) {
				if (!transition.getWriteSymbol().empty()) {
					simulationTape.write(transition.getWriteSymbol());
				}
				simulationTape.move(transition.getDirection());
				currentState = transition.getToStateKey();
				// Only increment the head if the input is a match and the input head is less than the input size
				const bool &incrementHead = transition.getInput() == currentInput && inputIdx < input.size();
				if (incrementHead) {
					inputIdx++;
				}
				transitionFound = true;
				break;
			}
		}
		if (!transitionFound && !currentInput.empty()) {
			return false;
		}
		currentDepth++;
	}

	return getStateInternal(simulationCurrentState)->getIsAccept();
}