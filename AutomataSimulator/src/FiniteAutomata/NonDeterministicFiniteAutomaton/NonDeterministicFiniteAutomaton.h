#pragma once
#include "../FiniteAutomaton.h"
#include "../State/State.h"
#include "../Transition/Transition.h"
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a non-deterministic finite automaton.
 * @brief A deterministic finite automaton is defined by a
 * @brief - Finite set of states. Formally defined as Q. including their transitions.
 * @brief - Finite set of input symbols (alphabet). Formally defined as Sigma.
 * @brief - Start state key. Formally defined as q0.
 * @brief - Finite set of accept states.
 * @brief - Current state.
 * @brief - Possible current states (for non-deterministic transitions).
 */
class AUTOMATASIMULATOR_API NonDeterministicFiniteAutomaton : public FiniteAutomaton {
  private:
	/**
	 * @brief The possible current states of the automaton.
	 */
	std::unordered_set<std::string> possibleCurrentStates;

	/**
	 * @brief Adds epsilon closures to the set of states.
	 */
	void addEpsilonClosure(std::unordered_set<std::string> &states);

	/**
	 * @brief Decides on a random state to transition to.
	 */
	std::string decideRandomState(const std::unordered_set<std::string> &states);

  public:
	/**
	 * @brief Sets the alphabet of the NFA automaton.
	 * @param alphabet The strings to add.
	 */
	void setAlphabet(const std::set<std::string> &alphabet) override;

	/**
	 * @brief Adds strings to the alphabet of the NFA automaton.
	 * @param alphabet The set of strings to add.
	 */
	void addAlphabet(const std::set<std::string> &alphabet) override;

	/**
	 * @brief Add a transition between 2 states to the automaton.
	 * @param fromStateKey The key of the state to transition from.
	 * @param input The input of the transition.
	 * @param toStateKey The key of the state to transition to.
	 */
	void addTransitionBetween(const std::string &fromStateKey, const std::string &input,
	                          const std::string &toStateKey) override;

	/**
	 * @brief Reset the NFA to the start state.
	 */
	void reset() override;

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
