#pragma once
#include "FiniteAutomaton.h"
#include "config.h"
#include <set>
#include <vector>

/**
 * @brief Represents a deterministic finite automaton.
 */
class AUTOMATASIMULATOR_API DeterministicFiniteAutomaton : public FiniteAutomaton {
  private:
	/**
	 * @brief Checks the validity of an alphabet.
	 * @throws InvalidTransitionException If alphabet contains an epsilon.
	 */
	void checkAlphabetValidity(const std::vector<std::string> &inputAlphabet) const;

	/**
	 * @brief Checks the determinism of a transition.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param input The input of the transition.
	 * @return True if transition is deterministic.
	 * @throws TransitionNotFoundException If the transition is not found.
	 */
	bool checkTransitionDeterminisim(const std::string &fromStateKey, const std::string &input);

  public:
	virtual ~DeterministicFiniteAutomaton() override;

	/**
	 * @brief Sets the input of the automaton.
	 * @param input The value to set.
	 * @throws InvalidAlphabetException If the input contains symbols not in the alphabet.
	 */
	void setInput(const std::vector<std::string> &input) override;

	/**
	 * @brief Adds to the input of the automaton.
	 * @param input The value to add.
	 * @throws InvalidAlphabetException If the input contains symbols not in the alphabet.
	 */
	void addInput(const std::vector<std::string> &input) override;

	/**
	 * @brief Sets the input alphabet.
	 * @param inputAlphabet The value to set.
	 * @param strict If true, will throw an exception if the alphabet symbols are used in transitions.
	 * @throws InvalidAlphabetException If the alphabet is invalid.
	 */
	void setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict = true);

	/**
	 * @brief Adds to the input alphabet.
	 * @param inputAlphabet The value to add.
	 * @throws InvalidAlphabetException If the alphabet is invalid.
	 */
	void addInputAlphabet(const std::vector<std::string> &inputAlphabet);

	/**
	 * @brief Add a transition to the automaton.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the to or from states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 * @throws InvalidAutomatonDefinitionException If the transition is non deterministic
	 */
	void addTransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws TransitionNotFoundException If the transition violates automaton constraints.
	 * @throws InvalidAutomatonDefinitionException If the transition is non deterministic
	 */
	void updateTransitionInput(const std::string &transitionKey, const std::string &input);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param fromStateKey The key of the from state.
	 * @throws StateNotFoundException If the from state is not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 * @throws InvalidAutomatonDefinitionException If the transition is non deterministic
	 */
	void updateTransitionFromState(const std::string &transitionKey, const std::string &fromStateKey);

	/**
	 * @brief Moves the automaton to the next state based on the current input head.
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
