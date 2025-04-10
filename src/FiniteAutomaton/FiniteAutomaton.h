#pragma once
#pragma once
#include "../AutomatonException/AutomatonException.h"
#include "FAState/FAState.h"
#include "FATransition/FATransition.h"
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a finite automaton.
 */
class AUTOMATASIMULATOR_API FiniteAutomaton {
  protected:
	/**
	 * @brief The input of the automaton.
	 */
	std::vector<std::string> input;

	/**
	 * @brief The head of the input tape.
	 */
	int inputHead;

	/**
	 * @brief The current state of the automaton.
	 */
	std::string currentState;

	/**
	 * @brief States of the automaton.
	 */
	std::unordered_map<std::string, FAState> states;

	/**
	 * @brief Input alphabet of the automaton.
	 */
	std::unordered_set<std::string> inputAlphabet;

	/**
	 * @brief Start state key.
	 */
	std::string startState;

	/**
	 * @brief Cached convertion of the states from unordered map to vector.
	 */
	std::vector<FAState> cachedStates;

	/**
	 * @brief Boolean indicating whether the cached states are invalidated or not.
	 */
	bool statesCacheInvalidated;

	/**
	 * @brief Cached convertion of the input alphabet from unordered map to vector.
	 */
	std::vector<std::string> cachedInputAlphabet;

	/**
	 * @brief Boolean indicating whether the cached input alphabet is invalidated or not.
	 */
	bool inputAlphabetCacheInvalidated;

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key.
	 */
	FAState *getStateInternal(const std::string &key);

	/**
	 * @brief Checks if the transition states exist.
	 * @param fromKey The key of the state to transition from.
	 * @param toKey The key of the state to transition to.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the to or form states are not found.
	 */
	void validateTransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input);

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
	 * @brief Gets the input of the automaton.
	 * @return The input of the automaton.
	 */
	std::vector<std::string> getInput() const;

	/**
	 * @brief Sets the input of the automaton.
	 * @param input The value to load into the tape.
	 * @throws InvalidAlphabetException If the input contains symbols not in the alphabet.
	 */
	virtual void setInput(const std::vector<std::string> &input);

	/**
	 * @brief Adds to the input of the automaton.
	 * @param input The value to load into the tape.
	 * @throws InvalidAlphabetException If the input contains symbols not in the alphabet.
	 */
	virtual void addInput(const std::vector<std::string> &input);

	/**
	 * @brief Gets the current input head of the automaton.
	 * @return The current input head of the automaton.
	 */
	int getInputHead() const;

	/**
	 * @brief Sets the current input head of the automaton.
	 * @param head The new input head.
	 */
	void setInputHead(const int &head);

	/**
	 * @brief Check if the state exists
	 * @param key The key of the state.
	 * @return Bool indicating whether the state exists or not.
	 */
	bool stateExists(const std::string &key) const;

	/**
	 * @brief Check if the input alphabet symbol exists
	 * @param key The input alphabet symbol.
	 * @return Bool indicating whether the symbol exists or not.
	 */
	bool inputAlphabetSymbolExists(const std::string &symbol) const;

	/**
	 * @brief Adds a state to the automaton.
	 * @param label The label of the state.
	 * @param isAccept Whether the state is an accept state or not.
	 */
	void addState(const std::string &label, const bool &isAccept = false);

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
	 * @brief Gets the current state of the automaton, defaulting to the start state if not set.
	 * @return The current state of the automaton.
	 * @throws StateNotFoundException If the current state and start state are not set.
	 */
	std::string getCurrentState() const;

	/**
	 * @brief Sets the current state of the automaton.
	 * @param state The key of the state to set.
	 * @throws StateNotFoundException If the state is not found.
	 */
	virtual void setCurrentState(const std::string &state);

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key as
	 * @throws StateNotFoundException If the state is not found.
	 */
	FAState getState(const std::string &key) const;

	/**
	 * @brief Gets the states of the automaton.
	 * @return The states of the automaton.
	 */
	std::vector<FAState> getStates();

	/**
	 * @brief Removes a state from the automaton.
	 * @param key The key of the state to remove.
	 * @param strict If true, will throw an exception if the state is used in transitions.
	 * @throws StateNotFoundException If the state is not found.
	 */
	virtual void removeState(const std::string &key, const bool &strict = true);

	/**
	 * @brief Removes states from the automaton.
	 * @brief If a state is not found, it will be ignored.
	 * @param key The keys of the states to remove.
	 * @param strict If true, will throw an exception if the states are used in transitions.
	 * @throws StateNotFoundException If one of the states are not found.
	 */
	virtual void removeStates(const std::vector<std::string> &keys, const bool &strict = true);

	/**
	 * @brief Clears the states of the automaton.
	 */
	virtual void clearStates();

	/**
	 * @brief Sets the input alphabet.
	 * @param inputAlphabet The value to load into the tape.
	 * @param strict If true, will throw an exception if the old symbols are used in transitions.
	 */
	void setInputAlphabet(const std::vector<std::string> &inputAlphabet, const bool &strict = true);

	/**
	 * @brief Adds to the input alphabet.
	 * @param inputAlphabet The value to load into the tape.
	 */
	void addInputAlphabet(const std::vector<std::string> &inputAlphabet);

	/**
	 * @brief Gets the input alphabet of the automaton.
	 * @return The alphabet of the automaton.
	 */
	std::vector<std::string> getInputAlphabet();

	/**
	 * @brief Remove a symbol from the input alphabet
	 * @param symbol The symbol to remove
	 * @param strict If true, will throw an exception if the alphabet symbol is used in transitions.
	 * @throws InputAlphabetSymbolNotFoundException If the alphabet symbol is not found.
	 */
	void removeInputAlphabetSymbol(const std::string &symbol, const bool &strict = true);

	/**
	 * @brief Remove symbols from the input alphabet
	 * @param symbols The symbols to remove
	 * @param strict If true, will throw an exception if the alphabet symbols are used in transitions.
	 * @throws InputAlphabetSymbolNotFoundException If any of the alphabet symbols are not found.
	 */
	void removeInputAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict = true);

	/**
	 * @brief Clears the input alphabet of the automaton.
	 * @param strict If true, will throw an exception if the alphabet symbols are used in transitions.
	 */
	void clearInputAlphabet(const bool &strict = true);

	/**
	 * @brief Gets the start state key of the automaton.
	 * @return The start state key of the automaton.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	std::string getStartState() const;

	/**
	 * @brief Sets the start state key of the automaton.
	 * @param key The state key.
	 * @throws StateNotFoundException If the state is not in the automaton.
	 */
	virtual void setStartState(const std::string &key);

	/**
	 * @brief Add a transition to the automaton.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the to or from states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	virtual void addTransition(const std::string &fromStateKey, const std::string &toStateKey,
	                           const std::string &input);
	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws TransitionNotFoundException If the transition violates automaton constraints.
	 */
	virtual void updateTransitionInput(const std::string &transitionKey, const std::string &input);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param fromStateKey The key of the from state.
	 * @throws StateNotFoundException If the from state is not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	virtual void updateTransitionFromState(const std::string &transitionKey, const std::string &fromStateKey);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param toStateKey The key of the to state.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void updateTransitionToState(const std::string &transitionKey, const std::string &toStateKey);

	/**
	 * @brief Remove a transition between 2 states.
	 * @param transitionKey The key of the transition to remove.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 */
	void removeTransition(const std::string &transitionKey);

	/**
	 * @brief Clears transitions between 2 states from the automaton.
	 * @param fromStateKey The key of the state to remove from.
	 * @param toStateKey The key of the state the transition points to.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 */
	void clearTransitionsBetween(const std::string &fromStateKey, const std::string &toStateKey);

	/**
	 * @brief Clears transition of a state.
	 * @param key The key of the state.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void clearStateTransitions(const std::string &stateKey);

	/**
	 * @brief Clears all transitions of the automaton.
	 */
	void clearTransitions();

	/**
	 * @brief Marks a state as an accept state in the automaton.
	 * @param key The key of the state to mark as an accept state.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void addAcceptState(const std::string &stateKey);

	/**
	 * @brief Marks states as accept states in the automaton.
	 * @param keys The keys of the states to mark as an accept state.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void addAcceptStates(const std::vector<std::string> &keys);

	/**
	 * @brief Removes a state from the accept states of the automaton.
	 * @param key The key of the state to remove from the accept states.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void removeAcceptState(const std::string &stateKey);

	/**
	 * @brief Removes states from the accept states of the automaton.
	 * @param keys The keys of the states to mark as an accept state.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void removeAcceptStates(const std::vector<std::string> &keys);

	/**
	 * @brief Clears the accept states of the automaton.
	 */
	void clearAcceptStates();	

	/**
	 * @brief Gets the accept states of the automaton.
	 * @return The accept states of the automaton.
	 */
	std::vector<FAState> getAcceptStates() const;

	/**
	 * @brief Resets the automaton state and head.
	 */
	virtual void reset();

	/**
	 * @brief Returns whether the automata is currenty in an accept state or not.
	 * @return True if the automaton is in an accept state.
	 */
	bool isAccepting() const;

	/**
	 * @brief Moves the automaton to the next state based on the current input head.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 * @throws InputConsumedException If the input head exceeds the length of the input.
	 */
	virtual bool processInput() = 0;

	/**
	 * @brief Simulates the automaton on a given input string and depth.
	 * @param input The input strings to process.
	 * @param simulationDepth The maximum number of transitions to simulate. Default is 50.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 */
	virtual bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) = 0;
};