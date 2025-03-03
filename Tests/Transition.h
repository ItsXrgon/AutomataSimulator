#pragma once
#include <string>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a transition between two states in an automaton.
 * A transition is defined by a "from" state, a "to" state, and an input value.
 */
class AUTOMATASIMULATOR_API Transition {
  private:
	/**
	 * @brief Unique identifier for the transition, formatted as "fromState-toState-Input-StackSymbol-PushSymbol".
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
	 * @brief The top of the stack symbol
	 */
	std::string stackSymbol;

	/**
	 * @brief The Symbol to be pushed onto the stack
	 */
	std::string pushSymbol;
  public:
	/**
	 * @brief Constructs a new Transition object.
	 * @param fromState The state key from which the transition starts.
	 * @param toState The state key to which the transition leads.
	 * @param input The input value that triggers the transition.
	 * @param stackSymbol The top of the stack symbol
	 * @param pushSymbol The Symbol to be pushed onto the stack
	 */
	Transition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
	           const std::string &stackSymbol, const std::string &pushSymbol);

	/**
	 * @brief Copy constructor for the Transition object.
	 * @param other The Transition object to copy.
	 */
	Transition(const Transition &other);

	/**
	 * @brief Copy assignment operator for the Transition object.
	 * @param other The Transition object to copy.
	 * @return A reference to the copied Transition object.
	 */
	Transition &operator=(const Transition &other);

	/**
	 * @brief Move constructor for the Transition object.
	 * @param other The Transition object to move.
	 */
	Transition(Transition &&other) noexcept;

	/**
	 * @brief Move assignment operator for the Transition object.
	 * @param other The Transition object to move.
	 * @return A reference to the moved Transition object.
	 */
	Transition &operator=(Transition &&other) noexcept;

	/**
	 * @brief Destructor for the Transition object.
	 */
	~Transition();

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
	void setInput(std::string input);

	/**
	 * @brief Gets the input value for this transition.
	 * @return The input value as a string.
	 */
	std::string getInput() const;

	/**
	 * @brief Sets the stack symbol for this transition.
	 * @param stackSymbol The new stack symbol.
	 */
	void setStackSymbol(const std::string &stackSymbol);

	/**
	 * @brief Gets the stack symbol for this transition.
	 * @return The stack symbol as a string.
	 */
	std::string getStackSymbol() const;

	/**
	 * @brief Sets the push symbol for this transition.
	 * @param pushSymbol The new push symbol.
	 */
	void setPushSymbol(const std::string &pushSymbol);

	/**
	 * @brief Gets the push symbol for this transition.
	 * @return The push symbol as a string.
	 */
	std::string getPushSymbol() const;
};