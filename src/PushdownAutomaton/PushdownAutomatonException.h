#pragma once
#include <exception>
#include <string>

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Base class for custom exceptions related to finite automata constraints.
 */
class AUTOMATASIMULATOR_API PushdownAutomataException : public std::exception {
  private:
	std::string message;

  public:
	PushdownAutomataException(const std::string &msg) : message(msg) {}

	const char *what() const noexcept override {
		return message.c_str();
	}
};

/**
 * @brief Thrown when a state is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StateNotFoundException : public PushdownAutomataException {
  public:
	StateNotFoundException(const std::string &stateKey) : PushdownAutomataException("State not found: " + stateKey) {}
};

/**
 * @brief Thrown when a input alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API InputAlphabetSymbolNotFoundException : public PushdownAutomataException {
  public:
	InputAlphabetSymbolNotFoundException(const std::string &symbol)
	    : PushdownAutomataException("Input alphabet symbol not found: " + symbol) {}
};

/**
 * @brief Thrown when a stack alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StackAlphabetSymbolNotFoundException : public PushdownAutomataException {
  public:
	StackAlphabetSymbolNotFoundException(const std::string &symbol)
	    : PushdownAutomataException("Input alphabet symbol not found: " + symbol) {}
};

/**
 * @brief Thrown when a transition is not found in the automaton.
 */
class AUTOMATASIMULATOR_API TransitionNotFoundException : public PushdownAutomataException {
  public:
	TransitionNotFoundException(const std::string &fromStateKey, const std::string &toStateKey,
	                            const std::string &input)
	    : PushdownAutomataException("Transition not found: " + fromStateKey + " -> " + input + " -> " + toStateKey) {}

	TransitionNotFoundException(const std::string &key)
	    : PushdownAutomataException("Transition with key " + key + " not found") {}
};

/**
 * @brief Thrown when the alphabet is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidAlphabetException : public PushdownAutomataException {
  public:
	InvalidAlphabetException(const std::string &msg) : PushdownAutomataException("Invalid alphabet: " + msg) {}
};

/**
 * @brief Thrown when the start state is not set or invalid.
 */
class AUTOMATASIMULATOR_API InvalidStartStateException : public PushdownAutomataException {
  public:
	InvalidStartStateException(const std::string &msg) : PushdownAutomataException("Invalid start state: " + msg) {}
};

/**
 * @brief Thrown when a transition is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidTransitionException : public PushdownAutomataException {
  public:
	InvalidTransitionException(const std::string &msg) : PushdownAutomataException("Invalid transition: " + msg) {}
};

/**
 * @brief Thrown when the automaton definition is invalid.
 */
class AUTOMATASIMULATOR_API InvalidAutomatonDefinitionException : public PushdownAutomataException {
  public:
	InvalidAutomatonDefinitionException(const std::string &msg)
	    : PushdownAutomataException("Invalid automaton definition: " + msg) {}
};