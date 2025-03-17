#define AUTOMATASIMULATOR_EXPORTS
#include "NonDeterministicPushdownAutomaton.h"

std::string NonDeterministicPushdownAutomaton::decideRandomState(const std::unordered_set<std::string> &states) {
	// If empty, return empty string
	if (states.empty()) {
		return "";
	}

	// Randomly choose a state from the set of states
	int randomIndex = rand() % states.size();
	auto it = states.begin();
	std::advance(it, randomIndex);
	return *it;
}

bool NonDeterministicPushdownAutomaton::processInput(const std::string &input) {
	return false;
}

bool NonDeterministicPushdownAutomaton::simulate(const std::vector<std::string> &input, const int &simulationDepth) {
	// Create a queue of configurations to explore
	// Each configuration is: <state, inputIndex, stack, depth>
	struct Configuration {
		std::string state;
		size_t inputIdx;
		std::stack<std::string> stack;
		size_t depth;
	};
	std::queue<Configuration> configurations;

	// Start with initial configuration
	std::stack<std::string> initialStack;
	initialStack.push(INITIAL_STACK_SYMBOL);
	configurations.push({getStartState(), 0, initialStack, 0});

	// Set to track visited configurations to avoid cycles
	std::set<std::tuple<std::string, size_t, std::string>> visited;

	while (!configurations.empty()) {
		// Get the next configuration to explore
		Configuration current = configurations.front();
		configurations.pop();

		// Check if we've exceeded simulation depth
		if (current.depth > simulationDepth) {
			throw SimulationDepthExceededException(simulationDepth);
		}

		// Check if we're in an accepting state and have consumed all input
		if (current.inputIdx >= input.size()) {
			if (getStateInternal(current.state)->getIsAccept()) {
				return true;
			}
		}

		// Get transitions for current state
		const std::vector<PDATransition> &transitions = getStateInternal(current.state)->getTransitions();
		std::string stackTop = current.stack.empty() ? "" : current.stack.top();

		// Try processing with current input if available
		bool anyTransition = false;
		if (current.inputIdx < input.size()) {
			std::string currentInput = input[current.inputIdx];

			for (const auto &transition : transitions) {
				if (transition.getInput() == currentInput && transition.getStackSymbol() == stackTop) {
					// Create a new configuration with the transition applied
					Configuration newConfig = current;
					newConfig.state = transition.getToStateKey();
					newConfig.inputIdx++; // Consume input
					newConfig.depth++;

					// Update stack
					if (!transition.getStackSymbol().empty()) {
						newConfig.stack.pop();
					}
					std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
					for (auto it = pushSymbolsVec.rbegin(); it != pushSymbolsVec.rend(); ++it) {
						newConfig.stack.push(*it);
					}

					// Create a serialized representation of the stack for visited check
					std::string stackStr = serializeStack(newConfig.stack);

					// Only add if we haven't visited this configuration
					auto configKey = std::make_tuple(newConfig.state, newConfig.inputIdx, stackStr);
					if (visited.find(configKey) == visited.end()) {
						visited.insert(configKey);
						configurations.push(newConfig);
						anyTransition = true;
					}
				}
			}
		}

		// Try epsilon transitions
		for (const auto &transition : transitions) {
			if (transition.getInput().empty() && transition.getStackSymbol() == stackTop) {
				// Create a new configuration with the epsilon transition applied
				Configuration newConfig = current;
				newConfig.state = transition.getToStateKey();
				newConfig.depth++;

				// Update stack
				if (!transition.getStackSymbol().empty()) {
					newConfig.stack.pop();
				}
				std::vector<std::string> pushSymbolsVec = parsePushSymbols(transition.getPushSymbol());
				for (auto it = pushSymbolsVec.rbegin(); it != pushSymbolsVec.rend(); ++it) {
					newConfig.stack.push(*it);
				}

				// Create a serialized representation of the stack for visited check
				std::string stackStr = serializeStack(newConfig.stack);

				// Only add if we haven't visited this configuration
				auto configKey = std::make_tuple(newConfig.state, newConfig.inputIdx, stackStr);
				if (visited.find(configKey) == visited.end()) {
					visited.insert(configKey);
					configurations.push(newConfig);
					anyTransition = true;
				}
			}
		}
	}

	// No accepting configuration found
	return false;
}

// Helper function to serialize a stack for the visited set
std::string NonDeterministicPushdownAutomaton::serializeStack(const std::stack<std::string> &stack) {
	std::stack<std::string> tempStack = stack;
	std::string result;

	while (!tempStack.empty()) {
		result += tempStack.top() + "|";
		tempStack.pop();
	}

	return result;
}