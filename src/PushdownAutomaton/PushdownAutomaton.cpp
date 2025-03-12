#define AUTOMATASIMULATOR_EXPORTS
#include "PushdownAutomaton.h"
#include "PushdownAutomatonException.h"
#include <unordered_map>

const int DEFAULT_SIMULATION_DEPTH = 50;
const std::string INITIAL_STACK_SYMBOL = "Z";

PushdownAutomaton::PushdownAutomaton() : startState("") {
	stack.push(INITIAL_STACK_SYMBOL);
}

PushdownAutomaton::~PushdownAutomaton() {}

bool PushdownAutomaton::stateExists(const std::string &key) const {
	return states.find(key) != states.end();
}

bool PushdownAutomaton::inputAlphabetSymbolExists(const std::string &symbol) const {
	return inputAlphabet.find(symbol) != inputAlphabet.end();
}

bool PushdownAutomaton::stackAlphabetSymbolExists(const std::string &symbol) const {
	return stackAlphabet.find(symbol) != stackAlphabet.end();
}

void PushdownAutomaton::validateTransition(const std::string &fromStateKey, const std::string &toStateKey) {
	if (!stateExists(fromStateKey)) {
		throw StateNotFoundException(fromStateKey);
	}
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}
}

void PushdownAutomaton::addState(const std::string &label) {
	// Check if state label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	PDAState state(label, false);

	// Update the vector and invalidate conversion cache
	states[state.getKey()] = state;
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateStateLabel(const std::string &key, const std::string &label) {
	// Check if state with new label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	PDAState *state = getState(key);

	state->setLabel(label);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::setCurrentState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	currentState = key;
}

std::string PushdownAutomaton::getCurrentState() const {
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

std::vector<PDAState> PushdownAutomaton::getStates() {
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

PDAState *PushdownAutomaton::getState(const std::string &key) {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return &(it->second);
}

PDAState PushdownAutomaton::getState(const std::string &key) const {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return it->second;
}

void PushdownAutomaton::removeState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}
	states.erase(key);
	statesCacheInvalidated = false;
}

void PushdownAutomaton::removeStates(const std::vector<std::string> &keys) {
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
	statesCacheInvalidated = false;
}

void PushdownAutomaton::clearStates() {
	states.clear();
	statesCacheInvalidated = false;
}

void PushdownAutomaton::setInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	this->inputAlphabet = std::unordered_set<std::string>(inputAlphabet.begin(), inputAlphabet.end());
	inputAlphabetCacheInvalidated = false;
}

void PushdownAutomaton::addInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	for (const auto &symbol : inputAlphabet) {
		this->inputAlphabet.insert(symbol);
	}
	inputAlphabetCacheInvalidated = false;
}

std::vector<std::string> PushdownAutomaton::getInputAlphabet() {
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

void PushdownAutomaton::removeInputAlphabetSymbol(const std::string &symbol) {
	// Check if symbol exists
	if (!inputAlphabetSymbolExists(symbol)) {
		throw InputAlphabetSymbolNotFoundException(symbol);
	}

	inputAlphabet.erase(symbol);
	inputAlphabetCacheInvalidated = false;
}

void PushdownAutomaton::removeInputAlphabetSymbols(const std::vector<std::string> &symbols) {
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
	inputAlphabetCacheInvalidated = false;
}

void PushdownAutomaton::clearInputAlphabet() {
	inputAlphabet.clear();
	inputAlphabetCacheInvalidated = false;
}

void PushdownAutomaton::setStackAlphabet(const std::vector<std::string> &stackAlphabet) {
	this->stackAlphabet = std::unordered_set<std::string>(stackAlphabet.begin(), stackAlphabet.end());
}

void PushdownAutomaton::addStackAlphabet(const std::vector<std::string> &stackAlphabet) {
	for (const auto &symbol : stackAlphabet) {
		this->stackAlphabet.insert(symbol);
	}
	stackAlphabetCacheInvalidated = false;
}

std::vector<std::string> PushdownAutomaton::getStackAlphabet() {
	// if the conversion cache from unordered_set to vector is not valid then recompute
	if (stackAlphabetCacheInvalidated) {
		cachedStackAlphabet.clear();
		for (const auto &symbol : stackAlphabet) {
			cachedStackAlphabet.push_back(symbol);
		}
		stackAlphabetCacheInvalidated = false;
	}
	return cachedStackAlphabet;
}

void PushdownAutomaton::removeStackAlphabetSymbol(const std::string &symbol) {
	// Check if symbol exists
	if (!stackAlphabetSymbolExists(symbol)) {
		throw InputAlphabetSymbolNotFoundException(symbol);
	}

	stackAlphabet.erase(symbol);
	stackAlphabetCacheInvalidated = true;
}

void PushdownAutomaton::removeStackAlphabetSymbols(const std::vector<std::string> &symbols) {
	// Storing the missing symbols if any for error handling
	std::vector<std::string> missingSymbols;

	// Check which symbols dont exist and add to missingSymbols vector
	for (const auto &symbol : symbols) {
		if (!stackAlphabetSymbolExists(symbol)) {
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
		stackAlphabet.erase(symbol);
	}
	stackAlphabetCacheInvalidated = true;
}

void PushdownAutomaton::clearStackAlphabet() {
	stackAlphabet.clear();
	stackAlphabetCacheInvalidated = true;
}

void PushdownAutomaton::setStartState(const std::string &key) {
	// Check if the state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	startState = key;

	if (currentState.empty()) {
		currentState = key;
	}
}

void PushdownAutomaton::addTransition(const std::string &fromStateKey, const std::string &input,
                                      const std::string &toStateKey, const std::string &stackSymbol,
                                      const std::string &pushSymbol) {
	validateTransition(fromStateKey, toStateKey);

	// Check if the input is in the alphabet if not epsilon
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	PDAState *state = getState(fromStateKey);
	state->addTransitionTo(input, toStateKey, stackSymbol, pushSymbol);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
	// Check if the input is in the alphabet if not epsilon
	if (input != "" && inputAlphabet.find(input) == inputAlphabet.end()) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}
	
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Check if the transition key is valid or not by checking if states exist
	if (!stateExists(fromStateKey) || !stateExists(toStateKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	PDAState *fromState = getState(fromStateKey);

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

void PushdownAutomaton::updateTransitionFromState(const std::string &transitionKey, const std::string &fromStateKey) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string oldFromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	PDAState *newFromState = getState(fromStateKey);

	// Check if the new transition would be a duplicate
	if (newFromState->transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + input + " -> " +
		                                 toStateKey);
	}

	PDAState *oldFromState = getState(oldFromStateKey);

	// Remove the transition from the old from state
	oldFromState->removeTransition(transitionKey);
	// Add the transition to the new from state
	newFromState->addTransitionTo(input, toStateKey, stackSymbol, pushSymbol);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionToState(const std::string &transitionKey, const std::string &toStateKey) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);

	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Check if new to state exists
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}

	PDAState *fromState = getState(toStateKey);

	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionStackSymbol(const std::string &transitionKey, const std::string &stackSymbol) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);

	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Check if new to state exists
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}

	PDAState *fromState = getState(toStateKey);

	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionPushSymbol(const std::string &transitionKey, const std::string &pushSymbol) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);

	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Check if new to state exists
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}

	PDAState *fromState = getState(toStateKey);
	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::removeTransition(const std::string &transitionKey) {
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);

	PDAState *fromState = getState(fromStateKey);
	fromState->removeTransition(transitionKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	PDAState *fromState = getState(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearStateTransitions(const std::string &key) {
	PDAState *state = getState(key);
	state->clearTransitions();
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearTransitions() {
	for (auto &pair : states) {
		pair.second.clearTransitions();
	}
	statesCacheInvalidated = true;
}

std::string PushdownAutomaton::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
}

void PushdownAutomaton::addAcceptState(const std::string &stateKey) {
	PDAState *state = getState(stateKey);

	state->setIsAccept(true);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::removeAcceptState(const std::string &stateKey) {
	PDAState *state = getState(stateKey);

	state->setIsAccept(false);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearAcceptStates() {
	for (auto &pair : states) {
		pair.second.setIsAccept(false);
	}
	statesCacheInvalidated = true;
}

std::vector<PDAState> PushdownAutomaton::getAcceptStates() const {
	std::vector<PDAState> acceptStates;
	for (auto &pair : states) {
		if (pair.second.getIsAccept()) {
			acceptStates.push_back(pair.second);
		}
	}
	return acceptStates;
}

void PushdownAutomaton::reset() {
	currentState = startState;
	stack = std::stack<std::string>();
	stack.push(INITIAL_STACK_SYMBOL);
}