#pragma once
#include "State/State.h"
#include "Transition/Transition.h"
#include <set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a finite automaton.
 * A finite automaton is defined by a finite set of states, a finite set of input symbols, a start state, and a finite
 * set of accept states.
 */
class AUTOMATASIMULATOR_API FiniteAutomaton {
  protected:
	/**
	 * @brief Finite set of states. Formally defined as Q.
	 */
	std::vector<State> states;

	/**
	 * @brief Finite set of input symbols. Formally defined as Sigma.
	 */
	std::set<std::string> alphabet;

	/**
	 * @brief Start state key. Formally defined as q0.
	 */
	std::string startState;

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key.
	 */
	State *getState(const std::string &key);

  public:
	/**
	 * @brief Constructs a new Finite Automaton object.
	 */
	FiniteAutomaton();

	/**
	 * @brief Destructor for the Finite Automaton object.
	 */
	virtual ~FiniteAutomaton();

	/**
	 * @brief Adds a state to the automaton.
	 * @param label The label for the state.
	 */
	void addState(const std::string &label);

	/**
	 * @brief Gets the states of the automaton.
	 * @return The states of the automaton.
	 */
	std::vector<State> getStates() const;

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key as
	 * @throws StateNotFoundException If the state is not found.
	 */
	State getState(const std::string &key) const;

	/**
	 * @brief Removes a state from the automaton.
	 * @param key The key of the state to remove.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void removeState(const std::string &key);

	/**
	 * @brief Sets the alphabet of the automaton.
	 * @param alphabet The strings to add.
	 * @throws InvalidAlphabetException If the alphabet is empty.
	 */
	virtual void setAlphabet(const std::set<std::string> &alphabet);

	/**
	 * @brief Gets the alphabet of the automaton.
	 * @return The alphabet of the automaton.
	 */
	std::set<std::string> getAlphabet() const;

	/**
	 * @brief Adds strings to the alphabet of the automaton.
	 * @param alphabet The set of strings to add.
	 * @throws InvalidAlphabetException If the alphabet to add is empty.
	 */
	virtual void addAlphabet(const std::set<std::string> &alphabet);

	/**
	 * @brief Sets the start state key of the automaton.
	 * @param key The state key.
	 * @throws InvalidStartStateException If the state is not in the automaton.
	 */
	void setStartState(const std::string &key);

	/**
	 * @brief Gets the start state key of the automaton.
	 * @return The start state key of the automaton.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	std::string getStartState() const;

	/**
	 * @brief Add a transition between 2 states to the automaton.
	 * @param fromKey The key of the state to transition from.
	 * @param input The input of the transition.
	 * @param toKey The key of the state to transition to.
	 */
	virtual void addTransitionBetween(const std::string &fromKey, const std::string &input, const std::string &toKey);

	/**
	 * @brief Remove a transition between 2 states.
	 * @param fromKey The key of the state to remove from.
	 * @param input The input of the transition.
	 * @param toKey The key of the state the transition points to.
	 */
	void removeTransitionBetween(const std::string &fromKey, const std::string &input, const std::string &toKey);

	/**
	 * @brief Clears transitions between 2 states from the automaton.
	 * @param fromKey The key of the state to remove from.
	 * @param toKey The key of the state the transition points to.
	 */
	void clearTransitionsBetween(const std::string &fromKey, const std::string &toKey);

	/**
	 * @brief Clears transition of a state.
	 * @param key The key of the state.
	 */
	void clearStateTransitions(const std::string &key);

	/**
	 * @brief Clears all transitions of the automaton.
	 */
	void clearTransitions();

	/**
	 * @brief Marks a state as an accept state in the automaton.
	 * @param key The key of the state to mark as an accept state.
	 */
	void addAcceptState(const std::string &key);

	/**
	 * @brief Removes a state from the accept states of the automaton.
	 * @param key The key of the state to remove from the accept states.
	 */
	void removeAcceptState(const std::string &key);

	/**
	 * @brief Clears the accept states of the automaton.
	 */
	void clearAcceptStates();

	/**
	 * @brief Gets the accept states of the automaton.
	 * @return The accept states of the automaton.
	 */
	std::vector<State> getAcceptStates() const;

	/**
	 * @brief Resets the automaton to its start state.
	 */
	virtual void reset() = 0;

	/**
	 * @brief Moves the automaton to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	virtual bool processInput(const std::string &input) = 0;

	/**
	 * @brief Simulates the automaton on a given input string.
	 * @param input The input strings to process.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	virtual bool simulate(const std::vector<std::string> &input) = 0;
};