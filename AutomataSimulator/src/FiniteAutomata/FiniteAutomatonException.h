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
class AUTOMATASIMULATOR_API FiniteAutomatonException : public std::exception {
  private:
	std::string message;

  public:
	FiniteAutomatonException(const std::string &msg) : message(msg) {}

	const char *what() const noexcept override {
		return message.c_str();
	}
};

/**
 * @brief Thrown when a state is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StateNotFoundException : public FiniteAutomatonException {
  public:
	StateNotFoundException(const std::string &stateKey) : FiniteAutomatonException("State not found: " + stateKey) {}
};

/**
 * @brief Thrown when a transition is not found in the automaton.
 */
class AUTOMATASIMULATOR_API TransitionNotFoundException : public FiniteAutomatonException {
  public:
	TransitionNotFoundException(const std::string &stateKey)
	    : FiniteAutomatonException("State not found: " + stateKey) {}
};

/**
 * @brief Thrown when the alphabet is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidAlphabetException : public FiniteAutomatonException {
  public:
	InvalidAlphabetException(const std::string &msg) : FiniteAutomatonException("Invalid alphabet: " + msg) {}
};

/**
 * @brief Thrown when the start state is not set or invalid.
 */
class AUTOMATASIMULATOR_API InvalidStartStateException : public FiniteAutomatonException {
  public:
	InvalidStartStateException(const std::string &msg) : FiniteAutomatonException("Invalid start state: " + msg) {}
};

/**
 * @brief Thrown when a transition is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidTransitionException : public FiniteAutomatonException {
  public:
	InvalidTransitionException(const std::string &msg) : FiniteAutomatonException("Invalid transition: " + msg) {}
};

/**
 * @brief Thrown when the automaton definition is invalid.
 */
class AUTOMATASIMULATOR_API InvalidAutomatonDefinitionException : public FiniteAutomatonException {
  public:
	InvalidAutomatonDefinitionException(const std::string &msg)
	    : FiniteAutomatonException("Invalid automaton definition: " + msg) {}
};