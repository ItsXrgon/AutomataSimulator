#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicPushdownAutomaton.h"

PDATransition
NonDeterministicPushdownAutomaton::decideRandomTransition(const std::unordered_set<PDATransition> &transitions) {
	int randomIndex = rand() % transitions.size();
	auto it = transitions.begin();
	std::advance(it, randomIndex);
	return *it;
}

std::string NonDeterministicPushdownAutomaton::serializeStack(const std::stack<std::string> &stack) {
	std::stack<std::string> tempStack = stack;
	std::string result;

	while (!tempStack.empty()) {
		result += tempStack.top() + "|";
		tempStack.pop();
	}

	return result;
}

std::vector<std::string> NonDeterministicPushdownAutomaton::getPossibleCurrentStates() {
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

void NonDeterministicPushdownAutomaton::reset() {
	PushdownAutomaton::reset();
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(startState);
	currentState = startState;
	possibleCurrentStatesCacheInvalidated = true;
}

bool NonDeterministicPushdownAutomaton::processInput() {
	if (currentState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state or start state must be set to run process input");
	}

	std::string input = "";
	if (inputHead < this->input.size()) {
		input = this->input[inputHead];
	}

	std::unordered_set<PDATransition> possibleTransitions;
	std::unordered_set<std::string> possibleCurrentStates;

	const std::vector<PDATransition> &transitions = getStateInternal(currentState)->getTransitions();
	std::string stackTop = stack.empty() ? "" : stack.top();

	for (const auto &transition : transitions) {
		if (transition.getStackSymbol() != stackTop && !transition.getStackSymbol().empty()) {
			continue;
		}
		if (transition.getInput() == input || transition.getInput().empty()) {
			possibleTransitions.insert(transition);
			possibleCurrentStates.insert(transition.getToStateKey());
		}
	}

	if (possibleTransitions.empty()) {
		return false;
	}

	PDATransition transitionChosen = decideRandomTransition(possibleTransitions);

	// Update current state to the chosen transition
	currentState = transitionChosen.getToStateKey();

	// If the stack symbol is not epsilon then pop from stack
	if (!transitionChosen.getStackSymbol().empty()) {
		stack.pop();
	}

	std::vector<std::string> pushSymbolsVec = parsePushSymbols(transitionChosen.getPushSymbol());
	for (auto it = pushSymbolsVec.rbegin(); it != pushSymbolsVec.rend(); ++it) {
		stack.push(*it);
	}
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
	std::string stack;

	bool operator==(const Visited &other) const {
		return state == other.state && head == other.head && stack == other.stack;
	}
};

namespace std {
template <> struct hash<Visited> {
	size_t operator()(const Visited &v) const {
		return hash<std::string>()(v.state) ^ (hash<int>()(v.head) << 1) ^ (hash<std::string>()(v.stack) << 2);
	}
};
} // namespace std

bool NonDeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state must be set to run simulate");
	}

	struct Branch {
		std::string state;
		std::stack<std::string> stack;
		int head;
		int depth;
	};

	std::queue<Branch> branches;
	std::unordered_set<Visited> visited;

	std::stack<std::string> initialStack;
	initialStack.push(INITIAL_STACK_SYMBOL);
	branches.push({getStartState(), initialStack, 0, 0});

	while (!branches.empty()) {
		Branch branch = branches.front();
		branches.pop();

		visited.insert({branch.state, branch.head, serializeStack(branch.stack)});

		if (branch.head == input.size() && getStateInternal(branch.state)->getIsAccept()) {
			return true;
		}

		if (branch.depth >= simulationDepth) {
			continue;
		}

		const std::vector<PDATransition> &transitions = getStateInternal(branch.state)->getTransitions();

		std::string currentInput = "";
		if (branch.head < input.size()) {
			currentInput = input[branch.head];
		}

		const std::string stackTop = branch.stack.empty() ? "" : branch.stack.top();

		for (const auto &transition : transitions) {
			if (transition.getStackSymbol() != stackTop && !transition.getStackSymbol().empty()) {
				continue;
			}

			if (transition.getInput() == "" || transition.getInput() == input[branch.head]) {
				const int &newHead = transition.getInput() == "" ? branch.head : branch.head + 1;
				std::stack<std::string> branchStack = branch.stack;
				if (!transition.getStackSymbol().empty()) {
					branchStack.pop();
				}

				std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
				for (auto it = pushSymbolsVec.rbegin(); it != pushSymbolsVec.rend(); ++it) {
					branchStack.push(*it);
				}

				std::string serializedStack = serializeStack(branchStack);

				if (visited.find({transition.getToStateKey(), newHead, serializedStack}) != visited.end()) {
					continue;
				}

				branches.push({transition.getToStateKey(), branchStack, newHead, branch.depth + 1});
			}
		}
	}
}