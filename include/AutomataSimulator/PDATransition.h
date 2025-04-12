#pragma once
#include "AutomatonException.h"
#include "config.h"
#include <string>

/**
 * @brief Represents a transition between two states in an automaton.
 * A transition is defined by a "from" state, a "to" state, and an input value.
 */
class AUTOMATASIMULATOR_API PDATransition {
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
	 * @brief The top of the stack symbol,
	 */
	std::string stackSymbol;

	/**
	 * @brief The Symbol to be pushed onto the stack.
	 */
	std::string pushSymbol;

	/**
	 * @brief Validates the format of a transition key
	 */
	static void validateTransitionKeyFormat(const std::string &key);

  public:
	PDATransition() = default;

	/**
	 * @brief Constructs a new Transition object.
	 * @param fromState The state key from which the transition starts.
	 * @param toState The state key to which the transition leads.
	 * @param input The input value that triggers the transition.
	 * @param stackSymbol The top of the stack symbol.
	 * @param pushSymbol The symbol to be pushed onto the stack.
	 */
	PDATransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
	              const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @brief Copy constructor for the Transition object.
	 * @param other The Transition object to copy.
	 */
	PDATransition(const PDATransition &other);

	/**
	 * @brief Copy assignment operator for the Transition object.
	 * @param other The Transition object to copy.
	 * @return A reference to the copied Transition object.
	 */
	PDATransition &operator=(const PDATransition &other);

	/**
	 * @brief Move constructor for the Transition object.
	 * @param other The Transition object to move.
	 */
	PDATransition(PDATransition &&other) noexcept;

	/**
	 * @brief Move assignment operator for the Transition object.
	 * @param other The Transition object to move.
	 * @return A reference to the moved Transition object.
	 */
	PDATransition &operator=(PDATransition &&other) noexcept;

	bool operator==(const PDATransition &other) const;

	/**
	 * @brief Destructor for the Transition object.
	 */
	~PDATransition();

	/**
	 * @brief Generate a unique transition key.
	 * @param fromStateKey The starting state key.
	 * @param toStateKey The destination state key.
	 * @param input The input symbol.
	 * @param stackSymbol The top of the stack symbol.
	 * @param pushSymbol The symbol to be pushed onto the stack.
	 * @return A unique transition key string.
	 */
	static std::string generateTransitionKey(const std::string &fromStateKey, const std::string &toStateKey,
	                                         const std::string &input, const std::string &stackSymbol,
	                                         const std::string &pushSymbol);

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
	 * @brief Gets the stack symbol of a transition from its key.
	 * @param key The transition key.
	 * @return The stack symbol.
	 */
	static std::string getStackSymbolFromKey(const std::string &key);

	/**
	 * @brief Gets the push symbol of a transition from its key.
	 * @param key The transition key.
	 * @return The push symbol.
	 */
	static std::string getPushSymbolFromKey(const std::string &key);

	/**
	 * @brief Gets the unique key for this transition.
	 * @return The key string.
	 */
	std::string getKey() const;

	/**
	 * @brief Sets the "from" state of this transition.
	 * @param key The new "from" state key.
	 */
	void setFromStateKey(const std::string &key);

	/**
	 * @brief Gets the "from" state of this transition.
	 * @return Key of the "from" state.
	 */
	std::string getFromStateKey() const;

	/**
	 * @brief Sets the "to" state of this transition.
	 * @param toState The new "to" state.
	 */
	void setToStateKey(const std::string &toState);

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
	 * @brief Sets the top of the stack symbol for this transition.
	 * @param stackSymbol The new stack symbol.
	 */
	void setStackSymbol(const std::string &stackSymbol);

	/**
	 * @brief Gets the top of the stack symbol for this transition.
	 * @return The stack symbol as a string.
	 */
	std::string getStackSymbol() const;

	/**
	 * @brief Sets the symbol to be pushed onto the stack for this transition.
	 * @param pushSymbol The new push symbol.
	 */
	void setPushSymbol(const std::string &pushSymbol);

	/**
	 * @brief Gets the symbol to be pushed onto the stack for this transition.
	 * @return The push symbol as a string.
	 */
	std::string getPushSymbol() const;

	/**
	 * @brief Gets a string representation of this transition.
	 * @return A string representation of the transition.
	 */
	std::string toString() const;
};

namespace std {
template <> struct hash<PDATransition> {
	size_t operator()(const PDATransition &t) const {
		size_t hashValue = hash<std::string>()(t.getFromStateKey()) ^ (hash<std::string>()(t.getToStateKey()) << 1) ^
		                   (hash<std::string>()(t.getInput()) << 2) ^ (hash<std::string>()(t.getStackSymbol()) << 3) ^
		                   (hash<std::string>()(t.getPushSymbol()) << 4);
		return hashValue;
	}
};
} // namespace std