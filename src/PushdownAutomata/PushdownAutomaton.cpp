#define AUTOMATASIMULATOR_EXPORTS
#include "PushdownAutomaton.h"
#include "PushdownAutomatonException.h"

const std::string INITIAL_STACK_SYMBOL = "Z";

PushdownAutomaton::PushdownAutomaton() : startState("") {
	stack.push(INITIAL_STACK_SYMBOL);
}

PushdownAutomaton::~PushdownAutomaton() {}

void PushdownAutomaton::validateStackAlphabet(const std::set<std::string> &stackAlphabet) const {
	// Validate that the stack alphabet does not contain epsilon
	for (auto &symbol : stackAlphabet) {
		if (symbol == "") {
			throw InvalidAlphabetException("Stack alphabet cannot contain epsilon");
		}
	}
}

void PushdownAutomaton::validateTransition(const std::string &fromStateKey, const std::string &input,
                                           const std::string &toStateKey, const std::string &stackSymbol,
                                           const std::string &pushSymbol) {
	// Validate that the stack alphabet does not contain epsilon
	// Check if the input is in the alphabet (including epsilon)
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input symbol '" + input + "' is not in the input alphabet.");
	}

	// Check if the "to" state exists
	bool toStateExists = false;
	for (const auto &s : states) {
		if (s.getKey() == toStateKey) {
			toStateExists = true;
			break;
		}
	}
	if (!toStateExists) {
		throw StateNotFoundException(toStateKey);
	}

	// Check if the "from" state exists
	bool fromStateExists = false;
	for (const auto &s : states) {
		if (s.getKey() == fromStateKey) {
			fromStateExists = true;
			break;
		}
	}
	if (!fromStateExists) {
		throw StateNotFoundException(fromStateKey);
	}

	// Check if the stack symbol is in the stack alphabet
	if (stackSymbol != "" && stackAlphabet.find(stackSymbol) == stackAlphabet.end()) {
		throw InvalidTransitionException("Stack symbol '" + stackSymbol + "' is not in the stack alphabet.");
	}

	// Check if the push symbols are in the stack alphabet
	for (char symbol : pushSymbol) {
		std::string symbolStr(1, symbol);
		if (stackAlphabet.find(symbolStr) == stackAlphabet.end()) {
			throw InvalidTransitionException("Push symbol '" + symbolStr + "' is not in the stack alphabet.");
		}
	}

	PDAState *fromState = getState(fromStateKey);

	// Check if the transition already exists
	for (auto &transition : fromState->getTransitions()) {
		if (transition.getInput() == input && transition.getToStateKey() == toStateKey &&
		    transition.getStackSymbol() == stackSymbol) {
			throw InvalidTransitionException("Transition already exists: (" + fromStateKey + ", " + input + ", " +
			                                 stackSymbol + ") -> " + toStateKey + ", " + pushSymbol);
		}
	}
}

void PushdownAutomaton::setCurrentState(const std::string &state) {
	getState(state);
	currentState = state;
}

std::string PushdownAutomaton::getCurrentState() const {
	if (currentState.empty()) {
		return getStartState();
	}
	return currentState;
}

void PushdownAutomaton::addState(const std::string &label) {
	PDAState state(label, false);
	// Check if the state already exists
	for (const auto &existingState : getStates()) {
		if (existingState.getKey() == state.getKey()) {
			throw InvalidAutomatonDefinitionException("State with label " + state.getLabel() + " already exists");
		}
	}

	states.push_back(state);
}

std::vector<PDAState> PushdownAutomaton::getStates() const {
	return states;
}

PDAState *PushdownAutomaton::getState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			return &state;
		}
	}
	throw StateNotFoundException(key);
}

PDAState PushdownAutomaton::getState(const std::string &key) const {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			return state;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomaton::removeState(const std::string &key) {
	for (auto it = states.begin(); it != states.end(); ++it) {
		if (it->getKey() == key) {
			states.erase(it);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomaton::setInputAlphabet(const std::set<std::string> &inputAlphabet) {
	this->inputAlphabet = inputAlphabet;
}

std::set<std::string> PushdownAutomaton::getInputAlphabet() const {
	return inputAlphabet;
}

void PushdownAutomaton::addInputAlphabet(const std::set<std::string> &inputAlphabet) {
	for (const auto &symbol : inputAlphabet) {
		this->inputAlphabet.insert(symbol);
	}
}

void PushdownAutomaton::setStackAlphabet(const std::set<std::string> &stackAlphabet) {
	validateStackAlphabet(stackAlphabet);
	this->stackAlphabet = stackAlphabet;
}

std::set<std::string> PushdownAutomaton::getStackAlphabet() const {
	return stackAlphabet;
}

void PushdownAutomaton::addStackAlphabet(const std::set<std::string> &stackAlphabet) {
	validateStackAlphabet(stackAlphabet);
	for (const auto &symbol : stackAlphabet) {
		this->stackAlphabet.insert(symbol);
	}
}

void PushdownAutomaton::setStartState(const std::string &key) {
	for (const auto &state : states) {
		if (state.getKey() == key) {
			startState = key;
			return;
		}
	}
	if (currentState.empty()) {
		currentState = key;
	}
	throw StateNotFoundException(key);
}

void PushdownAutomaton::addTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                             const std::string &toStateKey, const std::string &stackSymbol,
                                             const std::string &pushSymbol) {
	validateTransition(fromStateKey, input, toStateKey, stackSymbol, pushSymbol);
	PDAState *state = getState(fromStateKey);
	state->addTransitionTo(input, toStateKey, stackSymbol, pushSymbol);
}

void PushdownAutomaton::removeTransitionBetween(const std::string &fromStateKey, const std::string &input,
                                                const std::string &toStateKey, const std::string &stackSymbol,
                                                const std::string &pushSymbol) {
	PDAState *state = getState(fromStateKey);
	state->removeTransitionTo(input, toStateKey, stackSymbol, pushSymbol);
}

void PushdownAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	PDAState *fromState = getState(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
}

void PushdownAutomaton::clearStateTransitions(const std::string &key) {
	PDAState *state = getState(key);
	state->clearTransitions();
}

void PushdownAutomaton::clearTransitions() {
	for (auto &state : states) {
		state.clearTransitions();
	}
}

std::string PushdownAutomaton::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void PushdownAutomaton::addAcceptState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			state.setIsAccept(true);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomaton::removeAcceptState(const std::string &key) {
	for (auto &state : states) {
		if (state.getKey() == key) {
			state.setIsAccept(false);
			return;
		}
	}
	throw StateNotFoundException(key);
}

void PushdownAutomaton::clearAcceptStates() {
	for (auto &state : states) {
		state.setIsAccept(false);
	}
}

std::vector<PDAState> PushdownAutomaton::getAcceptStates() const {
	std::vector<PDAState> acceptStates;
	for (const auto &state : states) {
		if (state.getIsAccept()) {
			acceptStates.push_back(state);
		}
	}
	return acceptStates;
}

void PushdownAutomaton::reset() {
	currentState = startState;
	stack = std::stack<std::string>();
	stack.push(INITIAL_STACK_SYMBOL);
}

void PushdownAutomaton::applyEpsilonClosure(PDAState *&currentState, std::stack<std::string> &simulationStack) {
	std::queue<std::pair<PDAState *, std::stack<std::string>>> stateQueue;

	// Initialize queue with the current state and stack
	stateQueue.push({currentState, simulationStack});

	while (!stateQueue.empty()) {
		PDAState *current = stateQueue.front().first;
		std::stack<std::string> currentStack = stateQueue.front().second;
		stateQueue.pop();

		// Iterate over all transitions from the current state
		for (const auto &transition : current->getTransitions()) {
			std::string stackTop = currentStack.empty() ? "" : currentStack.top();

			// Ensure valid comparison between stack symbols
			std::string transitionStackSymbol = transition.getStackSymbol();

			// Check for epsilon transition and stack symbol match
			if (transition.getInput().empty() && transitionStackSymbol == stackTop) {
				PDAState *nextState = getState(transition.getToStateKey());
				if (!nextState)
					continue; // Avoid null pointer access

				// Update the stack for the next state (copy)
				std::stack<std::string> nextStack = currentStack;

				// Pop the stack if symbols match
				if (!nextStack.empty() && transitionStackSymbol == stackTop) {
					nextStack.pop();
				}

				// Push symbols onto the stack in reverse order
				std::string pushSymbols = transition.getPushSymbol();
				for (auto it = pushSymbols.rbegin(); it != pushSymbols.rend(); ++it) {
					nextStack.push(std::string(1, *it));
				}

				// Avoid infinite loops by only pushing unique states
				if (nextState != current) {
					stateQueue.push({nextState, nextStack});
				}

				// Update the main references
				currentState = nextState;
				simulationStack = nextStack;
			}
		}
	}
}


void PushdownAutomaton::addEpsilonClosure(std::unordered_set<std::string> &states, std::stack<std::string> stack) {
	std::queue<std::pair<std::string, std::stack<std::string>>> stateQueue;

	// Initialize queue with all current states and their stacks
	for (const auto &state : states) {
		stateQueue.push({state, stack}); // Use a copy of the current stack
	}

	// Process queue to find all epsilon transitions
	while (!stateQueue.empty()) {
		auto currentPair = stateQueue.front(); // Get the pair
		stateQueue.pop();

		std::string currentStateKey = currentPair.first;           // Extract the state
		std::stack<std::string> currentStack = currentPair.second; // Extract the stack

		PDAState *state = getState(currentStateKey);

		// Find all epsilon transitions from this state
		for (const auto &transition : state->getTransitions()) {
			// Check if the transition is an epsilon transition
			if (transition.getInput() == "") {
				std::string stackTop = currentStack.empty() ? "" : currentStack.top();

				// Check if the stack symbol matches the transition's stack symbol
				if (transition.getStackSymbol() == stackTop) {
					std::string nextStateKey = transition.getToStateKey();

					// If this state hasn't been seen before, add it
					if (states.find(nextStateKey) == states.end()) {
						states.insert(nextStateKey);

						// Update the stack for the next state
						std::stack<std::string> nextStack = currentStack; // Copy the current stack
						if (!transition.getPushSymbol().empty()) {
							if (!nextStack.empty() && transition.getStackSymbol() == stackTop) {
								nextStack.pop(); // Pop the stack top if it matches
							}
							for (auto it = transition.getPushSymbol().rbegin(); it != transition.getPushSymbol().rend();
							     ++it) {
								nextStack.push(std::string(1, *it)); // Push symbols onto the stack in reverse order
							}
						}

						// Add the next state and its updated stack to the queue
						stateQueue.push({nextStateKey, nextStack});
					}
				}
			}
		}
	}
}