#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicFiniteAutomaton.h"

FATransition
NonDeterministicFiniteAutomaton::decideRandomTransition(const std::unordered_set<FATransition> &transitions) {
	int randomIndex = rand() % transitions.size();
	auto it = transitions.begin();
	std::advance(it, randomIndex);
	return *it;
}

void NonDeterministicFiniteAutomaton::setInput(const std::vector<std::string> &input) {
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

	FiniteAutomaton::setInput(input);
}

void NonDeterministicFiniteAutomaton::addInput(const std::vector<std::string> &input) {
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

	FiniteAutomaton::addInput(input);
}

void NonDeterministicFiniteAutomaton::updateStateLabel(const std::string &key, const std::string &label) {
	FiniteAutomaton::updateStateLabel(key, label);
	if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
		possibleCurrentStates.erase(key);
		possibleCurrentStates.insert(label);
		possibleCurrentStatesCacheInvalidated = true;
	}
}

void NonDeterministicFiniteAutomaton::setCurrentState(const std::string &state) {
	FiniteAutomaton::setCurrentState(state);
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(state);
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::removeState(const std::string &key, const bool &strict) {
	FiniteAutomaton::removeState(key, strict);

	if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
		possibleCurrentStates.erase(key);
	}
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::removeStates(const std::vector<std::string> &keys, const bool &strict) {
	FiniteAutomaton::removeStates(keys, strict);

	for (const auto &key : keys) {
		if (possibleCurrentStates.find(key) != possibleCurrentStates.end()) {
			possibleCurrentStates.erase(key);
		}
	}
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::clearStates() {
	FiniteAutomaton::clearStates();
	possibleCurrentStates.clear();
	possibleCurrentStatesCacheInvalidated = true;
}

void NonDeterministicFiniteAutomaton::setStartState(const std::string &key) {
	FiniteAutomaton::setStartState(key);

	if (possibleCurrentStates.empty()) {
		possibleCurrentStates.insert(startState);
		possibleCurrentStatesCacheInvalidated = true;
	}
}

void NonDeterministicFiniteAutomaton::addTransition(const std::string &fromStateKey, const std::string &toStateKey,
                                                    const std::string &input) {
	// Check if the input is in the alphabet
	if (!input.empty() && !inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::addTransition(fromStateKey, toStateKey, input);
}

void NonDeterministicFiniteAutomaton::updateTransitionInput(const std::string &transitionKey,
                                                            const std::string &input) {
	// Check if the input is in the alphabet
	if (!input.empty() && !inputAlphabetSymbolExists(input)) {
		throw InvalidTransitionException("Input not in alphabet: " + input);
	}

	FiniteAutomaton::updateTransitionInput(transitionKey, input);
}

std::vector<std::string> NonDeterministicFiniteAutomaton::getPossibleCurrentStates() {
	// if the conversion cache from unordered_set to vector is not valid then recompute
	if (possibleCurrentStatesCacheInvalidated) {
		cachedPossibleCurrentStates.clear();
		for (const auto &state : possibleCurrentStates) {
			cachedPossibleCurrentStates.push_back(state);
		}
		possibleCurrentStatesCacheInvalidated = false;
	}
	return cachedPossibleCurrentStates;
}

void NonDeterministicFiniteAutomaton::reset() {
	FiniteAutomaton::reset();
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(startState);
	currentState = startState;
	possibleCurrentStatesCacheInvalidated = true;
}

bool NonDeterministicFiniteAutomaton::processInput() {
	if (currentState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state or start state must be set to run process input");
	}

	std::string input = "";
	if (inputHead < this->input.size()) {
		input = this->input[inputHead];
	}

	std::unordered_set<FATransition> possibleTransitions;
	std::unordered_set<std::string> possibleCurrentStates;

	const std::vector<FATransition> &transitions = getStateInternal(currentState)->getTransitions();

	for (const auto &transition : transitions) {
		if (transition.getInput() == input || transition.getInput().empty()) {
			possibleTransitions.insert(transition);
			possibleCurrentStates.insert(transition.getToStateKey());
		}
	}

	if (possibleTransitions.empty()) {
		return false;
	}

	FATransition transitionChosen = decideRandomTransition(possibleTransitions);

	// Update current state to the chosen transition
	currentState = transitionChosen.getToStateKey();
	// Only increment the head if the input is a match
	const bool &incrementHead = transitionChosen.getInput() == input && inputHead < this->input.size();
	if (incrementHead) {
		inputHead++;
	}

	this->possibleCurrentStates = possibleCurrentStates;
	possibleCurrentStatesCacheInvalidated = true;

	// Return whether the randomly chosen current state is an accept state
	return getStateInternal(currentState)->getIsAccept();
}

struct Visited {
	std::string state;
	int head;

	bool operator==(const Visited &other) const {
		return state == other.state && head == other.head;
	}
};

namespace std {
template <> struct hash<Visited> {
	size_t operator()(const Visited &v) const {
		return hash<std::string>()(v.state) ^ (hash<int>()(v.head) << 1);
	}
};
} // namespace std

bool NonDeterministicFiniteAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state must be set to run simulate");
	}

	struct Branch {
		std::string state;
		int head;
		int depth;
	};

	std::queue<Branch> branches;
	std::unordered_set<Visited> visited;

	branches.push({startState, 0, 0});

	while (!branches.empty()) {
		Branch branch = branches.front();
		branches.pop();

		visited.insert({branch.state, branch.head});

		if (branch.head == input.size() && getStateInternal(branch.state)->getIsAccept()) {
			return true;
		}

		if (branch.depth >= simulationDepth) {
			continue;
		}

		std::string currentInput = "";
		if (branch.head < input.size()) {
			currentInput = input[branch.head];
		}

		const std::vector<FATransition> &transitions = getStateInternal(branch.state)->getTransitions();

		for (const auto &transition : transitions) {
			if (transition.getInput().empty() || transition.getInput() == currentInput) {
				// Only increment the head if the input is a match and the input head is less than the input size
				const bool &incrementHead = transition.getInput() == currentInput && branch.head < input.size();
				const int &newHead = incrementHead ? branch.head + 1 : branch.head;

				if (visited.find({transition.getToStateKey(), newHead}) != visited.end()) {
					continue;
				}

				branches.push({transition.getToStateKey(), newHead, branch.depth + 1});
			}
		}
	}

	return false;
}