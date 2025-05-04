#pragma once
#include "PushdownAutomaton.h"
#include "config.h"
#include <set>
#include <unordered_set>
#include <vector>

/**
 * @brief Represents a deterministic pushdown automaton.
 */
class AUTOMATASIMULATOR_API NonDeterministicPushdownAutomaton : public PushdownAutomaton {
  private:
	/**
	 * @brief The possible current states of the automaton.
	 */
	std::unordered_set<std::string> possibleCurrentStates;

	/**
	 * @brief Cached convertion of the  possible current states from unordered map to vector.
	 */
	std::vector<std::string> cachedPossibleCurrentStates;

	/**
	 * @brief Boolean indicating whether the cached  possible current states is invalidated or not.
	 */
	bool possibleCurrentStatesCacheInvalidated;

	/**
	 * @brief Decides a random transition from a set of transitions.
	 * @param transitions The set of transitions to decide from.
	 * @return A random transitions from the set.
	 */
	PDATransition decideRandomTransition(const std::unordered_set<PDATransition> &transitions);

	/**
	 * @brief Serializes the stack to a string.
	 * @param stack The stack to serialize.
	 * @return The serialized stack.
	 */
	std::string serializeStack(const std::stack<std::string> &stack);

  public:
	virtual ~NonDeterministicPushdownAutomaton() override;

	/**
	 * @brief Gets the possible current states
	 * @brief Possible current states are the states that the automata could of
	 * chosen in the last simulation based on the previous current state
	 * @return The possible current states of the automaton.
	 */
	std::vector<std::string> getPossibleCurrentStates();

	/**
	 * @brief Reset the NPDA to the start state.
	 */
	void reset() override;

	/**
	 * @brief Moves the automaton to the next state based on the current input head.
	 * @brief If an epsilon transition is taken, the input head is not incremented.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 * @throws InputConsumedException If the input head exceeds the length of the input.
	 */
	bool processInput() override;

	/**
	 * @brief Simulates the automaton on a given input string and depth.
	 * @brief Returns false if the simulation depth is exceeded and no accept state is reached.
	 * @param input The input strings to process.
	 * @param simulationDepth The maximum number of transitions to simulate. Default is 50.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) override;
};
