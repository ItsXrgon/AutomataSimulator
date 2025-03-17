#pragma once
#include "PDAState/PDAState.h"
#include "PDATransition/PDATransition.h"
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Initial stack symbol.
 */
extern const std::string INITIAL_STACK_SYMBOL;

/**
 * @brief Represents a pushdown automaton.
 */
class AUTOMATASIMULATOR_API PushdownAutomaton {
  protected:
	/**
	 * @brief The current state of the automaton.
	 */
	std::string currentState;

	/**
	 * @brief Finite set of states.
	 */
	std::unordered_map<std::string, PDAState> states;

	/**
	 * @brief Finite set of input symbols. Formally defined as Sigma.
	 */
	std::unordered_set<std::string> inputAlphabet;

	/**
	 * @brief Finite set of stack symbols. Formally defined as Gamma.
	 */
	std::unordered_set<std::string> stackAlphabet;

	/**
	 * @brief Start state key. Formally defined as q0.
	 */
	std::string startState;

	/**
	 * @brief Stack for the pushdown automaton.
	 */
	std::stack<std::string> stack;

	/**
	 * @brief Cached convertion of the states from unordered map to vector.
	 */
	std::vector<PDAState> cachedStates;

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
	 * @brief Cached convertion of the stack alphabet from unordered map to vector.
	 */
	std::vector<std::string> cachedStackAlphabet;

	/**
	 * @brief Boolean indicating whether the cached stack alphabet is invalidated or not.
	 */
	bool stackAlphabetCacheInvalidated;

	
	/**
	 * @brief Splits the push symbols string
	 * @param pushSymbols Push symbols string
	 * @return Separated symbols
	*/
	std::vector<std::string> parsePushSymbols(const std::string &pushSymbols);

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key.
	 */
	PDAState *getStateInternal(const std::string &key);

	/**
	 * @brief Checks if the transition is valid and throws an exception if it is not.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @throws StateNotFoundException If the to or form states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void validateTransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
	                        const std::string &stackSymol, const std::string &pushSymbol);

  public:
	/**
	 * @brief Constructs a new Pushdown Automaton object.
	 */
	PushdownAutomaton();

	/**
	 * @brief Destructor for the Pushdown Automaton object.
	 */
	virtual ~PushdownAutomaton();

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
	 * @brief Check if the stack alphabet symbol exists
	 * @param key The stack alphabet symbol.
	 * @return Bool indicating whether the symbol exists or not.
	 */
	bool stackAlphabetSymbolExists(const std::string &symbol) const;

	/**
	 * @brief Gets the stack.
	 * @return The stack
	 */
	std::stack<std::string> getStack();

	/**
	 * @brief Set the stack.
	 * @param stack The stack
	 */
	void setStack(std::stack<std::string> stack);

	/**
	 * @brief Adds a state to the automaton.
	 * @param label The label for the state.
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
	 * @brief Gets the current state of the automaton.
	 * @return The current state of the automaton.
	 */
	std::string getCurrentState() const;

	/**
	 * @brief Sets the current state of the automaton.
	 * @param state The key of the state to set.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void setCurrentState(const std::string &state);

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key as
	 * @throws StateNotFoundException If the state is not found.
	 */
	PDAState getState(const std::string &key) const;

	/**
	 * @brief Gets the states of the automaton.
	 * @return The states of the automaton.
	 */
	std::vector<PDAState> getStates();

	/**
	 * @brief Removes a state from the automaton.
	 * @param key The key of the state to remove.
	 * @param strict If true, will throw an exception if the state is used in transitions.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void removeState(const std::string &key, const bool &strict = true);

	/**
	 * @brief Removes states from the automaton.
	 * @brief If a state is not found, it will be ignored.
	 * @param key The keys of the states to remove.
	 * @param strict If true, will throw an exception if the states are used in transitions.
	 * @throws StateNotFoundException If one of the states are not found.
	 */
	void removeStates(const std::vector<std::string> &keys, const bool &strict = true);

	/**
	 * @brief Clears the states of the automaton.
	 */
	void clearStates();

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
	 * @brief Gets the alphabet of the automaton.
	 * @return The alphabet of the automaton.
	 */
	std::vector<std::string> getInputAlphabet();

	/**
	 * @brief Remove a symbol from the alphabet
	 * @param symbol The symbol to remove
	 * @param strict If true, will throw an exception if the symbol is used in transitions.
	 * @throws InputAlphabetSymbolNotFoundException If the alphabet symbol is not found.
	 */
	void removeInputAlphabetSymbol(const std::string &symbol, const bool &strict = true);

	/**
	 * @brief Remove symbols from the alphabet
	 * @param symbols The symbols to remove
	 * @param strict If true, will throw an exception if the symbols are used in transitions.
	 * @throws InputAlphabetSymbolNotFoundException If any of the alphabet symbols are not found.
	 */
	void removeInputAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict = true);

	/**
	 * @brief Clears the input alphabet of the automaton.
	 * @param strict If true, will throw an exception if the symbols are used in transitions.
	 */
	void clearInputAlphabet(const bool &strict = true);

	/**
	 * @brief Sets the stack alphabet.
	 * @param stackAlphabet The value to load into the tape.
	 * @param strict If true, will throw an exception if the old symbols are used in transitions.
	 */
	void setStackAlphabet(const std::vector<std::string> &stackAlphabet, const bool &strict = true);

	/**
	 * @brief Adds to the stack alphabet.
	 * @param inputAlphabet The value to load into the tape.
	 */
	void addStackAlphabet(const std::vector<std::string> &stackAlphabet);

	/**
	 * @brief Gets the stack alphabet of the automaton.
	 * @return The alphabet of the automaton.
	 */
	std::vector<std::string> getStackAlphabet();

	/**
	 * @brief Remove a symbol from the stack alphabet
	 * @param symbol The symbol to remove
	 * @param strict If true, will throw an exception if the symbol is used in transitions.
	 * @throws InputAlphabetSymbolNotFoundException If the alphabet symbol is not found.
	 */
	void removeStackAlphabetSymbol(const std::string &symbol, const bool &strict = true);

	/**
	 * @brief Remove symbols from the stack alphabet
	 * @param symbols The symbols to remove
	 * @param strict If true, will throw an exception if the symbols are used in transitions.
	 * @throws InputAlphabetSymbolNotFoundException If any of the alphabet symbols are not found.
	 */
	void removeStackAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict = true);

	/**
	 * @brief Clears the stack alphabet of the automaton.
	 * @param strict If true, will throw an exception if the symbols are used in transitions.
	 */
	void clearStackAlphabet(const bool &strict = true);

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
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param input The input of the transition.
	 * @param stackSymbol The top of the stack symbol.
	 * @param pushSymbol The symbol to be pushed onto the stack.
	 * @throws StateNotFoundException If the from or to states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 * @throws InvalidAutomatonDefinitionException If the transition is non deterministic
	 */
	virtual void addTransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
	                           const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @brief Updates the input of a transition.
	 * @param transitionKey The key of the transition.
	 * @param input The input of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
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
	 * @brief Updates the stack symbol of a transition.
	 * @param transitionKey The key of the transition.
	 * @param stackSymbol The stack symbol of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	virtual void updateTransitionStackSymbol(const std::string &transitionKey, const std::string &stackSymbol);

	/**
	 * @brief Updates the push symbol of a transition.
	 * @param transitionKey The key of the transition.
	 * @param pushSymbol The push symbol of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void updateTransitionPushSymbol(const std::string &transitionKey, const std::string &pushSymbol);

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
	std::vector<PDAState> getAcceptStates() const;

	/**
	 * @brief Resets the automaton to its start state.
	 */
	void reset();

	/**
	 * @brief Moves the automaton to the next state based on the input.
	 * @param input The input string to process.
	 * @return True if the current state is accept.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	virtual bool processInput(const std::string &input) = 0;

	/**
	 * @brief Simulates the automaton on a given input string and depth.
	 * @param input The input strings to process.
	 * @param simulationDepth The maximum number of transitions to simulate. Default is 50.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws SimulationDepthExceededException If the simulation depth is exceeded.
	 */
	virtual bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) = 0;
};