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
	std::unordered_set<std::string> possibleCurrentStates;

  public:
	/**
	 * @brief Moves the NFA to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	bool processInput(const std::string &input) override;

	/**
	 * @brief Simulates the NFA on a given input string.
	 * @param input The input string to process.
	 * @return True if the input is accepted, false otherwise.
	 */
	bool simulate(const std::vector<std::string> &input) override;
};
