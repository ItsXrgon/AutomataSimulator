#define AUTOMATASIMULATOR_EXPORTS
#include "FiniteAutomaton.h"
#include "FiniteAutomatonException.h"

const int DEFAULT_SIMULATION_DEPTH = 50;

FiniteAutomaton::FiniteAutomaton() : startState("") {}

FiniteAutomaton::~FiniteAutomaton() {}

void FiniteAutomaton::validateTransition(const std::string &fromStateKey, const std::string &input,
                                         const std::string &toStateKey) const {
	if (!stateExists(fromStateKey)) {
		throw StateNotFoundException(fromStateKey);
	}
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}
}

bool FiniteAutomaton::stateExists(const std::string &key) const {
	return states.find(key) != states.end();
}

bool FiniteAutomaton::inputAlphabetSymbolExists(const std::string &symbol) const {
	return inputAlphabet.find(symbol) != inputAlphabet.end();
}

void FiniteAutomaton::addState(const std::string &label) {
	// Check if state label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	FAState state(label, false);

	// Update the vector and invalidate conversion cache
	states[state.getKey()] = state;
	statesCacheInvalidated = true;
}

void FiniteAutomaton::updateStateLabel(const std::string &key, const std::string &label) {
	// Check if state with new label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	FAState *state = getState(key);

	state->setLabel(label);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::setCurrentState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	currentState = key;
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

FAState *FiniteAutomaton::getState(const std::string &key) {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return &(it->second);
}

FAState FiniteAutomaton::getState(const std::string &key) const {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return it->second;
}

void FiniteAutomaton::removeState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}
	states.erase(key);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeStates(const std::vector<std::string> &keys) {
	// Storing the missing states if any for error handling
	std::vector<std::string> missingStates;

	// Check which states dont exist and add to missingStates vector
	for (const auto &key : keys) {
		if (!stateExists(key)) {
			missingStates.push_back(key);
		}
	}

	// If missing states are found then throw error after formatting
	if (!missingStates.empty()) {
		std::string missingStatesString = "[";
		for (const auto &missingState : missingStates) {
			missingStatesString += missingState + ", ";
		}
		missingStatesString.erase(missingStatesString.size() - 2, 2);
		throw StateNotFoundException(missingStatesString);
	}

	// If no missing states found then we remove
	for (const auto &key : keys) {
		states.erase(key);
	}
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearStates() {
	states.clear();
	statesCacheInvalidated = true;
}

void FiniteAutomaton::setInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	this->inputAlphabet = std::unordered_set<std::string>(inputAlphabet.begin(), inputAlphabet.end());
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

void FiniteAutomaton::removeInputAlphabetSymbol(const std::string &symbol) {
	// Check if symbol exists
	if (!inputAlphabetSymbolExists(symbol)) {
		throw InputAlphabetSymbolNotFoundException(symbol);
	}

	inputAlphabet.erase(symbol);
	inputAlphabetCacheInvalidated = true;
}

void FiniteAutomaton::removeInputAlphabetSymbols(const std::vector<std::string> &symbols) {
	// Storing the missing symbols if any for error handling
	std::vector<std::string> missingSymbols;

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

	// If no missing symbols found then we remove
	for (const auto &symbol : symbols) {
		inputAlphabet.erase(symbol);
	}
	inputAlphabetCacheInvalidated = true;
}

void FiniteAutomaton::clearInputAlphabet() {
	inputAlphabet.clear();
	inputAlphabetCacheInvalidated = true;
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

void FiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &input,
                                    const std::string &toStateKey) {
	validateTransition(fromStateKey, input, toStateKey);
	FAState *state = getState(fromStateKey);
	state->addTransitionTo(input, toStateKey);
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

	// Check if the transition key is valid or not by checking if states exist
	if (!stateExists(fromStateKey) || !stateExists(toStateKey)) {
		throw TransitionNotFoundException(transitionKey);
	}


	FAState *fromState = getState(fromStateKey);

	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	// Check if the new transition would be a duplicate
	if (fromState->transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + input + " -> " +
		                                 toStateKey);
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

	FAState *newFromState = getState(fromStateKey);

	// Check if the new transition would be a duplicate
	if (newFromState->transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + input + " -> " +
		                                 toStateKey);
	}

	FAState *oldFromState = getState(oldFromStateKey);

	// Remove the transition from the old from state
	oldFromState->removeTransition(input);
	// Add the transition to the new from state
	newFromState->addTransitionTo(input, toStateKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::updateTransitionToState(const std::string& transitionKey, const std::string& toStateKey) {
	std::string input = FATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);

	std::string newTransitionKey = FATransition::generateTransitionKey(fromStateKey, toStateKey, input);
	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Check if new to state exists
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}

	FAState *fromState = getState(toStateKey);

	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeTransition(const std::string& transitionKey) {
	std::string fromStateKey = FATransition::getFromStateFromKey(transitionKey);
	
	FAState *fromState = getState(fromStateKey);
	fromState->removeTransition(transitionKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	FAState *fromState = getState(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearStateTransitions(const std::string &stateKey) {
	FAState *state = getState(stateKey);
	state->clearTransitions();
	statesCacheInvalidated = true;
}

void FiniteAutomaton::clearTransitions() {
	for (auto &pair : states) {
		pair.second.clearTransitions();
	}
	statesCacheInvalidated = true;
}

std::string FiniteAutomaton::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void FiniteAutomaton::addAcceptState(const std::string &stateKey) {
	FAState *state = getState(stateKey);

	state->setIsAccept(true);
	statesCacheInvalidated = true;
}

void FiniteAutomaton::removeAcceptState(const std::string &stateKey) {
	FAState *state = getState(stateKey);

	state->setIsAccept(false);
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
}