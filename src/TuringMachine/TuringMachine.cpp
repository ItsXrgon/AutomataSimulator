#include "TuringMachine.h"

const std::string &DEFAULT_BLANK_SYMBOL = "_";

TuringMachine::TuringMachine(std::string blankSymbol)
    : startState(""), inputAlphabetCacheInvalidated(false), tapeAlphabetCacheInvalidated(false),
      statesCacheInvalidated(false) {
	tape = TMTape(blankSymbol);
}

TuringMachine::~TuringMachine() {}

void TuringMachine::validateTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                       const std::string &readSymbol, const std::string &writeSymbol,
                                       TMDirection direction) {
	if (!stateExists(fromStateKey)) {
		throw StateNotFoundException(fromStateKey);
	}
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}
	if (!readSymbol.empty() && readSymbol != tape.getBlankSymbol() && !tapeAlphabetSymbolExists(readSymbol)) {
		throw InvalidTransitionException("Read symbol not in tape alphabet: " + readSymbol);
	}
	if (!writeSymbol.empty() && writeSymbol != tape.getBlankSymbol() && !tapeAlphabetSymbolExists(writeSymbol)) {
		throw InvalidTransitionException("Write not in tape alphabet: " + writeSymbol);
	}

	TMState *fromState = getStateInternal(fromStateKey);
	const std::string &transitionKey =
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Check if the new transition would be a duplicate
	if (fromState->transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                 " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                 " | direction: " + TMDirectionHelper::toString(direction));
	}
}

TMState *TuringMachine::getStateInternal(const std::string &key) {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return &(it->second);
}

std::vector<std::string> TuringMachine::getInput() const {
	return input;
}

void TuringMachine::setInput(const std::vector<std::string> &input) {
	std::unordered_set<std::string> missingSymbols;

	for (const auto &symbol : input) {
		if (!symbol.empty() && !inputAlphabetSymbolExists(symbol)) {
			missingSymbols.insert(symbol);
		}
	}

	if (!missingSymbols.empty()) {
		std::string missingSymbolsString = "[" + *missingSymbols.begin();
		for (const auto &symbol : missingSymbols) {
			missingSymbolsString += ", " + symbol;
		}
		missingSymbolsString += "]";
		throw InputAlphabetSymbolNotFoundException(missingSymbolsString);
	}

	this->input = input;
	this->tape.loadInput(this->input);
}

void TuringMachine::addInput(const std::vector<std::string> &input) {
	std::unordered_set<std::string> missingSymbols;

	for (const auto &symbol : input) {
		if (!symbol.empty() && !inputAlphabetSymbolExists(symbol)) {
			missingSymbols.insert(symbol);
		}
	}

	if (!missingSymbols.empty()) {
		std::string missingSymbolsString = "[" + *missingSymbols.begin();
		for (const auto &symbol : missingSymbols) {
			missingSymbolsString += ", " + symbol;
		}
		missingSymbolsString += "]";
		throw InputAlphabetSymbolNotFoundException(missingSymbolsString);
	}

	this->input.insert(this->input.end(), input.begin(), input.end());
	this->tape.loadInput(this->input);
}

std::map<int, std::string> TuringMachine::getTape() const {
	return tape.getTape();
}

void TuringMachine::setTape(std::map<int, std::string> &tape) {
	this->tape.setTape(tape);
}

bool TuringMachine::stateExists(const std::string &key) const {
	return states.find(key) != states.end();
}

bool TuringMachine::inputAlphabetSymbolExists(const std::string &symbol) const {
	return inputAlphabet.find(symbol) != inputAlphabet.end();
}

bool TuringMachine::tapeAlphabetSymbolExists(const std::string &symbol) const {
	return tapeAlphabet.find(symbol) != tapeAlphabet.end();
}

void TuringMachine::addState(const std::string &label, const bool &isAccept) {
	// Check if state label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	TMState state(label, isAccept);

	// Update the vector and invalidate conversion cache
	states[state.getKey()] = state;
	statesCacheInvalidated = true;
}

void TuringMachine::updateStateLabel(const std::string &key, const std::string &label) {
	// Check if state with new label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	TMState *state = getStateInternal(key);

	if (key == startState) {
		startState = label;
	}

	state->setLabel(label);

	states[state->getKey()] = *state;
	states.erase(key);

	statesCacheInvalidated = true;
}

std::string TuringMachine::getCurrentState() const {
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

void TuringMachine::setCurrentState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	currentState = key;
}

TMState TuringMachine::getState(const std::string &key) const {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return it->second;
}

std::vector<TMState> TuringMachine::getStates() {
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

void TuringMachine::removeState(const std::string &key, const bool &strict) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<TMTransition> transitions = pair.second.getTransitions();
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

void TuringMachine::removeStates(const std::vector<std::string> &keys, const bool &strict) {
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
			std::vector<TMTransition> transitions = pair.second.getTransitions();
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

void TuringMachine::clearStates() {
	states.clear();
	currentState = "";
	startState = "";
	statesCacheInvalidated = true;
}

void TuringMachine::setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict) {
	std::unordered_map<std::string, std::vector<std::string>> conflictingTransitions;
	std::unordered_set<std::string> newAlphabet =
	    std::unordered_set<std::string>(inputAlphabet.begin(), inputAlphabet.end());

	for (auto &pair : states) {
		std::vector<TMTransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			std::string symbol = transition.getReadSymbol();
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

	addTapeAlphabet(inputAlphabet);
	this->inputAlphabet = newAlphabet;
	inputAlphabetCacheInvalidated = true;
}

void TuringMachine::addInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	for (const auto &symbol : inputAlphabet) {
		this->inputAlphabet.insert(symbol);
	}
	addTapeAlphabet(inputAlphabet);
	inputAlphabetCacheInvalidated = true;
}

std::vector<std::string> TuringMachine::getInputAlphabet() {
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

void TuringMachine::removeInputAlphabetSymbol(const std::string &symbol, const bool &strict) {
	// Check if symbol exists
	if (!inputAlphabetSymbolExists(symbol)) {
		throw InputAlphabetSymbolNotFoundException(symbol);
	}
	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<TMTransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			if (transition.getReadSymbol() == symbol) {
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

void TuringMachine::removeInputAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict) {
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
			std::vector<TMTransition> transitions = pair.second.getTransitions();
			for (const auto &transition : transitions) {
				if (transition.getReadSymbol() == symbol) {
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

void TuringMachine::clearInputAlphabet(const bool &strict) {
	for (auto &pair : states) {
		std::vector<TMTransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			if (!transition.getReadSymbol().empty()) {
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
	inputAlphabetCacheInvalidated = false;
}

void TuringMachine::setTapeAlphabet(const std::vector<std::string> &tapeAlphabet, const bool &strict) {
	this->tapeAlphabet = std::unordered_set<std::string>(tapeAlphabet.begin(), tapeAlphabet.end());

	// remove elements in new tape alphabet but not input alphabet
	std::unordered_set<std::string> missingSymbols;
	for (auto &inputSymbol : inputAlphabet) {
		if (!tapeAlphabetSymbolExists(inputSymbol)) {
			missingSymbols.insert(inputSymbol);
		}
	}
	for (auto &symbol : missingSymbols) {
		removeInputAlphabetSymbol(symbol);
	} 

	tapeAlphabetCacheInvalidated = true;
}

void TuringMachine::addTapeAlphabet(const std::vector<std::string> &tapeAlphabet) {
	for (const auto &symbol : tapeAlphabet) {
		this->tapeAlphabet.insert(symbol);
	}
	tapeAlphabetCacheInvalidated = true;
}

std::vector<std::string> TuringMachine::getTapeAlphabet() {
	// if the conversion cache from unordered_set to vector is not valid then recompute
	if (tapeAlphabetCacheInvalidated) {
		cachedTapeAlphabet.clear();
		for (const auto &symbol : tapeAlphabet) {
			cachedTapeAlphabet.push_back(symbol);
		}
		tapeAlphabetCacheInvalidated = true;
	}
	return cachedTapeAlphabet;
}

void TuringMachine::removeTapeAlphabetSymbol(const std::string &symbol, const bool &strict) {
	// Check if symbol exists
	if (!tapeAlphabetSymbolExists(symbol)) {
		throw TapeAlphabetSymbolNotFoundException(symbol);
	}

	if (inputAlphabetSymbolExists(symbol)) {
		removeInputAlphabetSymbol(symbol);
	}

	tapeAlphabet.erase(symbol);
	tapeAlphabetCacheInvalidated = true;
}

void TuringMachine::removeTapeAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict) {
	// Storing the missing symbols if any for error handling
	std::vector<std::string> missingSymbols;

	// Check which symbols dont exist and add to missingSymbols vector
	for (const auto &symbol : symbols) {
		if (!tapeAlphabetSymbolExists(symbol)) {
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
		throw TapeAlphabetSymbolNotFoundException(missingSymbolsString);
	}



	// If no missing symbols found then we remove
	for (const auto &symbol : symbols) {
		tapeAlphabet.erase(symbol);
	}

	// Remove symbols from input alphabet (subset)
	for (auto &symbol : symbols) {
		if (inputAlphabetSymbolExists(symbol)) {
			removeInputAlphabetSymbol(symbol);
		}
	}

	tapeAlphabetCacheInvalidated = true;
}

void TuringMachine::clearTapeAlphabet(const bool &strict) {
	tapeAlphabet.clear();
	tapeAlphabetCacheInvalidated = true;

	clearInputAlphabet(strict);
}

void TuringMachine::setStartState(const std::string &key) {
	// Check if the state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	startState = key;

	if (currentState.empty()) {
		currentState = key;
	}
}

void TuringMachine::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                  const std::string &readSymbol, const std::string &writeSymbol,
                                  TMDirection direction) {
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
	TMState *state = getStateInternal(fromStateKey);
	state->addTransition(toStateKey, readSymbol, writeSymbol, direction);
	statesCacheInvalidated = true;
}

void TuringMachine::updateTransitionFromState(const std::string &transitionKey, const std::string &fromStateKey) {
	std::string oldFromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	TMState *oldFromState = getStateInternal(oldFromStateKey);
	// Check if the old transition exists
	if (!oldFromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	TMState *newFromState = getStateInternal(fromStateKey);

	// Remove the transition from the old from state
	oldFromState->removeTransition(transitionKey);
	// Add the transition to the new from state
	newFromState->addTransition(toStateKey, readSymbol, writeSymbol, direction);
	statesCacheInvalidated = true;
}

void TuringMachine::updateTransitionToState(const std::string &transitionKey, const std::string &toStateKey) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	TMState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void TuringMachine::updateTransitionReadSymbol(const std::string &transitionKey, const std::string &readSymbol) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	TMState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionReadSymbol(transitionKey, readSymbol);
	statesCacheInvalidated = true;
}

void TuringMachine::updateTransitionWriteSymbol(const std::string &transitionKey, const std::string &writeSymbol) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	TMState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionWriteSymbol(transitionKey, writeSymbol);
	statesCacheInvalidated = true;
}

void TuringMachine::updateTransitionDirection(const std::string &transitionKey, const TMDirection &direction) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
	std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
	std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);

	TMState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionDirection(transitionKey, direction);
	statesCacheInvalidated = true;
}

void TuringMachine::removeTransition(const std::string &transitionKey) {
	std::string fromStateKey = TMTransition::getFromStateFromKey(transitionKey);

	TMState *fromState = getStateInternal(fromStateKey);
	fromState->removeTransition(transitionKey);
	statesCacheInvalidated = true;
}

void TuringMachine::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	TMState *fromState = getStateInternal(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
	statesCacheInvalidated = true;
}

void TuringMachine::clearStateTransitions(const std::string &key) {
	TMState *state = getStateInternal(key);
	state->clearTransitions();
	statesCacheInvalidated = true;
}

void TuringMachine::clearTransitions() {
	for (auto &pair : states) {
		pair.second.clearTransitions();
	}
	statesCacheInvalidated = true;
}

std::string TuringMachine::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void TuringMachine::addAcceptState(const std::string &stateKey) {
	TMState *state = getStateInternal(stateKey);

	state->setIsAccept(true);
	statesCacheInvalidated = true;
}

void TuringMachine::addAcceptStates(const std::vector<std::string> &keys) {
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
		TMState *state = getStateInternal(key);
		state->setIsAccept(true);
	}

	statesCacheInvalidated = true;
}

void TuringMachine::removeAcceptState(const std::string &stateKey) {
	TMState *state = getStateInternal(stateKey);

	state->setIsAccept(false);
	statesCacheInvalidated = true;
}

void TuringMachine::removeAcceptStates(const std::vector<std::string> &keys) {
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
		TMState *state = getStateInternal(key);
		state->setIsAccept(false);
	}

	statesCacheInvalidated = true;
}

void TuringMachine::clearAcceptStates() {
	for (auto &pair : states) {
		pair.second.setIsAccept(false);
	}
	statesCacheInvalidated = true;
}

std::vector<TMState> TuringMachine::getAcceptStates() const {
	std::vector<TMState> acceptStates;
	for (auto &pair : states) {
		if (pair.second.getIsAccept()) {
			acceptStates.push_back(pair.second);
		}
	}
	return acceptStates;
}

void TuringMachine::reset() {
	currentState = startState;
	tape.reset();
}

bool TuringMachine::isAccepting() const {
	return getState(currentState).getIsAccept();
}