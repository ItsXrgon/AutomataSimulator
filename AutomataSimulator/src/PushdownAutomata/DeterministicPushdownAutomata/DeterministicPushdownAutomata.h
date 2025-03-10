#pragma once
#include "../PushdownAutomata.h"
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
 * A deterministic finite automaton is defined by a finite set of states, a finite set of input symbols, a start state,
 * and a finite set of accept states.
 */
class AUTOMATASIMULATOR_API DeterministicPushdownAutomata : public PushdownAutomata {
  private:
	/**
	 * @brief The current state of the automaton.
	 */
	std::string currentState;

	/**
	 * @brief Checks the validity of an alphabet
	 * @return True if alphabet is valid
	 */
	bool checkAlphabetValidity(const std::set<std::string> &alphabet) const;

  public:
	/**
	 * @brief Sets the current state of the automaton.
	 * @param state The key of the state to set.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void setCurrentState(const std::string &state);

	/**
	 * @brief Gets the current state of the automaton.
	 * @return The current state of the automaton or start state if not set.
	 * @throws StateNotFoundException If the state is not found.
	 */
	std::string getCurrentState() const;

	/**
	 * @brief Sets the alphabet of the DFA automaton.
	 * @param alphabet The strings to add.
	 * @throws InvalidAlphabetException If the alphabet contains an epsilon.
	 */
	void setAlphabet(const std::set<std::string> &alphabet) override;

	/**
	 * @brief Adds strings to the alphabet of the DFA automaton.
	 * @param alphabet The set of strings to add.
	 * @throws InvalidAlphabetException If the alphabet contains an epsilon.
	 */
	void addAlphabet(const std::set<std::string> &alphabet) override;

	/**
	 * @brief Add a transition between 2 states to the DFA automaton.
	 * @param fromKey The key of the state to transition from.
	 * @param input The input of the transition.
	 * @param toKey The key of the state to transition to.
	 * @throws StateNotFoundException If the to or form states are not found.
	 * @throws InvalidTransitionException If the transition violates DFA constraints.
	 */
	void addTransitionBetween(const std::string &fromKey, const std::string &input, const std::string &toKey);

	/**
	 * @brief Reset the DFA to the start state.
	 */
	void reset();

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
