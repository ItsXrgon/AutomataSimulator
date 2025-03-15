#pragma once
#include "../FiniteAutomaton.h"
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
	 * @brief Cached convertion of the  possible current states from unordered map to vector.
	 */
	std::vector<std::string> cachedPossibleCurrentStates;

	/**
	 * @brief Boolean indicating whether the cached  possible current states is invalidated or not.
	 */
	bool possibleCurrentStatesCacheInvalidated;

	/**
	 * @brief Adds all states reachable by epsilon transitions to the set of states.
	 * @param states The set of states to add epsilon transitions to.
	 */
	void addEpsilonClosure(std::unordered_set<std::string> &states);

	/**
	 * @brief Gets all next states for a set of states and an input symbol.
	 * @param currentStates The set of current states.
	 * @param input The input symbol.
	 * @return The set of next states.
	 */
	std::unordered_set<std::string> getNextPossibleStates(const std::unordered_set<std::string> &currentStates,
	                                                      const std::string &input);

	/**
	 * @brief Decides a random state from a set of states.
	 * @param states The set of states to decide from.
	 * @return A random state from the set.
	 */
	std::string decideRandomState(const std::unordered_set<std::string> &states);

  public:
	/**
	 * @brief Updates the label of a state.
	 * @brief This also updates its key to be the new label and updates it's transitions' keys.
	 * @param key Key of the state to update.
	 * @param label Label to set for the state.
	 * @throws StateNotFoundException If the state is not found.
	 * @throws InvalidAutomatonDefinitionException If state with the same label (key) already exists
	 */
	void updateStateLabel(const std::string &key, const std::string &label);

	/**
	 * @brief Sets the current state of the automaton.
	 * @param state The key of the state to set.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void setCurrentState(const std::string &state);

	/**
	 * @brief Removes a state from the automaton.
	 * @param key The key of the state to remove.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void removeState(const std::string &key);

	/**
	 * @brief Removes states from the automaton.
	 * @brief If a state is not found, it will be ignored.
	 * @param key The keys of the states to remove.
	 * @throws StateNotFoundException If one of the states are not found.
	 */
	void removeStates(const std::vector<std::string> &keys);

	/**
	 * @brief Clears the states of the automaton.
	 */
	void clearStates();

	/**
	 * @brief Sets the start state key of the automaton.
	 * @param key The state key.
	 * @throws InvalidStartStateException If the state is not in the automaton.
	 */
	void setStartState(const std::string &key);

	/**
	 * @brief Add a transition between 2 states to the automaton.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param input The input of the transition.
	 */
	void addTransition(const std::string &fromStateKey, const std::string &toStateKey,
	                   const std::string &input) override;
	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws TransitionNotFoundException If the transition violates automaton constraints.
	 * @throws InvalidAutomatonDefinitionException If the transition is non deterministic
	 */
	virtual void updateTransitionInput(const std::string &transitionKey, const std::string &input);

	/**
	 * @brief Gets the possible current states
	 * @brief Possible current states are the states that the automata could of
	 * chosen in the last simulation based on the previous current state
	 * @return The possible current states of the automaton.
	 */
	std::vector<std::string> getPossibleCurrentStates();

	/**
	 * @brief Reset the NFA to the start state.
	 */
	void reset() override;

	/**
	 * @brief Moves the NFA to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	bool processInput(const std::string &input) override;

	/**
	 * @brief Simulates the NFA on a given input string.
	 * @param input The input string to process.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) override;
};
