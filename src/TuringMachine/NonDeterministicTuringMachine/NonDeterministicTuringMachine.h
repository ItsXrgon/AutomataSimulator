#pragma once
#include "../TuringMachine.h"
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
class AUTOMATASIMULATOR_API NonDeterministicTuringMachine : public TuringMachine {
  private:
	/**
	 * @brief Adds all states reachable by epsilon transitions to the set of states.
	 * @param states The set of states to add epsilon transitions to.
	 */
	void addEpsilonClosure(std::unordered_set<std::string> &states);

	/**
	 * @brief Gets all next states for a set of states and an input symbol.
	 * @param currentStates The set of current states.
	 * @param input The input symbol.
	 * @param readSymbol The symbol read from the tape.
	 * @return The set of next states.
	 */
	std::unordered_set<std::string> getNextPossibleStates(const std::unordered_set<std::string> &currentStates,
	                                                      const std::string &input, const std::string &readSymbol);

	/**
	 * @brief Decides a random state from a set of states.
	 * @param states The set of states to decide from.
	 * @return A random state from the set.
	 */
	std::string decideRandomState(const std::unordered_set<std::string> &states);

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
	 * @brief Simulates the automaton on a given input string and depth.
	 * @param input The input strings to process.
	 * @param simulationDepth The maximum number of transitions to simulate. Default is 50.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws SimulationDepthExceededException If the simulation depth is exceeded.
	 */
	bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) override;
};
