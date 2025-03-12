#pragma once
#include "../PDATransition/PDATransition.h"
#include <string>
#include <unordered_map>
#include <vector>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a state in an automaton.
 * A state is defined by a "label" string, a vector of transitions that start from it, and a boolean indicating whether
 * it is an accept state.
 */
class AUTOMATASIMULATOR_API PDAState {
  private:
	/**
	 * @brief Unique identifier for the transition, formatted as "label".
	 */
	std::string key;

	/**
	 * @brief Label for the state.
	 */
	std::string label;

	/**
	 * @brief Transitions the start from the state.
	 */
	std::unordered_map<std::string, PDATransition> transitions;

	/**
	 * @brief Boolean indicating whether it is an accept state.
	 */
	bool isAccept;

	/**
	 * @brief Cached convertion of the transitions from unordered map to vector.
	 */
	std::vector<PDATransition> cachedTransitions;

	/**
	 * @brief Boolean indicating whether the cached states are invalidated or not.
	 */
	bool transitionsCacheInvalidated;

	/**
	 * @brief Gets the transition with the key provided.
	 * @param key The key of the transition to get.
	 * @return The transition with the specified key.
	 */
	PDATransition *getTransition(const std::string &key);

  public:
	/**
	 * @brief Constructs a new State object.
	 * @param label The label for the state.
	 * @param isAccept The boolean indicating whether it is an accept state.
	 */
	PDAState(const std::string &label, const bool &isAccept);

	/**
	 * @brief Copy constructor for the State object.
	 * @param other The State object to copy.
	 */
	PDAState(const PDAState &other);

	/**
	 * @brief Copy assignment operator for the State object.
	 * @param other The State object to copy.
	 * @return A reference to the copied State object.
	 */
	PDAState &operator=(const PDAState &other);

	/**
	 * @brief Move constructor for the State object.
	 * @param other The State object to move.
	 */
	PDAState(PDAState &&other) noexcept;

	/**
	 * @brief Move assignment operator for the State object.
	 * @param other The State object to move.
	 * @return A reference to the moved State object.
	 */
	PDAState &operator=(PDAState &&other) noexcept;

	/**
	 * @brief Destructor for the State object.
	 */
	~PDAState();

	/**
	 * @brief Gets the unique key for this transition.
	 * @return The key string.
	 */
	std::string getKey() const;

	/**
	 * @brief Gets the label for the state.
	 * @return The label for the state.
	 */
	std::string getLabel() const;

	/**
	 * @brief Sets the label for the state.
	 * @param label The label for the state.
	 */
	void setLabel(const std::string &label);

	/**
	 * @brief Returns whether the state is an accept state.
	 * @return True if the state is an accept state, false otherwise.
	 */
	bool getIsAccept() const;

	/**
	 * @brief Sets whether the state is an accept state.
	 * @param isAccept True if the state is an accept state, false otherwise.
	 */
	void setIsAccept(const bool &isAccept);

	/**
	 * @brief Checks if a transition exists.
	 * @param key The key of the transition.
	 * @return Bool indicating whether the transition exists or not.
	 */
	bool transitionExists(const std::string &key) const;

	/**
	 * @brief Adds a transition to the state's transitions vector.
	 * @param input The input of the transition.
	 * @param toStateKey The to state key of the transition.
	 * @param stackSymbol The top of the stack symbol.
	 * @param pushSymbol The symbol to be pushed onto the stack.
	 */
	void addTransitionTo(const std::string &input, const std::string &toStateKey, const std::string &stackSymbol,
	                     const std::string &pushSymbol);
	/**
	 * @brief Gets a transition input.
	 * @param transitionKey The key of the transition.
	 * @return The input of the transition.
	 */
	std::string getTransitionInput(const std::string &transitionKey);

	/**
	 * @brief Sets a transition input.
	 * @param transitionKey The key of the transition.
	 * @param input The input to set.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionInput(const std::string &transitionKey, const std::string &input);

	/**
	 * @brief Gets a transition to state.
	 * @param transitionKey The key of the transition.
	 * @return The to state key of the transition.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	std::string getTransitionToState(const std::string &transitionKey);

	/**
	 * @brief Sets a transition to state.
	 * @param transitionKey The key of the transition.
	 * @param toState The key of the to state.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionToState(const std::string &transitionKey, const std::string &toState);

	/**
	 * @brief Gets a transition stack symbol.
	 * @param transitionKey The key of the transition.
	 * @return The stack symbol of the transition.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	std::string getTransitionStackSymbol(const std::string &transitionKey);

	/**
	 * @brief Sets a transition stack symbol.
	 * @param transitionKey The key of the transition.
	 * @param stackSymbol The stack symbol to set.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionStackSymbol(const std::string &transitionKey, const std::string &stackSymbol);

	/**
	 * @brief Gets a transition push symbol.
	 * @param transitionKey The key of the transition.
	 * @return The push symbol of the transition.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	std::string getTransitionPushSymbol(const std::string &transitionKey);

	/**
	 * @brief Sets a transition push symbol.
	 * @param transitionKey The key of the transition.
	 * @param pushSymbol The push symbol to set.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionPushSymbol(const std::string &transitionKey, const std::string &pushSymbol);

	/**
	 * @brief Removes a transition from the state's transitions vector.
	 * @param transitionKey The key of the transition.
	 * @throws TransitionNotFoundException If transition is not found.
	 */
	void removeTransition(const std::string &transitionKey);

	/**
	 * @brief Clears transitions to the to state from the state's transitions vector.
	 * @param toStateKey The to state key of the transition.
	 */
	void clearTransitionsTo(const std::string &toStateKey);

	/**
	 * @brief Gets transitions from the state.
	 * @return A vector of transitions that start from the state.
	 */
	std::vector<PDATransition> getTransitions();

	/**
	 * @brief Clears all transitions from the state's transitions vector.
	 */
	void clearTransitions();

	/**
	 * @brief Gets a string representation of this state.
	 * @return A string representation of the state.
	 */
	std::string toString() const;
};
