#define AUTOMATASIMULATOR_EXPORTS
#include "DeterministicTuringMachine.h"

bool DeterministicTuringMachine::checkTransitionDeterminisim(const std::string &fromStateKey, const std::string &input,
                                                             const std::string &readSymbol) {
	TMState *fromState = getStateInternal(fromStateKey);

	// Check if there's already a transition with the same input/read symbol
	for (auto &transition : fromState->getTransitions()) {
		// Conflicting transitions needs to have same read symbol
		if (transition.getReadSymbol() != readSymbol) {
			continue;
		}
		// For a DTM, we can't have multiple transitions with:
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

void DeterministicTuringMachine::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                               const std::string &input, const std::string &readSymbol,
                                               const std::string &writeSymbol, TMDirection direction) {
	validateTransition(fromStateKey, toStateKey, input, readSymbol, writeSymbol, direction);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, readSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          "| input: " + input + " | read symbol: " + readSymbol +
		                                          " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::addTransition(fromStateKey, toStateKey, input, readSymbol, writeSymbol, direction);
}

void DeterministicTuringMachine::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input, readSymbol, writeSymbol, direction);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, readSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          "| input: " + input + " | read symbol: " + readSymbol +
		                                          " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::updateTransitionInput(transitionKey, input);
}

void DeterministicTuringMachine::updateTransitionFromState(const std::string &transitionKey,
                                                           const std::string &fromStateKey) {
	std::string input = TMTransition::getInputFromKey(transitionKey);
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	validateTransition(fromStateKey, toStateKey, input, readSymbol, writeSymbol, direction);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, readSymbol)) {
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          "| input: " + input + " | read symbol: " + readSymbol +
		                                          " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::updateTransitionFromState(transitionKey, fromStateKey);
}

void DeterministicTuringMachine::updateTransitionReadSymbol(const std::string &transitionKey,
                                                            const std::string &readSymbol) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string input = TMTransition::getInputFromKey(transitionKey);

	// Check if the transition is deterministic
	if (!checkTransitionDeterminisim(fromStateKey, input, readSymbol)) {
		std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
		std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
		TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);
		throw InvalidAutomatonDefinitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                          "| input: " + input + " | read symbol: " + readSymbol +
		                                          " | write symbol: " + writeSymbol +
		                                          " | direction: " + TMDirectionHelper::toString(direction));
	}

	TuringMachine::updateTransitionReadSymbol(transitionKey, readSymbol);
}

bool DeterministicTuringMachine::processInput(const std::string &input) {
	const std::vector<TMTransition> &transitions = getStateInternal(currentState)->getTransitions();
	std::string tapeValue = tape.read();

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() != tapeValue) {
			continue;
		}
		if (transition.getInput() == input) {
			if (!transition.getWriteSymbol().empty()) {
				tape.write(transition.getWriteSymbol());
			}
			tape.move(transition.getDirection());
			continue;
		}
	}

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() != tapeValue) {
			continue;
		}
		if (transition.getInput().empty()) {
			if (!transition.getWriteSymbol().empty()) {
				tape.write(transition.getWriteSymbol());
			}
			tape.move(transition.getDirection());
			continue;
		}
	}

	return getStateInternal(currentState)->getIsAccept();
}

bool DeterministicTuringMachine::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	size_t inputIdx = 0;
	size_t currentDepth = 0;
	std::string simulationCurrentState = getStartState();
	TMTape simulationTape = TMTape();

	while (currentDepth <= simulationDepth) {
		bool transitioned = false;
		const std::vector<TMTransition> &transitions = getStateInternal(simulationCurrentState)->getTransitions();
		std::string tapeValue = simulationTape.read();

		// First try to process the next input symbol if available
		if (inputIdx < input.size()) {
			std::string currentInput = input[inputIdx];

			for (const auto &transition : transitions) {
				if (transition.getReadSymbol() == tapeValue && transition.getInput() == currentInput) {
					// Process transition
					if (!transition.getWriteSymbol().empty()) {
						simulationTape.write(transition.getWriteSymbol());
					}
					simulationTape.move(transition.getDirection());
					// Move to the next state
					simulationCurrentState = transition.getToStateKey();
					transitioned = true;
				}
			}
		}

		// If no input transition was taken, try epsilon transitions
		if (!transitioned) {
			for (const auto &transition : transitions) {
				if (transition.getReadSymbol() == tapeValue && transition.getInput().empty()) {
					// Process epsilon transition
					simulationCurrentState = transition.getToStateKey();
					if (!transition.getWriteSymbol().empty()) {
						simulationTape.write(transition.getWriteSymbol());
					}
					simulationTape.move(transition.getDirection());

					transitioned = true;
					break;
				}
			}
			currentDepth++;
		}

		// Exit conditions:
		// 1) No transition available and all input processed
		// 2) Simulation depth exceeded
		if (!transitioned) {
			if (inputIdx < input.size()) {
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