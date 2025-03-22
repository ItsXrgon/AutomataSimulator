#pragma once
#include "../AutomatonException/AutomatonException.h"
#include "TMState/TMState.h"
#include "TMTape/TMTape.h"
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Default blank symbol.
 */
extern const std::string &DEFAULT_BLANK_SYMBOL;

class AUTOMATASIMULATOR_API TuringMachine {
  protected:
	/**
	 * @brief The input of the automaton.
	 */
	std::vector<std::string> input;

	/**
	 * @brief The current state of the automaton.
	 */
	std::string currentState;

	/**
	 * @brief Finite set of states.
	 */
	std::unordered_map<std::string, TMState> states;

	/**
	 * @brief Finite set of input symbols. Formally defined as Sigma.
	 */
	std::unordered_set<std::string> inputAlphabet;

	/**
	 * @brief Finite set of stack symbols. Formally defined as Gamma.
	 */
	std::unordered_set<std::string> tapeAlphabet;

	/**
	 * @brief Start state key. Formally defined as q0.
	 */
	std::string startState;

	/**
	 * @brief Tape for the turing machine automaton.
	 */
	TMTape tape;

	/**
	 * @brief Cached convertion of the states from unordered map to vector.
	 */
	std::vector<TMState> cachedStates;

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
	std::vector<std::string> cachedTapeAlphabet;

	/**
	 * @brief Boolean indicating whether the cached stack alphabet is invalidated or not.
	 */
	bool tapeAlphabetCacheInvalidated;

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key.
	 */
	TMState *getStateInternal(const std::string &key);

	/**
	 * @brief Checks if the transition is valid and throws an exception if it is not.
	 * @param fromStateKey The key of the state to transition from.
	 * @param toStateKey The key of the state to transition to.
	 * @param readSymbol The symbol to read from the tape.
	 * @param writeSymbol The symbol to write onto the tape.
	 * @param direction The direction of the transition
	 * @throws StateNotFoundException If the to or form states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void validateTransition(const std::string &fromStateKey, const std::string &toStateKey,
	                        const std::string &readSymbol, const std::string &writeSymbol, TMDirection direction);

  public:
	/**
	 * @brief Constructs a new TuringMachine Automaton object.
	 */
	TuringMachine(std::string blankSymbol = DEFAULT_BLANK_SYMBOL);

	/**
	 * @brief Destructor for the TuringMachine Automaton object.
	 */
	virtual ~TuringMachine();

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
	bool tapeAlphabetSymbolExists(const std::string &symbol) const;

	/**
	 * @brief Gets the input of the automaton.
	 * @return The input of the automaton.
	 */
	std::vector<std::string> getInput() const;

	/**
	 * @brief Sets the input of the automaton.
	 * @param input The value to load into the tape.
	 */
	void setInput(const std::vector<std::string> &input);

	/**
	 * @brief Adds to the input of the automaton.
	 * @param input The value to load into the tape.
	 */
	void addInput(const std::vector<std::string> &input);

	/**
	 * @brief Sets the tape.
	 * @brief This resets the tape and tape head.
	 * @param tape The tape as an int - char map.
	 */
	void setTape(std::map<int, std::string> &tape);

	/**
	 * @brief Gets the tape.
	 * @return The tape as char vector.
	 */
	std::map<int, std::string> getTape() const;

	/**
	 * @brief Adds a state to the automaton.
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
	TMState getState(const std::string &key) const;

	/**
	 * @brief Gets the states of the automaton.
	 * @return The states of the automaton.
	 */
	std::vector<TMState> getStates();

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
	 * @param tapeAlphabet The value to load into the tape.
	 */
	void setTapeAlphabet(const std::vector<std::string> &tapeAlphabet, const bool &strict = true);

	/**
	 * @brief Adds to the stack alphabet.
	 * @param inputAlphabet The value to load into the tape.
	 */
	void addTapeAlphabet(const std::vector<std::string> &tapeAlphabet);

	/**
	 * @brief Gets the stack alphabet of the automaton.
	 * @return The alphabet of the automaton.
	 */
	std::vector<std::string> getTapeAlphabet();

	/**
	 * @brief Remove a symbol from the stack alphabet
	 * @param symbol The symbol to remove
	 * @throws InputAlphabetSymbolNotFoundException If the alphabet symbol is not found.
	 */
	void removeTapeAlphabetSymbol(const std::string &symbol, const bool &strict = true);

	/**
	 * @brief Remove symbols from the stack alphabet
	 * @param symbols The symbols to remove
	 * @throws InputAlphabetSymbolNotFoundException If any of the alphabet symbols are not found.
	 */
	void removeTapeAlphabetSymbols(const std::vector<std::string> &symbols, const bool &strict = true);

	/**
	 * @brief Clears the stack alphabet of the automaton.
	 */
	void clearTapeAlphabet(const bool &strict = true);

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
	 * @param readSymbol The symbol to read from the tape.
	 * @param writeSymbol The symbol to write onto the tape.
	 * @param direction The direction of the transition
	 * @throws StateNotFoundException If the from or to states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	virtual void addTransition(const std::string &fromStateKey, const std::string &toStateKey,
	                           const std::string &readSymbol, const std::string &writeSymbol, TMDirection direction);

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
	 * @brief Updates the read symbol of a transition.
	 * @param transitionKey The key of the transition.
	 * @param readSymbol The read symbol of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	virtual void updateTransitionReadSymbol(const std::string &transitionKey, const std::string &readSymbol);

	/**
	 * @brief Updates the write symbol of a transition.
	 * @param transitionKey The key of the transition.
	 * @param WriteSymbol The write symbol of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void updateTransitionWriteSymbol(const std::string &transitionKey, const std::string &WriteSymbol);

	/**
	 * @brief Updates the direction symbol of a transition.
	 * @param transitionKey The key of the transition.
	 * @param direction The direction of the transition.
	 * @throws StateNotFoundException If the from or to states from the transition key are not found.
	 * @throws TransitionNotFoundException If the transition with the input from the key is not found.
	 */
	void updateTransitionDirection(const std::string &transitionKey, const TMDirection &direction);

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
	std::vector<TMState> getAcceptStates() const;

	/**
	 * @brief Resets the automaton state, tape and head.
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
	 * @throws SimulationDepthExceededException If the simulation depth is exceeded.
	 */
	virtual bool simulate(const std::vector<std::string> &input, const int &simulationDepth = 50) = 0;
};
