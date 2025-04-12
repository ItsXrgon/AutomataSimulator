#pragma once
#include "AutomatonException.h"
#include "FATransition.h"
#include "config.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Represents a state in an automaton.
 * A state is defined by a "label" string, a vector of transitions that start from it, and a boolean indicating whether
 * it is an accept state.
 */
class AUTOMATASIMULATOR_API FAState {
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
	std::unordered_map<std::string, FATransition> transitions;

	/**
	 * @brief Boolean indicating whether it is an accept state.
	 */
	bool isAccept;

	/**
	 * @brief Cached convertion of the transitions from unordered map to vector.
	 */
	std::vector<FATransition> cachedTransitions;

	/**
	 * @brief Boolean indicating whether the cached states are invalidated or not.
	 */
	bool transitionsCacheInvalidated;

	/**
	 * @brief Gets the transition with the key provided.
	 * @param key The key of the transition to get.
	 * @return The transition with the specified key.
	 */
	FATransition *getTransitionInternal(const std::string &key);

  public:
	FAState() = default;

	/**
	 * @brief Constructs a new State object.
	 * @param label The label for the state.
	 * @param isAccept Whether the state is an accept state or not.
	 */
	FAState(const std::string &label, const bool &isAccept = false);

	/**
	 * @brief Copy constructor for the State object.
	 * @param other The State object to copy.
	 */
	FAState(const FAState &other);

	/**
	 * @brief Copy assignment operator for the State object.
	 * @param other The State object to copy.
	 * @return A reference to the copied State object.
	 */
	FAState &operator=(const FAState &other);

	/**
	 * @brief Move constructor for the State object.
	 * @param other The State object to move.
	 */
	FAState(FAState &&other) noexcept;

	/**
	 * @brief Move assignment operator for the State object.
	 * @param other The State object to move.
	 * @return A reference to the moved State object.
	 */
	FAState &operator=(FAState &&other) noexcept;

	bool operator==(const FAState &other) const;

	/**
	 * @brief Destructor for the State object.
	 */
	~FAState();

	/**
	 * @brief Gets the unique key for this transition.
	 * @return The key string.
	 */
	std::string getKey() const;

	/**
	 * @brief Sets the label for the state.
	 * @param label The label for the state.
	 */
	void setLabel(const std::string &label);

	/**
	 * @brief Gets the label for the state.
	 * @return The label for the state.
	 */
	std::string getLabel() const;

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
	 * @param input The input of the transition.
	 * @throws InvalidTransitionException If the transition already exists.
	 */
	void addTransition(const std::string &toStateKey, const std::string &input);

	/**
	 * @brief Gets the transition with the key provided.
	 * @param key The key of the transition to get.
	 * @return The transition with the specified key.
	 * @throw TransitionNotFoundException If transition is not found.
	 */
	FATransition getTransition(const std::string &key);

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
	 * @return Transitions that start from the state.
	 */
	std::vector<FATransition> getTransitions();

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
