#pragma once
#include "AutomatonException.h"
#include "config.h"
#include <string>

/**
 * @brief Represents a transition between two states in an automaton.
 * A transition is defined by a "from" state, a "to" state, and an input value.
 */
class AUTOMATASIMULATOR_API FATransition {
  private:
	/**
	 * @brief Unique identifier for the transition, formatted as "fromState-toState-Input".
	 */
	std::string key;

	/**
	 * @brief The key of the state from which the transition starts.
	 */
	std::string fromStateKey;

	/**
	 * @brief The state to which the transition leads.
	 */
	std::string toStateKey;

	/**
	 * @brief The input value that triggers the transition.
	 */
	std::string input;

	/**
	 * @brief Validates the format of a transition key
	 */
	static void validateTransitionKeyFormat(const std::string &key);

  public:
	FATransition() = default;
	/**
	 * @brief Constructs a new Transition object.
	 * @param fromState The state key from which the transition starts.
	 * @param toState The state key to which the transition leads.
	 * @param input The input value that triggers the transition.
	 */
	FATransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input);

	/**
	 * @brief Copy constructor for the Transition object.
	 * @param other The Transition object to copy.
	 */
	FATransition(const FATransition &other);

	/**
	 * @brief Copy assignment operator for the Transition object.
	 * @param other The Transition object to copy.
	 * @return A reference to the copied Transition object.
	 */
	FATransition &operator=(const FATransition &other);

	/**
	 * @brief Move constructor for the Transition object.
	 * @param other The Transition object to move.
	 */
	FATransition(FATransition &&other) noexcept;

	/**
	 * @brief Move assignment operator for the Transition object.
	 * @param other The Transition object to move.
	 * @return A reference to the moved Transition object.
	 */
	FATransition &operator=(FATransition &&other) noexcept;

	bool operator==(const FATransition &other) const;

	/**
	 * @brief Destructor for the Transition object.
	 */
	~FATransition();

	/**
	 * @brief Generate a unique transition key.
	 * @param fromStateKey The starting state key.
	 * @param toStateKey The destination state key.
	 * @param input The input symbol.
	 * @return A unique transition key string.
	 */
	static std::string generateTransitionKey(const std::string &fromStateKey, const std::string &toStateKey,
	                                         const std::string &input);

	/**
	 * @brief Gets the to state key of a transition from its key.
	 * @param key The transition key.
	 * @return to state key.
	 */
	static std::string getFromStateFromKey(const std::string &key);

	/**
	 * @brief Gets the from state key of a transition from its key.
	 * @param key The transition key.
	 * @return from state key.
	 */
	static std::string getToStateFromKey(const std::string &key);

	/**
	 * @brief Gets the input value of a transition from its key.
	 * @param key The transition key.
	 * @return The input value.
	 */
	static std::string getInputFromKey(const std::string &key);

	/**
	 * @brief Gets the unique key for this transition.
	 * @return The key string.
	 */
	std::string getKey() const;

	/**
	 * @brief Sets the "from" state of this transition.
	 * @param key The new "from" state key.
	 */
	void setFromStateKey(const std::string &fromStateKey);

	/**
	 * @brief Gets the "from" state of this transition.
	 * @return Key of the "from" state.
	 */
	std::string getFromStateKey() const;

	/**
	 * @brief Sets the "to" state of this transition.
	 * @param toState The new "to" state.
	 */
	void setToStateKey(const std::string &toStateKey);

	/**
	 * @brief Gets the "to" state key of this transition.
	 * @return Key of the "to" state.
	 */
	std::string getToStateKey() const;

	/**
	 * @brief Sets the input value for this transition.
	 * @param input The new input value.
	 */
	void setInput(const std::string &input);

	/**
	 * @brief Gets the input value for this transition.
	 * @return The input value as a string.
	 */
	std::string getInput() const;

	/**
	 * @brief Gets a string representation of this transition.
	 * @return A string representation of the transition.
	 */
	std::string toString() const;
};

namespace std {
template <> struct hash<FATransition> {
	size_t operator()(const FATransition &t) const {
		size_t hashValue = hash<std::string>()(t.getFromStateKey()) ^ (hash<std::string>()(t.getToStateKey()) << 1) ^
		                   (hash<std::string>()(t.getInput()) << 2);
		return hashValue;
	}
};
} // namespace std