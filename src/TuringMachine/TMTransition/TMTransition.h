#pragma once
#include "../../AutomatonException/AutomatonException.h"
#include "../TMDirection.h"
#include <string>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a transition between two states in a turing machine.
 * A transition is defined by a "from" state, a "to" state, and an input value.
 */
class AUTOMATASIMULATOR_API TMTransition {
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
	 * @brief The top of the stack symbol,
	 */
	std::string readSymbol;

	/**
	 * @brief Direction of the transition
	 */
	TMDirection direction;

	/**
	 * @brief The Symbol to be pushed onto the stack.
	 */
	std::string writeSymbol;

	/**
	 * @brief Validates the format of a transition key
	 */
	static void validateTransitionKeyFormat(const std::string &key);

  public:
	TMTransition() = default;

	/**
	 * @brief Constructs a new Transition object.
	 * @param fromState The state key from which the transition starts.
	 * @param toState The state key to which the transition leads.
	 * @param readSymbol The top of the stack symbol.
	 * @param writeSymbol The symbol to be pushed onto the stack.
	 */
	TMTransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &readSymbol,
	             const std::string &writeSymbol, TMDirection direction);

	/**
	 * @brief Copy constructor for the Transition object.
	 * @param other The Transition object to copy.
	 */
	TMTransition(const TMTransition &other);

	/**
	 * @brief Copy assignment operator for the Transition object.
	 * @param other The Transition object to copy.
	 * @return A reference to the copied Transition object.
	 */
	TMTransition &operator=(const TMTransition &other);

	/**
	 * @brief Move constructor for the Transition object.
	 * @param other The Transition object to move.
	 */
	TMTransition(TMTransition &&other) noexcept;

	/**
	 * @brief Move assignment operator for the Transition object.
	 * @param other The Transition object to move.
	 * @return A reference to the moved Transition object.
	 */
	TMTransition &operator=(TMTransition &&other) noexcept;

	bool operator==(const TMTransition &other) const;

	/**
	 * @brief Destructor for the Transition object.
	 */
	~TMTransition();

	/**
	 * @brief Generate a unique transition key.
	 * @param fromStateKey The starting state key.
	 * @param toStateKey The destination state key.
	 * @param readSymbol The top of the stack symbol.
	 * @param writeSymbol The symbol to be pushed onto the stack.
	 * @return A unique transition key string.
	 */
	static std::string generateTransitionKey(const std::string &fromStateKey, const std::string &toStateKey,
	                                         const std::string &readSymbol, const std::string &writeSymbol,
	                                         TMDirection direction);

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
	 * @brief Gets the direction from the transition key.
	 * @param key The transition key.
	 * @return The direction enum.
	 */
	static TMDirection getDirectionFromKey(const std::string &key);

	/**
	 * @brief Gets the read symbol from the transition key.
	 * @param key The transition key.
	 * @return The read symbol.
	 */
	static std::string getReadSymbolFromKey(const std::string &key);

	/**
	 * @brief Gets the write symbol from the transition key.
	 * @param key The transition key.
	 * @return The write symbol.
	 */
	static std::string getWriteSymbolFromKey(const std::string &key);

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
	 * @brief Sets the top of the stack symbol for this transition.
	 * @param readSymbol The new stack symbol.
	 */
	void setReadSymbol(const std::string &readSymbol);

	/**
	 * @brief Gets the top of the stack symbol for this transition.
	 * @return The stack symbol as a string.
	 */
	std::string getReadSymbol() const;

	/**
	 * @brief Sets the symbol to be pushed onto the stack for this transition.
	 * @param writeSymbol The new push symbol.
	 */
	void setWriteSymbol(const std::string &writeSymbol);

	/**
	 * @brief Sets the direction value for this transition.
	 * @param input The new input value.
	 */
	void setDirection(TMDirection direction);

	/**
	 * @brief Gets the direction value for this transition.
	 * @return The direction value as an enum.
	 */
	TMDirection getDirection() const;

	/**
	 * @brief Gets the symbol to be pushed onto the stack for this transition.
	 * @return The push symbol as a string.
	 */
	std::string getWriteSymbol() const;

	/**
	 * @brief Gets a string representation of this transition.
	 * @return A string representation of the transition.
	 */
	std::string toString() const;
};

namespace std {
template <> struct hash<TMTransition> {
	size_t operator()(const TMTransition &t) const {
		size_t hashValue = hash<std::string>()(t.getFromStateKey()) ^ (hash<std::string>()(t.getToStateKey()) << 1) ^
		                   (hash<std::string>()(t.getReadSymbol()) << 2) ^
		                   (hash<std::string>()(t.getWriteSymbol()) << 3) ^
		                   (hash<int>()(static_cast<int>(t.getDirection())) << 4);
		return hashValue;
	}
};
} // namespace std
