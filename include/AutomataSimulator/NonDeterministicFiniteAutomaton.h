#pragma once
#include "FiniteAutomaton.h"
#include "config.h"
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>

/**
 * @brief Represents a non-deterministic finite automaton.
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
	 * @brief Decides a random transition from a set of transitions.
	 * @param transitions The set of transitions to decide from.
	 * @return A random transitions from the set.
	 */
	FATransition decideRandomTransition(const std::unordered_set<FATransition> &transitions);

  public:
	virtual ~NonDeterministicFiniteAutomaton() override;

	/**
	 * @brief Sets the input of the automaton.
	 * @param input The value to load into the tape.
	 * @throws InvalidAlphabetException If the input contains non-epsilon symbols not in the alphabet.
	 */
	void setInput(const std::vector<std::string> &input) override;

	/**
	 * @brief Adds to the input of the automaton.
	 * @param input The value to load into the tape.
	 * @throws InvalidAlphabetException If the input contains non-epsilon symbols not in the alphabet.
	 */
	void addInput(const std::vector<std::string> &input) override;

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
	 * @param strict If true, will throw an exception if the alphabet symbols are used in transitions.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void removeState(const std::string &key, const bool &strict = true);

	/**
	 * @brief Removes states from the automaton.
	 * @brief If a state is not found, it will be ignored.
	 * @param key The keys of the states to remove.
	 * @param strict If true, will throw an exception if the alphabet symbols are used in transitions.
	 * @throws StateNotFoundException If one of the states are not found.
	 */
	void removeStates(const std::vector<std::string> &keys, const bool &strict = true);

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
	const std::vector<std::string> getPossibleCurrentStates();

	/**
	 * @brief Reset the NFA to the start state.
	 */
	void reset() override;

	/**
	 * @brief Moves the automaton to the next state based on the current input head.
	 * @brief If an epsilon transition is taken, the input head is not incremented.
	 * @brief Returns false if the simulation depth is exceeded and no accept state is reached.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 * @throws InputConsumedException If the input head exceeds the length of the input.
	 */
	const bool processInput() override;

	/**
	 * @brief Simulates the NFA on a given input string.
	 * @param input The input string to process.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	const bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) override;
};
