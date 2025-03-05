#pragma once
#include "../FiniteAutomaton.h"
#include "../State/State.h"
#include "../Transition/Transition.h"
#include <set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a deterministic finite automaton.
 * @brief A deterministic finite automaton is defined by a
 * @brief - Finite set of states. Formally defined as Q. including their transitions.
 * @brief - Finite set of input symbols (alphabet). Formally defined as Sigma.
 * @brief - Start state key. Formally defined as q0.
 * @brief - Finite set of accept states.
 * @brief - Current state.
 */
class AUTOMATASIMULATOR_API DeterministicFiniteAutomaton : public FiniteAutomaton {
  private:
	/**
	 * @brief Checks the validity of an alphabet
	 * @return True if alphabet is valid
	 */
	bool checkAlphabetValidity(const std::set<std::string> &alphabet) const;

  public:
	/**
	 * @brief Sets the alphabet of the automaton.
	 * @param alphabet The strings to add.
	 * @throws InvalidAlphabetException If the alphabet contains an epsilon (empty string).
	 */
	void setAlphabet(const std::set<std::string> &alphabet) override;

	/**
	 * @brief Adds strings to the alphabet of the automaton.
	 * @param alphabet The set of strings to add.
	 * @throws InvalidAlphabetException If the alphabet contains an epsilon (empty string).
	 */
	void addAlphabet(const std::set<std::string> &alphabet) override;

	/**
	 * @brief Add a transition between 2 states to the automaton.
	 * @param fromStateKey The key of the state to transition from.
	 * @param input The input of the transition.
	 * @param toStateKey The key of the state to transition to.
	 * @throws StateNotFoundException If the to or form states are not found.
	 * @throws InvalidTransitionException If the transition is non-deterministic.
	 */
	void addTransitionBetween(const std::string &fromStateKey, const std::string &input,
	                          const std::string &toStateKey) override;

	/**
	 * @brief Reset the DFA to the start state.
	 */
	void reset();

	/**
	 * @brief Moves the automaton to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	bool processInput(const std::string &input);

	/**
	 * @brief Simulates the automaton on a given input strings.
	 * @param input The input strings to process.
	 * @return True if the inputs are accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	bool simulate(const std::vector<std::string> &input) override;
};
