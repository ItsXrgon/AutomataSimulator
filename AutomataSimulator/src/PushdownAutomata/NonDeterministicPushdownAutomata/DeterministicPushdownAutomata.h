#pragma once
#include "../PushdownAutomata.h"
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
 * A non-deterministic finite automaton is defined by a finite set of states, a finite set of input symbols, a start
 * state, and a finite set of accept states.
 */
class AUTOMATASIMULATOR_API NonDeterministicPushdownAutomata : public PushdownAutomata {
  private:
	void addEpsilonClosure(std::unordered_set<std::string> &states);
	std::unordered_set<std::string> possibleCurrentStates;

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
	 * @brief Add a transition between 2 states to the NFA automaton.
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
};
