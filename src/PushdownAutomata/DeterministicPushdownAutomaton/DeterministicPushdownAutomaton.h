#pragma once
#include "../PushdownAutomaton.h"
#include <set>
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
class AUTOMATASIMULATOR_API DeterministicPushdownAutomaton : public PushdownAutomaton {
  private:
	/**
	 * @brief The current state of the automaton.
	 */
	std::string currentState;

  public:
	/**
	 * @brief Add a transition between 2 states to the DFA automaton.
	 * @param fromKey The key of the state to transition from.
	 * @param input The input of the transition.
	 * @param toKey The key of the state to transition to.
	 * @throws StateNotFoundException If the to or form states are not found.
	 * @throws InvalidTransitionException If the transition violates DFA constraints.
	 */
	void addTransitionBetween(const std::string &fromStateKey, const std::string &input, const std::string &toStateKey,
	                          const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @brief Moves the DFA to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	bool processInput(const std::string &input);

	/**
	 * @brief Simulates the DFA on a given input string.
	 * @param input The input string to process.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	bool simulate(const std::vector<std::string> &input) override;
};
