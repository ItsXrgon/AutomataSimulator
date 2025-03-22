#pragma once
#include "../PushdownAutomaton.h"
#include <set>
#include <unordered_set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a deterministic pushdown automaton.
 * @brief A deterministic pushdown automaton is defined by a
 * @brief - Finite set of states. Formally defined as Q. including their transitions.
 * @brief - Finite set of input symbols (alphabet). Formally defined as Sigma.
 * @brief - Finite set of stack symbols (stack alphabet). Formally defined as Gamma.
 * @brief - Stack with unbounded memory. With initial stack symbol Z.
 * @brief - Start state key. Formally defined as q0.
 * @brief - Finite set of accept states.
 * @brief - Current state.
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
