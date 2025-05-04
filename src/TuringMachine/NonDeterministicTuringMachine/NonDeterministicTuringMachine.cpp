#include "AutomataSimulator/NonDeterministicTuringMachine.h"

NonDeterministicTuringMachine::~NonDeterministicTuringMachine() = default;

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
	std::unordered_set<std::string> newPossibleCurrentStates;

	const std::vector<TMTransition> &transitions = getStateInternal(currentState)->getTransitions();
	std::string inputSymbol = tape.read();

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() != inputSymbol && !transition.getReadSymbol().empty()) {
			continue;
		}
		possibleTransitions.insert(transition);
		newPossibleCurrentStates.insert(transition.getToStateKey());
	}

	if (possibleTransitions.empty()) {
		return false;
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

	this->possibleCurrentStates = newPossibleCurrentStates;
	possibleCurrentStatesCacheInvalidated = true;

	// Return whether the randomly chosen current state is an accept state
	return getStateInternal(currentState)->getIsAccept();
}

struct Visited {
	std::string state;
	std::string tape;

	bool operator==(const Visited &other) const {
		return state == other.state && tape == other.tape;
	}
};

namespace std {
template <> struct hash<Visited> {
	size_t operator()(const Visited &v) const {
		return hash<std::string>()(v.state) ^ (hash<std::string>()(v.tape) << 1);
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
		int depth;
	};

	std::queue<Branch> branches;
	std::unordered_set<Visited> visited;

	TMTape initialTape;
	initialTape.loadInput(input);

	branches.push({getStartState(), initialTape, 0});

	while (!branches.empty()) {
		Branch branch = branches.front();
		branches.pop();

		if (getStateInternal(branch.state)->getIsAccept()) {
			return true;
		}

		visited.insert({branch.state, branch.tape.toString()});

		if (branch.depth >= simulationDepth) {
			continue;
		}

		const std::string &tapeValue = branch.tape.read();

		const std::vector<TMTransition> &transitions = getStateInternal(branch.state)->getTransitions();

		for (const auto &transition : transitions) {
			if (transition.getReadSymbol() != tapeValue && !transition.getReadSymbol().empty()) {
				continue;
			}

			TMTape branchTape = branch.tape;
			if (!transition.getWriteSymbol().empty()) {
				branchTape.write(transition.getWriteSymbol());
			}
			branchTape.move(transition.getDirection());

			if (visited.find({transition.getToStateKey(), branchTape.toString()}) != visited.end()) {
				continue;
			}

			branches.push({transition.getToStateKey(), branchTape, branch.depth + 1});
		}
	}

	return false;
}
