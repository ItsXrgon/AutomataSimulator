#pragma once
#include "../FiniteAutomaton.h"
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
	/**
	 * @brief Sets the input alphabet.
	 * @param inputAlphabet The value to load into the tape.
	 * @param strict If true, will throw an exception if the alphabet symbols are used in transitions.
	 * @throws InvalidAlphabetException If the alphabet is invalid.
	 */
	void setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict = true);

	/**
	 * @brief Adds to the input alphabet.
	 * @param inputAlphabet The value to load into the tape.
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
	 * @brief Moves the automaton to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the current or start state are not set.
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
