#pragma once
#include "PDAState/PDAState.h"
#include "PDATransition/PDATransition.h"
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

extern const std::string INITIAL_STACK_SYMBOL;

/**
 * @brief Represents a pushdown automaton.
 * @brief A pushdown automaton is defined by a
 * @brief - Finite set of states. Formally defined as Q. including their transitions.
 * @brief - Finite set of input symbols (alphabet). Formally defined as Sigma.
 * @brief - Finite set of stack symbols (stack alphabet). Formally defined as Gamma.
 * @brief - Stack with unbounded memory. With initial stack symbol Z.
 * @brief - Start state key. Formally defined as q0.
 * @brief - Finite set of accept states.
 * @brief - Current state.
 */
class AUTOMATASIMULATOR_API PushdownAutomaton {
  protected:
	/**
	 * @brief The current state of the automaton.
	 */
	std::string currentState;

	/**
	 * @brief Finite set of states. Formally defined as Q.
	 */
	std::vector<PDAState> states;

	/**
	 * @brief Finite set of input symbols. Formally defined as Sigma.
	 */
	std::set<std::string> inputAlphabet;

	/**
	 * @brief Finite set of stack symbols. Formally defined as Gamma.
	 */
	std::set<std::string> stackAlphabet;

	/**
	 * @brief Start state key. Formally defined as q0.
	 */
	std::string startState;

	/**
	 * @brief Stack for the pushdown automaton.
	 */
	std::stack<std::string> stack;

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key.
	 */
	PDAState *getState(const std::string &key);

	/**
	 * @brief Checks the validity of a stack alphabet.
	 */
	void validateStackAlphabet(const std::set<std::string> &stackAlphabet) const;

	/**
	 * @brief Checks if the transition is valid and throws an exception if it is not.
	 * @param fromKey The key of the state to transition from.
	 * @param input The input of the transition.
	 * @param toKey The key of the state to transition to.
	 * @throws StateNotFoundException If the to or form states are not found.
	 * @throws InvalidTransitionException If the transition violates automaton constraints.
	 */
	void validateTransition(const std::string &fromStateKey, const std::string &input, const std::string &toStateKey,
	                        const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @Brief Adds the epsilon closure to the provided states set
	 * using the stack provided
	 */
	void addEpsilonClosure(std::unordered_set<std::string> &states, std::stack<std::string> stack);

	void applyEpsilonClosure(PDAState *&currentState, std::stack<std::string> &simulationStack);

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
	 * @brief Sets the current state of the automaton.
	 * @param state The key of the state to set.
	 * @throws StateNotFoundException If the state is not found.
	 */
	void setCurrentState(const std::string &state);

	/**
	 * @brief Gets the current state of the automaton.
	 * @return The current state of the automaton.
	 */
	std::string getCurrentState() const;

	/**
	 * @brief Adds a state to the automaton.
	 * @param label The label for the state.
	 */
	void addState(const std::string &label);

	/**
	 * @brief Gets the states of the automaton.
	 * @return The states of the automaton.
	 */
	std::vector<PDAState> getStates() const;

	/**
	 * @brief Gets the state with the key provided.
	 * @param key The key of the state to get.
	 * @return The state with the specified key as
	 * @throws StateNotFoundException If the state is not found.
	 */
	PDAState getState(const std::string &key) const;

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
	void setInputAlphabet(const std::set<std::string> &inputAlphabet);

	/**
	 * @brief Gets the alphabet of the automaton.
	 * @return The alphabet of the automaton.
	 */
	std::set<std::string> getInputAlphabet() const;

	/**
	 * @brief Adds strings to the alphabet of the automaton.
	 * @param alphabet The set of strings to add.
	 * @throws InvalidAlphabetException If the alphabet to add is empty.
	 */
	virtual void addInputAlphabet(const std::set<std::string> &inputAlphabet);

	/**
	 * @brief Sets the stack alphabet of the automaton.
	 * @param stackAlphabet The strings to add.
	 * @throws InvalidStackAlphabetException If the stack alphabet is empty.
	 */
	void setStackAlphabet(const std::set<std::string> &stackAlphabet);

	/**
	 * @brief Gets the stack alphabet of the automaton.
	 * @return The stack alphabet of the automaton.
	 */
	std::set<std::string> getStackAlphabet() const;

	/**
	 * @brief Adds strings to the stack alphabet of the automaton.
	 * @param stackAlphabet The set of strings to add.
	 * @throws InvalidStackAlphabetException If the stack alphabet to add is empty.
	 */
	void addStackAlphabet(const std::set<std::string> &stackAlphabet);

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
	 * @param stackSymbol The top of the stack symbol.
	 * @param pushSymbol The symbol to be pushed onto the stack.
	 */
	void addTransitionBetween(const std::string &fromStateKey, const std::string &input, const std::string &toStateKey,
	                          const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @brief Remove a transition between 2 states.
	 * @param fromKey The key of the state to remove from.
	 * @param input The input of the transition.
	 * @param toKey The key of the state the transition points to.
	 */
	void removeTransitionBetween(const std::string &fromStateKey, const std::string &input,
	                             const std::string &toStateKey, const std::string &stackSymbol,
	                             const std::string &pushSymbol);

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
	 * @brief Simulates the automaton on a given input string.
	 * @param input The input strings to process.
	 * @return True if the input is accepted, false otherwise.
	 * @throws InvalidStartStateException If the start state is not set.
	 * @throws InvalidAlphabetException If the alphabet is not set.
	 */
	virtual bool simulate(const std::vector<std::string> &input) = 0;
};