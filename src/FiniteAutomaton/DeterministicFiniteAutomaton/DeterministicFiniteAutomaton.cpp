#include "AutomataSimulator/DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::~DeterministicFiniteAutomaton() = default;

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

void DeterministicFiniteAutomaton::setInput(const std::vector<std::string> &input) {
	std::unordered_set<std::string> missingSymbols;

	for (const auto &symbol : input) {
		if (!inputAlphabetSymbolExists(symbol)) {
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

	FiniteAutomaton::setInput(input);
}

void DeterministicFiniteAutomaton::addInput(const std::vector<std::string> &input) {
	std::unordered_set<std::string> missingSymbols;

	for (const auto &symbol : input) {
		if (!inputAlphabetSymbolExists(symbol)) {
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

	FiniteAutomaton::addInput(input);
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

bool DeterministicFiniteAutomaton::processInput() {
	if (currentState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state or start state must be set to run process input");
	}

	if (inputHead >= this->input.size()) {
		return getStateInternal(currentState)->getIsAccept();
	}

	const std::string &inputSymbol = this->input[inputHead];

	const std::vector<FATransition> &transitions = getStateInternal(currentState)->getTransitions();

	for (const auto &transition : transitions) {
		if (transition.getInput() != inputSymbol) {
			continue;
		}
		currentState = transition.getToStateKey();
		inputHead++;
		return getStateInternal(currentState)->getIsAccept();
	}
	return false;
}

bool DeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state must be set to run simulate");
	}

	int inputIdx = 0;
	int currentDepth = 0;
	std::string simulationCurrentState = getStartState();

	while (currentDepth <= simulationDepth && inputIdx < input.size()) {
		const std::vector<FATransition> &transitions = getStateInternal(simulationCurrentState)->getTransitions();

		std::string currentInput = input[inputIdx];

		bool transitionFound = false;
		for (const auto &transition : transitions) {
			if (transition.getInput() != currentInput) {
				continue;
			}
			simulationCurrentState = transition.getToStateKey();
			transitionFound = true;
			break;
		}
		if (!transitionFound) {
			return false;
		}
		currentDepth++;
		inputIdx++;
	}

	return getStateInternal(simulationCurrentState)->getIsAccept();
}