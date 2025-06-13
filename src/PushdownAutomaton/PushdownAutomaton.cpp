#include "AutomataSimulator/PushdownAutomaton.h"

const std::string INITIAL_STACK_SYMBOL = "Z";

PushdownAutomaton::PushdownAutomaton()
    : inputHead(0), startState(""), inputAlphabetCacheInvalidated(false), stackAlphabetCacheInvalidated(false),
      statesCacheInvalidated(false) {
	stack.push(INITIAL_STACK_SYMBOL);
}

PushdownAutomaton::~PushdownAutomaton() {}

std::vector<std::string> PushdownAutomaton::parsePushSymbols(const std::string &pushSymbols) {
	std::vector<std::string> symbols;
	char delimiter = ',';

	if (pushSymbols.find(delimiter) == std::string::npos) {
		if (!pushSymbols.empty()) {
			symbols.push_back(pushSymbols);
		}
		return symbols;
	}

	std::stringstream ss(pushSymbols);
	std::string symbol;

	// Extract symbols separated by the delimiter
	while (std::getline(ss, symbol, delimiter)) {
		if (!symbol.empty()) {
			symbols.push_back(symbol);
		}
	}
	return symbols;
}

void PushdownAutomaton::validateTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                           const std::string &input, const std::string &stackSymbol,
                                           const std::string &pushSymbol) {
	if (!stateExists(fromStateKey)) {
		throw StateNotFoundException(fromStateKey);
	}
	if (!stateExists(toStateKey)) {
		throw StateNotFoundException(toStateKey);
	}
	if (!input.empty() && !inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in input alphabet: " + input);
	}
	if (!stackSymbol.empty() && !stackAlphabetSymbolExists(stackSymbol)) {
		throw InvalidTransitionException("Stack symbol not in stack alphabet: " + stackSymbol);
	}
	std::vector<std::string> pushSymbolsVec = parsePushSymbols(pushSymbol);
	std::string invalidPushSymbols;
	for (const auto &symbol : pushSymbolsVec) {
		if (!symbol.empty() && !stackAlphabetSymbolExists(symbol)) {
			invalidPushSymbols += symbol + ", ";
		}
	}
	if (!invalidPushSymbols.empty()) {
		invalidPushSymbols.erase(invalidPushSymbols.size() - 2, 2);
		throw InvalidTransitionException("Push symbol not in stack alphabet: " + invalidPushSymbols);
	}

	PDAState *fromState = getStateInternal(fromStateKey);
	std::string transitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Check if the new transition would be a duplicate
	if (fromState->transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + fromStateKey + " -> " + toStateKey +
		                                 "| input: " + input + " | stack symbol: " + stackSymbol +
		                                 " | push symbol: " + pushSymbol);
	}
}

PDAState *PushdownAutomaton::getStateInternal(const std::string &key) {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return &(it->second);
}

std::vector<std::string> PushdownAutomaton::getInput() const {
	return input;
}

void PushdownAutomaton::setInput(const std::vector<std::string> &input) {
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
	this->inputHead = 0;
}

void PushdownAutomaton::addInput(const std::vector<std::string> &input) {
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
}

int PushdownAutomaton::getInputHead() const {
	return inputHead;
}

void PushdownAutomaton::setInputHead(const int &inputHead) {
	if (inputHead < 0) {
		this->inputHead = 0;
	} else if (inputHead >= input.size()) {
		this->inputHead = input.size() - 1;
	} else {
		this->inputHead = inputHead;
	}
}

std::stack<std::string> PushdownAutomaton::getStack() {
	return stack;
}

void PushdownAutomaton::setStack(std::stack<std::string> stack) {
	this->stack = stack;
}

void PushdownAutomaton::pushStack(const std::string &symbol) {
	if (symbol.empty() || !stackAlphabetSymbolExists(symbol)) {
		throw StackAlphabetSymbolNotFoundException("Symbol not in alphabet");
	}

	stack.push(symbol);
}

std::string PushdownAutomaton::popStack() {
	if (stack.empty()) {
		return "";
	}

	const std::string &top = stack.top();
	stack.pop();

	return top;
}

std::string PushdownAutomaton::peekStack() const {
	if (stack.empty()) {
		return "";
	}

	return stack.top();
}

void PushdownAutomaton::resetStack() {
	stack = std::stack<std::string>();
}

bool PushdownAutomaton::stateExists(const std::string &key) const {
	return states.find(key) != states.end();
}

bool PushdownAutomaton::inputAlphabetSymbolExists(const std::string &symbol) const {
	return inputAlphabet.find(symbol) != inputAlphabet.end();
}

bool PushdownAutomaton::stackAlphabetSymbolExists(const std::string &symbol) const {
	return stackAlphabet.find(symbol) != stackAlphabet.end();
}

void PushdownAutomaton::addState(const std::string &label, const bool &isAccept) {
	// Check if state label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	PDAState state(label, isAccept);

	// Check if start state is empty, if so set it to the new state
	if (startState.empty()) {
		startState = label;
	}

	// Check if current state is empty, if so set it to the new state
	if (currentState.empty()) {
		currentState = label;
	}

	// Update the vector and invalidate conversion cache
	states[state.getKey()] = state;
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateStateLabel(const std::string &key, const std::string &label) {
	// Check if state with new label already exists
	if (stateExists(label)) {
		throw InvalidAutomatonDefinitionException("State with label " + label + " already exists");
	}

	PDAState *state = getStateInternal(key);

	if (key == startState) {
		startState = label;
	}

	state->setLabel(label);

	states[state->getKey()] = *state;
	states.erase(key);

	statesCacheInvalidated = true;
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

void PushdownAutomaton::setCurrentState(const std::string &key) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	currentState = key;
}

PDAState PushdownAutomaton::getState(const std::string &key) const {
	auto it = states.find(key);
	// Check if state exists
	if (it == states.end()) {
		throw StateNotFoundException(key);
	}
	return it->second;
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

void PushdownAutomaton::removeState(const std::string &key, const bool &strict) {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<PDATransition> transitions = pair.second.getTransitions();
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

void PushdownAutomaton::removeStates(const std::vector<std::string> &keys, const bool &strict) {
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
			std::vector<PDATransition> transitions = pair.second.getTransitions();
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

void PushdownAutomaton::clearStates() {
	states.clear();
	currentState = "";
	startState = "";
	statesCacheInvalidated = true;
}

void PushdownAutomaton::setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict) {
	std::unordered_map<std::string, std::vector<std::string>> conflictingTransitions;
	std::unordered_set<std::string> newAlphabet =
	    std::unordered_set<std::string>(inputAlphabet.begin(), inputAlphabet.end());

	for (auto &pair : states) {
		std::vector<PDATransition> transitions = pair.second.getTransitions();
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

void PushdownAutomaton::addInputAlphabet(const std::vector<std::string> &inputAlphabet) {
	for (const auto &symbol : inputAlphabet) {
		this->inputAlphabet.insert(symbol);
	}
	inputAlphabetCacheInvalidated = true;
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

void PushdownAutomaton::removeInputAlphabetSymbol(const std::string &symbol, const bool &strict) {
	// Check if symbol exists
	if (!inputAlphabetSymbolExists(symbol)) {
		throw InputAlphabetSymbolNotFoundException(symbol);
	}
	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<PDATransition> transitions = pair.second.getTransitions();
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

void PushdownAutomaton::removeInputAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict) {
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
			std::vector<PDATransition> transitions = pair.second.getTransitions();
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

void PushdownAutomaton::clearInputAlphabet(const bool &strict) {
	for (auto &pair : states) {
		std::vector<PDATransition> transitions = pair.second.getTransitions();
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

void PushdownAutomaton::setStackAlphabet(const std::vector<std::string> &stackAlphabet, const bool &strict) {
	std::unordered_map<std::string, std::vector<std::string>> conflictingTransitions;
	std::unordered_set<std::string> newAlphabet =
	    std::unordered_set<std::string>(inputAlphabet.begin(), inputAlphabet.end());

	for (auto &pair : states) {
		std::vector<PDATransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			const std::string &stackSymbol = transition.getStackSymbol();
			bool found = newAlphabet.find(stackSymbol) != newAlphabet.end();
			if (!found) {
				if (strict) {
					conflictingTransitions[stackSymbol].push_back(transition.getKey());
				} else {
					pair.second.removeTransition(transition.getKey());
				}
			}

			// Check for push symbols
			std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
			for (const auto &pushSymbol : pushSymbolsVec) {
				found = newAlphabet.find(pushSymbol) != newAlphabet.end();
				if (!found) {
					if (strict) {
						conflictingTransitions[pushSymbol].push_back(transition.getKey());
					} else {
						pair.second.removeTransition(transition.getKey());
					}
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

	this->stackAlphabet = std::unordered_set<std::string>(stackAlphabet.begin(), stackAlphabet.end());
	stackAlphabetCacheInvalidated = true;
}

void PushdownAutomaton::addStackAlphabet(const std::vector<std::string> &stackAlphabet) {
	for (const auto &symbol : stackAlphabet) {
		this->stackAlphabet.insert(symbol);
	}
	stackAlphabetCacheInvalidated = true;
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

void PushdownAutomaton::removeStackAlphabetSymbol(const std::string &symbol, const bool &strict) {
	// Check if symbol exists
	if (!stackAlphabetSymbolExists(symbol)) {
		throw StackAlphabetSymbolNotFoundException(symbol);
	}

	std::vector<std::string> conflictingTransitions;

	// Check for conflicting transitions
	for (auto &pair : states) {
		std::vector<PDATransition> transitions = pair.second.getTransitions();
		for (const auto &transition : transitions) {
			if (transition.getStackSymbol() == symbol || transition.getPushSymbol() == symbol) {
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

	stackAlphabet.erase(symbol);
	stackAlphabetCacheInvalidated = true;
}

void PushdownAutomaton::removeStackAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict) {
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
		throw StackAlphabetSymbolNotFoundException(missingSymbolsString);
	}

	// If no missing symbols found then we remove
	for (const auto &symbol : symbols) {
		stackAlphabet.erase(symbol);
	}
	stackAlphabetCacheInvalidated = true;
}

void PushdownAutomaton::clearStackAlphabet(const bool &strict) {
	stackAlphabet.clear();
	stackAlphabetCacheInvalidated = true;
}

std::string PushdownAutomaton::getStartState() const {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state is not set");
	}
	return startState;
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

PDATransition PushdownAutomaton::getTransition(const std::string &key) const {
	const std::string &stateKey = PDATransition::getFromStateFromKey(key);
	return getState(stateKey).getTransition(key);
}

void PushdownAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                      const std::string &input, const std::string &stackSymbol,
                                      const std::string &pushSymbol) {
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
	PDAState *state = getStateInternal(fromStateKey);
	state->addTransition(toStateKey, input, stackSymbol, pushSymbol);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionInput(const std::string &transitionKey, const std::string &input) {
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

	// Validate state existence and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	PDAState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
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

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	PDAState *oldFromState = getStateInternal(oldFromStateKey);
	// Check if the old transition exists
	if (!oldFromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	PDAState *newFromState = getStateInternal(fromStateKey);

	// Remove the transition from the old from state
	oldFromState->removeTransition(transitionKey);
	// Add the transition to the new from state
	newFromState->addTransition(toStateKey, input, stackSymbol, pushSymbol);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionToState(const std::string &transitionKey, const std::string &toStateKey) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	PDAState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionToState(transitionKey, toStateKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionStackSymbol(const std::string &transitionKey, const std::string &stackSymbol) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
	std::string pushSymbol = PDATransition::getPushSymbolFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Validate transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	PDAState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionStackSymbol(transitionKey, stackSymbol);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::updateTransitionPushSymbol(const std::string &transitionKey, const std::string &pushSymbol) {
	std::string input = PDATransition::getInputFromKey(transitionKey);
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);
	std::string toStateKey = PDATransition::getToStateFromKey(transitionKey);
	std::string stackSymbol = PDATransition::getStackSymbolFromKey(transitionKey);

	// Generate the key of the transition after the update
	std::string newTransitionKey =
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	// Do we even need to update?
	if (transitionKey == newTransitionKey) {
		return;
	}

	// Vali	date transition definition and transition not being a duplicate
	validateTransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);

	PDAState *fromState = getStateInternal(fromStateKey);
	// Check if the old transition exists
	if (!fromState->transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	fromState->setTransitionPushSymbol(transitionKey, pushSymbol);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::removeTransition(const std::string &transitionKey) {
	std::string fromStateKey = PDATransition::getFromStateFromKey(transitionKey);

	PDAState *fromState = getStateInternal(fromStateKey);
	fromState->removeTransition(transitionKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey) {
	PDAState *fromState = getStateInternal(fromStateKey);
	fromState->clearTransitionsTo(toStateKey);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearStateTransitions(const std::string &key) {
	PDAState *state = getStateInternal(key);
	state->clearTransitions();
	statesCacheInvalidated = true;
}

void PushdownAutomaton::clearTransitions() {
	for (auto &pair : states) {
		pair.second.clearTransitions();
	}
	statesCacheInvalidated = true;
}

void PushdownAutomaton::addAcceptState(const std::string &stateKey) {
	PDAState *state = getStateInternal(stateKey);

	state->setIsAccept(true);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::addAcceptStates(const std::vector<std::string> &keys) {
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
		PDAState *state = getStateInternal(key);
		state->setIsAccept(true);
	}

	statesCacheInvalidated = true;
}

void PushdownAutomaton::removeAcceptState(const std::string &stateKey) {
	PDAState *state = getStateInternal(stateKey);

	state->setIsAccept(false);
	statesCacheInvalidated = true;
}

void PushdownAutomaton::removeAcceptStates(const std::vector<std::string> &keys) {
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
		PDAState *state = getStateInternal(key);
		state->setIsAccept(false);
	}

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
	inputHead = 0;
}

bool PushdownAutomaton::isAccepting() const {
	return getState(currentState).getIsAccept();
}

bool PushdownAutomaton::checkNextState(const std::string &key) const {
	// Check if state exists
	if (!stateExists(key)) {
		throw StateNotFoundException(key);
	}

	const std::string &currentInput = getInput()[inputHead];

	PDAState state = getState(currentState);
	const std::vector<PDATransition> &transitions = state.getTransitions();

	const std::string &stackTop = stack.empty() ? "" : stack.top();
	for (const auto &transition : transitions) {
		if (transition.getToStateKey() != key) {
			continue;
		}
		if (transition.getStackSymbol() != stackTop && !transition.getStackSymbol().empty()) {
			continue;
		}
		if (transition.getInput().empty() || transition.getInput() == currentInput) {
			return true;
		}
	}
	return false;
}