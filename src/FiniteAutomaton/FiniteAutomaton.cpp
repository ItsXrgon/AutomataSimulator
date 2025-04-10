#define AUTOMATASIMULATOR_EXPORTS
#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton()
    : inputHead(0), startState(""), inputAlphabetCacheInvalidated(false), statesCacheInvalidated(false) {}

FiniteAutomaton::~FiniteAutomaton() {}

void FiniteAutomaton::validateTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                         const std::string &input) {
	if (!stateExists(fromStateKey)) {
		throw StateNotFoundException(fromStateKey);
	}
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}
	FAState *fromState = getStateInternal(fromStateKey);
	std::string transitionKey = FATransition::generateTransitionKey(fromStateKey, toStateKey, input);

	// Check if the new transition would be a duplicate
	if (fromState->transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + input + " -> " +
		                                 toStateKey);
	}
}

FAState *FiniteAutomaton::getStateInternal(const std::string &key) {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return &(it->second);
}

bool FiniteAutomaton::stateExists(const std::string &key) const {
	return states.find(key) != states.end();
}

bool FiniteAutomaton::inputAlphabetSymbolExists(const std::string &symbol) const {
	return inputAlphabet.find(symbol) != inputAlphabet.end();
}

std::vector<std::string> FiniteAutomaton::getInput() const {
	return input;
}

void FiniteAutomaton::setInput(const std::vector<std::string> &input) {
	this->input = input;
	this->inputHead = 0;
}

void FiniteAutomaton::addInput(const std::vector<std::string> &input) {
	this->input.insert(this->input.end(), input.begin(), input.end());
}

int FiniteAutomaton::getInputHead() const {
	return inputHead;
}

void FiniteAutomaton::setInputHead(const int &inputHead) {
	if (inputHead < 0) {
		this->inputHead = 0;
	} else if (inputHead >= input.size()) {
		this->inputHead = input.size() - 1;
	} else {
		this->inputHead = inputHead;
	}
}

void FiniteAutomaton::addState(const std::string &label, const bool &isAccept) {
	// Check if state label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	FAState state(label, isAccept);

	// Update the vector and invalidate conversion cache
	states[state.getKey()] = state;
	statesCacheInvalidated = true;
}

void FiniteAutomaton::updateStateLabel(const std::string &key, const std::string &label) {
	// Check if state with new label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	FAState *state = getStateInternal(key);

	state->setLabel(label);

	if (key == startState) {
		startState = label;
	}

	states[state->getKey()] = *state;
	states.erase(key);

	statesCacheInvalidated = true;
}

std::string FiniteAutomaton::getCurrentState() const {
	// If there is no current state and no start state to fallback to then throw error
	if (currentState.empty() && startState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state and start state are not set");
	}

	// Fallback to start state if no current state is set
	if (currentState.empty()) {
		return startState;
	}

	return currentState;
}

void FiniteAutomaton::setCurrentState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	currentState = key;
}

FAState FiniteAutomaton::getState(const std::string &key) const {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return it->second;
}

std::vector<FAState> FiniteAutomaton::getStates() {
	// if the conversion cache from unordered_map to vector is not valid then recompute
	if (statesCacheInvalidated) {
		cachedStates.clear();
		for (const auto &pair : states) {
			cachedStates.push_back(pair.second);
		}
		statesCacheInvalidated = false;
	}
	return cachedStates;
}

void FiniteAutomaton::removeState(const std::string &key, const bool &strict) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<FATransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			if (transition.getToStateKey() == key) {
				if (strict) {
					conflictingTransitions.push_back(transition.getKey());
				} else {
					pair.second.removeTransition(transition.getKey());
				}
			}
		}
	}

	// If strict mode is enabled and conflicts exist, throw an error
	if (strict && !conflictingTransitions.empty()) {
		std::string conflictMessage = "Cannot remove state " + key + " because it is used in transitions: [";
		for (const auto &t : conflictingTransitions) {
			conflictMessage += t + ", ";
		}
		conflictMessage.erase(conflictMessage.size() - 2, 2); // Remove trailing ", "
		conflictMessage += "]\nIf you wish to delete these transitions, call the function again with strict=false.";

		throw InvalidAutomatonDefinitionException(conflictMessage);
	}

	states.erase(key);
	if (key == startState) {
		startState = "";
	}
	if (key == currentState) {
		currentState = "";
	}
	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeStates(const std::vector<std::string> &keys, const bool &strict) {
	std::vector<std::string> missingStates;
	std::unordered_map<std::string, std::vector<std::string>> conflictingTransitions;

	// Check for missing states
	for (const auto &key : keys) {
		if (!stateExists(key)) {
			missingStates.push_back(key);
		}
	}

	// If missing states are found, throw an exception
	if (!missingStates.empty()) {
		std::string missingStatesString = "[" + missingStates[0];
		for (size_t i = 1; i < missingStates.size(); i++) {
			missingStatesString += ", " + missingStates[i];
		}
		missingStatesString += "]";
		throw StateNotFoundException("States not found: " + missingStatesString);
	}

	// Check for conflicting transitions
	for (const auto &key : keys) {
		for (auto &pair : states) {
			std::vector<FATransition> transitions = pair.second.getTransitions();
			for (const auto &transition : transitions) {
				if (transition.getToStateKey() == key) {
					if (strict) {
						conflictingTransitions[key].push_back(transition.getKey());
					} else {
						pair.second.removeTransition(transition.getKey());
					}
				}
			}
		}
	}

	// If strict mode is enabled and conflicts exist, throw an error
	if (strict && !conflictingTransitions.empty()) {
		std::string conflictMessage = "Cannot remove states because they are used in transitions:\n";
		for (const auto &[state, transitions] : conflictingTransitions) {
			conflictMessage += "State " + state + " is part of transitions: [";
			for (const auto &t : transitions) {
				conflictMessage += t + ", ";
			}
			conflictMessage.erase(conflictMessage.size() - 2, 2); // Remove trailing ", "
			conflictMessage += "]\n";
		}
		throw InvalidAutomatonDefinitionException(
		    conflictMessage + " If you wish to delete these transitions, call the function again with strict=false.");
	}

	// Remove states
	for (const auto &key : keys) {
		states.erase(key);
		if (key == startState) {
			startState = "";
		}
		if (key == currentState) {
			currentState = "";
		}
	}

	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearStates() {
	states.clear();
	currentState = "";
	startState = "";
	statesCacheInvalidated = true;
}

void FiniteAutomaton::setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict) {

	std::unordered_map<std::string, std::vector<std::string>> conflictingTransitions;
	std::unordered_set<std::string> newAlphabet =
	    std::unordered_set<std::string>(inputAlphabet.begin(), inputAlphabet.end());

	for (auto &pair : states) {
		std::vector<FATransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			std::string symbol = transition.getInput();
			bool found = newAlphabet.find(symbol) != newAlphabet.end();
			if (!found) {
				if (strict) {
					conflictingTransitions[symbol].push_back(transition.getKey());
				} else {
					pair.second.removeTransition(transition.getKey());
				}
			}
		}
	}

	// If strict mode is enabled and conflicts exist, throw an error
	if (strict && !conflictingTransitions.empty()) {
		std::string conflictMessage =
		    "Setting the alphabet will remove symbols that cannot be removed because they are used in transitions:\n";
		for (const auto &[symbol, transitions] : conflictingTransitions) {
			conflictMessage += "Symbol " + symbol + " is part of transitions: [";
			for (const auto &t : transitions) {
				conflictMessage += t + ", ";
			}
			conflictMessage.erase(conflictMessage.size() - 2, 2); // Remove trailing ", "
			conflictMessage += "]\n";
		}
		throw InvalidAutomatonDefinitionException(
		    conflictMessage + " If you wish to delete these transitions, call the function again with strict=false.");
	}

	this->inputAlphabet = newAlphabet;
	inputAlphabetCacheInvalidated = true;
}

void FiniteAutomaton::addInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	for (const auto &symbol : inputAlphabet) {
		this->inputAlphabet.insert(symbol);
	}
	inputAlphabetCacheInvalidated = true;
}

std::vector<std::string> FiniteAutomaton::getInputAlphabet() {
	// if the conversion cache from unordered_set to vector is not valid then recompute
	if (inputAlphabetCacheInvalidated) {
		cachedInputAlphabet.clear();
		for (const auto &symbol : inputAlphabet) {
			cachedInputAlphabet.push_back(symbol);
		}
		inputAlphabetCacheInvalidated = false;
	}
	return cachedInputAlphabet;
}

void FiniteAutomaton::removeInputAlphabetSymbol(const std::string &symbol, const bool &strict) {
	// Check if symbol exists
	if (!inputAlphabetSymbolExists(symbol)) {
		throw InputAlphabetSymbolNotFoundException(symbol);
	}

	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<FATransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			if (transition.getInput() == symbol) {
				if (strict) {
					conflictingTransitions.push_back(transition.getKey());
				} else {
					pair.second.removeTransition(transition.getKey());
				}
			}
		}
	}

	// If strict mode is enabled and conflicts exist, throw an error
	if (strict && !conflictingTransitions.empty()) {
		std::string conflictMessage = "Cannot remove symbol " + symbol + " because it is used in transitions: [";
		for (const auto &t : conflictingTransitions) {
			conflictMessage += t + ", ";
		}
		conflictMessage.erase(conflictMessage.size() - 2, 2); // Remove trailing ", "
		conflictMessage += "]\nIf you wish to delete these transitions, call the function again with strict=false.";

		throw InvalidAutomatonDefinitionException(conflictMessage);
	}

	inputAlphabet.erase(symbol);
	inputAlphabetCacheInvalidated = true;
}

void FiniteAutomaton::removeInputAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict) {
	// Storing the missing symbols if any for error handling
	std::vector<std::string> missingSymbols;
	std::unordered_map<std::string, std::vector<std::string>> conflictingTransitions;

	// Check which symbols dont exist and add to missingSymbols vector
	for (const auto &symbol : symbols) {
		if (!inputAlphabetSymbolExists(symbol)) {
			missingSymbols.push_back(symbol);
		}
	}

	// If missing symbols are found then throw error after formatting
	if (!missingSymbols.empty()) {
		std::string missingSymbolsString = "[";
		for (const auto &missingSymbol : missingSymbols) {
			missingSymbolsString += missingSymbol + ", ";
		}
		missingSymbolsString.erase(missingSymbolsString.size() - 2, 2);
		throw InputAlphabetSymbolNotFoundException(missingSymbolsString);
	}

	// Check for conflicting transitions
	for (const auto &symbol : symbols) {
		for (auto &pair : states) {
			std::vector<FATransition> transitions = pair.second.getTransitions();
			for (const auto &transition : transitions) {
				if (transition.getInput() == symbol) {
					if (strict) {
						conflictingTransitions[symbol].push_back(transition.getKey());
					} else {
						pair.second.removeTransition(transition.getKey());
					}
				}
			}
		}
	}

	// If strict mode is enabled and conflicts exist, throw an error
	if (strict && !conflictingTransitions.empty()) {
		std::string conflictMessage = "Cannot remove symbols because they are used in transitions:\n";
		for (const auto &[symbol, transitions] : conflictingTransitions) {
			conflictMessage += "Symbol " + symbol + " is part of transitions: [";
			for (const auto &t : transitions) {
				conflictMessage += t + ", ";
			}
			conflictMessage.erase(conflictMessage.size() - 2, 2); // Remove trailing ", "
			conflictMessage += "]\n";
		}
		throw InvalidAutomatonDefinitionException(
		    conflictMessage + " If you wish to delete these transitions, call the function again with strict=false.");
	}

	// If no missing symbols found then we remove
	for (const auto &symbol : symbols) {
		inputAlphabet.erase(symbol);
	}
	inputAlphabetCacheInvalidated = true;
}

void FiniteAutomaton::clearInputAlphabet(const bool &strict) {
	for (auto &pair : states) {
		std::vector<FATransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			if (!transition.getInput().empty()) {
				if (strict) {
					throw InvalidAutomatonDefinitionException(
					    "Cannot clear input alphabet because non-epsilon transitions exist");
				} else {
					pair.second.removeTransition(transition.getKey());
				}
			}
		}
	}

	inputAlphabet.clear();
	inputAlphabetCacheInvalidated = true;
}

std::string FiniteAutomaton::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void FiniteAutomaton::setStartState(const std::string &key) {
	// Check if the state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	startState = key;

	if (currentState.empty()) {
		currentState = key;
	}
}

void FiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                    const std::string &input) {
	validateTransition(fromStateKey, toStateKey, input);
	FAState *state = getStateInternal(fromStateKey);
	state->addTransition(toStateKey, input);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = FATransition::getToStateFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey = FATransition::generateTransitionKey(fromStateKey, toStateKey, input);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate state existence and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input);

	FAState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionInput(transitionKey, input);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::updateTransitionFromState(const std::string &transitionKey, const std::string &fromStateKey) {
	std::string input = FATransition::getInputFromKey(transitionKey);
	std::string oldFromStateKey = FATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = FATransition::getToStateFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey = FATransition::generateTransitionKey(fromStateKey, toStateKey, input);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate state existence and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input);

	FAState *oldFromState = getStateInternal(oldFromStateKey);
	// Check if the old transition exists
	if (!oldFromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	FAState *newFromState = getStateInternal(fromStateKey);

	// Remove the transition from the old from state
	oldFromState->removeTransition(transitionKey);
	// Add the transition to the new from state
	newFromState->addTransition(toStateKey, input);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::updateTransitionToState(const std::string &transitionKey, const std::string &toStateKey) {
	std::string input = FATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);

	std::string newTransitionKey = FATransition::generateTransitionKey(fromStateKey, toStateKey, input);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate state existence and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input);

	FAState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeTransition(const std::string &transitionKey) {
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);

	FAState *fromState = getStateInternal(fromStateKey);
	fromState->removeTransition(transitionKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	FAState *fromState = getStateInternal(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearStateTransitions(const std::string &stateKey) {
	FAState *state = getStateInternal(stateKey);
	state->clearTransitions();
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearTransitions() {
	for (auto &pair : states) {
		pair.second.clearTransitions();
	}
	statesCacheInvalidated = true;
}

void FiniteAutomaton::addAcceptState(const std::string &stateKey) {
	FAState *state = getStateInternal(stateKey);

	state->setIsAccept(true);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::addAcceptStates(const std::vector<std::string> &keys) {
	// Storing the missing symbols if any for error handling
	std::vector<std::string> missingStates;

	for (const auto &key : keys) {
		if (!stateExists(key)) {
			missingStates.push_back(key);
		}
	}

	if (!missingStates.empty()) {
		std::string missingStatesString = "[";
		for (const auto &missingState : missingStates) {
			missingStatesString += missingState + ", ";
		}
		missingStatesString.erase(missingStatesString.size() - 2, 2);
		throw StateNotFoundException(missingStatesString);
	}

	for (const auto &key : keys) {
		FAState *state = getStateInternal(key);
		state->setIsAccept(true);
	}

	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeAcceptState(const std::string &stateKey) {
	FAState *state = getStateInternal(stateKey);

	state->setIsAccept(false);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeAcceptStates(const std::vector<std::string> &keys) {
	// Storing the missing symbols if any for error handling
	std::vector<std::string> missingStates;

	for (const auto &key : keys) {
		if (!stateExists(key)) {
			missingStates.push_back(key);
		}
	}

	if (!missingStates.empty()) {
		std::string missingStatesString = "[";
		for (const auto &missingState : missingStates) {
			missingStatesString += missingState + ", ";
		}
		missingStatesString.erase(missingStatesString.size() - 2, 2);
		throw StateNotFoundException(missingStatesString);
	}

	for (const auto &key : keys) {
		FAState *state = getStateInternal(key);
		state->setIsAccept(false);
	}

	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearAcceptStates() {
	for (auto &pair : states) {
		pair.second.setIsAccept(false);
	}
	statesCacheInvalidated = true;
}

std::vector<FAState> FiniteAutomaton::getAcceptStates() const {
	std::vector<FAState> acceptStates;
	for (auto &pair : states) {
		if (pair.second.getIsAccept()) {
			acceptStates.push_back(pair.second);
		}
	}
	return acceptStates;
}

void FiniteAutomaton::reset() {
	currentState = startState;
	inputHead = 0;
}

bool FiniteAutomaton::isAccepting() const {
	return getState(currentState).getIsAccept();
}