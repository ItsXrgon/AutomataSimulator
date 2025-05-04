#pragma once
#include "PushdownAutomaton.h"
#include "config.h"
#include <set>
#include <vector>

/**
 * @brief Represents a deterministic pushdown automaton.
 */
class AUTOMATASIMULATOR_API DeterministicPushdownAutomaton : public PushdownAutomaton {
  private:
	/**
	 * @brief Checks the determinism of a transition.
	 * @return True if alphabet is valid.
	 * @throws StateNotFoundException If the form state is not found.
	 */
	bool checkTransitionDeterminisim(const std::string &fromStateKey, const std::string &input,
	                                 const std::string &stackSymbol);

  public:
	virtual ~DeterministicPushdownAutomaton() override;

	/**
	 * @brief Add a transition between 2 states to the automaton.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param input The input of the transition.
	 * @param stackSymbol The top of the stack symbol.
	 * @param pushSymbol The symbol to be pushed onto the stack.
	 * @throws StateNotFoundException If the from or to states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 * @throws InvalidAutomatonDefinitionException If the transition is non deterministic
	 */
	void addTransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
	                   const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidAutomatonDefinitionException If the transition violates automaton constraints.
	 */
	void updateTransitionInput(const std::string &transitionKey, const std::string &input);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param fromStateKey The key of the from state.
	 * @throws StateNotFoundException If the from state is not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void updateTransitionFromState(const std::string &transitionKey, const std::string &fromStateKey);

	/**
	 * @brief Updates the stack symbol of a transition.
	 * @param transitionKey The key of the transition.
	 * @param stackSymbol The stack symbol of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void updateTransitionStackSymbol(const std::string &transitionKey, const std::string &stackSymbol);

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
