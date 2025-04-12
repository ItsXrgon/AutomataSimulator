#pragma once
#include "AutomatonException.h"
#include "TMTransition.h"
#include "config.h"
#include <string>
#include <unordered_map>

class AUTOMATASIMULATOR_API TMState {
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
	std::unordered_map<std::string, TMTransition> transitions;

	/**
	 * @brief Boolean indicating whether it is an accept state.
	 */
	bool isAccept;

	/**
	 * @brief Cached convertion of the transitions from unordered map to vector.
	 */
	std::vector<TMTransition> cachedTransitions;

	/**
	 * @brief Boolean indicating whether the cached states are invalidated or not.
	 */
	bool transitionsCacheInvalidated;

	/**
	 * @brief Gets the transition with the key provided.
	 * @param key The key of the transition to get.
	 * @return The transition with the specified key.
	 */
	TMTransition *getTransitionInternal(const std::string &key);

  public:
	TMState() = default;

	/**
	 * @brief Constructs a new State object.
	 * @param label The label for the state.
	 * @param isAccept Whether the state is an accept state or not.
	 */
	TMState(const std::string &label, const bool &isAccept = false);

	/**
	 * @brief Copy constructor for the State object.
	 * @param other The State object to copy.
	 */
	TMState(const TMState &other);

	/**
	 * @brief Copy assignment operator for the State object.
	 * @param other The State object to copy.
	 * @return A reference to the copied State object.
	 */
	TMState &operator=(const TMState &other);

	/**
	 * @brief Move constructor for the State object.
	 * @param other The State object to move.
	 */
	TMState(TMState &&other) noexcept;

	/**
	 * @brief Move assignment operator for the State object.
	 * @param other The State object to move.
	 * @return A reference to the moved State object.
	 */
	TMState &operator=(TMState &&other) noexcept;

	bool operator==(const TMState &other) const;

	/**
	 * @brief Destructor for the State object.
	 */
	~TMState();

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
	 * @param toStateKey The to state key of the transition.
	 * @param readSymbol The top of the stack symbol.
	 * @param writeSymbol The symbol to be writeed onto the stack.
	 * @param direction The Direction of the transitions
	 */
	void addTransition(const std::string &toStateKey, const std::string &readSymbol, const std::string &writeSymbol,
	                   const TMDirection &direction);

	/**
	 * @brief Gets the transition with the key provided.
	 * @param key The key of the transition to get.
	 * @return The transition with the specified key.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	TMTransition getTransition(const std::string &key);

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
	 * @brief Gets a transition read symbol.
	 * @param transitionKey The key of the transition.
	 * @return The read symbol of the transition.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	std::string getTransitionReadSymbol(const std::string &transitionKey);

	/**
	 * @brief Sets a transition read symbol.
	 * @param transitionKey The key of the transition.
	 * @param readSymbol The stack symbol to set.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionReadSymbol(const std::string &transitionKey, const std::string &readSymbol);

	/**
	 * @brief Gets a transition write symbol.
	 * @param transitionKey The key of the transition.
	 * @return The write symbol of the transition.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	std::string getTransitionWriteSymbol(const std::string &transitionKey);

	/**
	 * @brief Sets a transition write symbol.
	 * @param transitionKey The key of the transition.
	 * @param writeSymbol The write symbol to set.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionWriteSymbol(const std::string &transitionKey, const std::string &writeSymbol);

	/**
	 * @brief Gets a transition direction.
	 * @param transitionKey The key of the transition.
	 * @return The direction of the transition.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	TMDirection getTransitionDirection(const std::string &transitionKey);

	/**
	 * @brief Sets a transition write symbol.
	 * @param transitionKey The key of the transition.
	 * @param direction The direction to set.
	 * @throw TransitionNotFoundException If transition is not found.
	 * @throw InvalidTransitionException If the transition already exists.
	 */
	void setTransitionDirection(const std::string &transitionKey, const TMDirection &direction);

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
	std::vector<TMTransition> getTransitions();

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
