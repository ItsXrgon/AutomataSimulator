#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicTuringMachine.h"

TMTransition
NonDeterministicTuringMachine::decideRandomTransition(const std::unordered_set<TMTransition> &transitions) {
	int randomIndex = rand() % transitions.size();
	auto it = transitions.begin();
	std::advance(it, randomIndex);
	return *it;
}

std::vector<std::string> NonDeterministicTuringMachine::getPossibleCurrentStates() {
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

void NonDeterministicTuringMachine::reset() {
	TuringMachine::reset();
	possibleCurrentStates.clear();
	possibleCurrentStates.insert(startState);
	currentState = startState;
	possibleCurrentStatesCacheInvalidated = true;
}

bool NonDeterministicTuringMachine::processInput() {
	if (currentState.empty()) {
		throw InvalidAutomatonDefinitionException("Current state or start state must be set to run process input");
	}

	std::unordered_set<TMTransition> possibleTransitions;

	const std::vector<TMTransition> &transitions = getStateInternal(currentState)->getTransitions();
	std::string tapeValue = tape.read();

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() != tapeValue && !transition.getReadSymbol().empty()) {
			continue;
		}
		possibleTransitions.insert(transition);
	}

	if (possibleTransitions.empty()) {
		return getStateInternal(currentState)->getIsAccept();
	}

	TMTransition transitionChosen = decideRandomTransition(possibleTransitions);

	// Update current state to the chosen transition
	currentState = transitionChosen.getToStateKey();

	// If the write symbol is not epsilon then write to the tape
	if (!transitionChosen.getWriteSymbol().empty()) {
		tape.write(transitionChosen.getWriteSymbol());
	}
	tape.move(transitionChosen.getDirection());
	currentState = transitionChosen.getToStateKey();

	if (transitionChosen.getInput() != "") {
		inputHead++;
	}

	// Return whether the randomly chosen current state is an accept state
	return getStateInternal(currentState)->getIsAccept();
}

struct Visited {
	std::string state;
	int head;
	std::string tape;

	bool operator==(const Visited &other) const {
		return state == other.state && head == other.head && tape == other.tape;
	}
};

namespace std {
template <> struct hash<Visited> {
	size_t operator()(const Visited &v) const {
		return hash<std::string>()(v.state) ^ (hash<int>()(v.head) << 1) ^ (hash<std::string>()(v.tape) << 2);
	}
};
} // namespace std

bool NonDeterministicTuringMachine::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	if (startState.empty()) {
		throw InvalidStartStateException("Start state must be set to run simulate");
	}

	struct Branch {
		std::string state;
		TMTape tape;
		int head;
		int depth;
	};

	std::queue<Branch> branches;
	std::unordered_set<Visited> visited;

	branches.push({getStartState(), TMTape(), 0, 0});

	while (!branches.empty()) {
		Branch branch = branches.front();
		branches.pop();

		visited.insert({branch.state, branch.head, tape.toString()});

		if (branch.depth >= simulationDepth) {
			continue;
		}

		std::string currentInput = "";
		if (branch.head < input.size()) {
			currentInput = input[branch.head];
		}

		const std::vector<TMTransition> &transitions = getStateInternal(branch.state)->getTransitions();

		for (const auto &transition : transitions) {
			if (transition.getInput() == "" || transition.getInput() == currentInput) {

				if (transition.getReadSymbol() != branch.tape.read()) {
					continue;
				}
				const int &newHead = transition.getInput() == "" ? branch.head : branch.head + 1;
				TMTape branchTape = branch.tape;
				if (!transition.getWriteSymbol().empty()) {
					branchTape.write(transition.getWriteSymbol());
				}
				branchTape.move(transition.getDirection());

				if (visited.find({transition.getToStateKey(), newHead, branchTape.toString()}) != visited.end()) {
					continue;
				}
				TMTape newTape = branch.tape;

				branches.push({transition.getToStateKey(), newTape, newHead, branch.depth + 1});
			}
		}

		if (branch.head == input.size() && getStateInternal(branch.state)->getIsAccept()) {
			return true;
		}
	}
}
