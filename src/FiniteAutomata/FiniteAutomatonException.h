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
class AUTOMATASIMULATOR_API FiniteAutomataException : public std::exception {
  private:
	std::string message;

  public:
	FiniteAutomataException(const std::string &msg) : message(msg) {}

	const char *what() const noexcept override {
		return message.c_str();
	}
};

/**
 * @brief Thrown when a state is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StateNotFoundException : public FiniteAutomataException {
  public:
	StateNotFoundException(const std::string &stateKey) : FiniteAutomataException("State not found: " + stateKey) {}
};

/**
 * @brief Thrown when a transition is not found in the automaton.
 */
class AUTOMATASIMULATOR_API TransitionNotFoundException : public FiniteAutomataException {
  public:
	TransitionNotFoundException(const std::string &stateKey)
	    : FiniteAutomataException("State not found: " + stateKey) {}
};

/**
 * @brief Thrown when the alphabet is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidAlphabetException : public FiniteAutomataException {
  public:
	InvalidAlphabetException(const std::string &msg) : FiniteAutomataException("Invalid alphabet: " + msg) {}
};

/**
 * @brief Thrown when the start state is not set or invalid.
 */
class AUTOMATASIMULATOR_API InvalidStartStateException : public FiniteAutomataException {
  public:
	InvalidStartStateException(const std::string &msg) : FiniteAutomataException("Invalid start state: " + msg) {}
};

/**
 * @brief Thrown when a transition is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidTransitionException : public FiniteAutomataException {
  public:
	InvalidTransitionException(const std::string &msg) : FiniteAutomataException("Invalid transition: " + msg) {}
};

/**
 * @brief Thrown when the automaton definition is invalid.
 */
class AUTOMATASIMULATOR_API InvalidAutomatonDefinitionException : public FiniteAutomataException {
  public:
	InvalidAutomatonDefinitionException(const std::string &msg)
	    : FiniteAutomataException("Invalid automaton definition: " + msg) {}
};