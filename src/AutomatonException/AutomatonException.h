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
class AUTOMATASIMULATOR_API AutomatonException : public std::exception {
  private:
	std::string message;

  public:
	AutomatonException(const std::string &msg) : message(msg) {}

	const char *what() const noexcept override {
		return message.c_str();
	}
};

/**
 * @brief Thrown when a state is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StateNotFoundException : public AutomatonException {
  public:
	StateNotFoundException(const std::string &stateKey) : AutomatonException("State not found: " + stateKey) {}
};

/**
 * @brief Thrown when a input alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API InputAlphabetSymbolNotFoundException : public AutomatonException {
  public:
	InputAlphabetSymbolNotFoundException(const std::string &symbol)
	    : AutomatonException("Input alphabet symbol not found: " + symbol) {}
};

/**
 * @brief Thrown when a stack alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StackAlphabetSymbolNotFoundException : public AutomatonException {
  public:
	StackAlphabetSymbolNotFoundException(const std::string &symbol)
	    : AutomatonException("Input alphabet symbol not found: " + symbol) {}
};

/**
 * @brief Thrown when a transition is not found in the automaton.
 */
class AUTOMATASIMULATOR_API TransitionNotFoundException : public AutomatonException {
  public:
	TransitionNotFoundException(const std::string &fromStateKey, const std::string &toStateKey,
	                            const std::string &input)
	    : AutomatonException("Transition not found: " + fromStateKey + " -> " + input + " -> " + toStateKey) {}

	TransitionNotFoundException(const std::string &key)
	    : AutomatonException("Transition with key " + key + " not found") {}
};

/**
 * @brief Thrown when the alphabet is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidAlphabetException : public AutomatonException {
  public:
	InvalidAlphabetException(const std::string &msg) : AutomatonException("Invalid alphabet: " + msg) {}
};

/**
 * @brief Thrown when the start state is not set or invalid.
 */
class AUTOMATASIMULATOR_API InvalidStartStateException : public AutomatonException {
  public:
	InvalidStartStateException(const std::string &msg) : AutomatonException("Invalid start state: " + msg) {}
};

/**
 * @brief Thrown when a transition is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidTransitionException : public AutomatonException {
  public:
	InvalidTransitionException(const std::string &msg) : AutomatonException("Invalid transition: " + msg) {}
};

/**
 * @brief Thrown when the automaton definition is invalid.
 */
class AUTOMATASIMULATOR_API InvalidAutomatonDefinitionException : public AutomatonException {
  public:
	InvalidAutomatonDefinitionException(const std::string &msg)
	    : AutomatonException("Invalid automaton definition: " + msg) {}
};

/**
 * @brief Thrown when simulation depth is exceeded
 */
class AUTOMATASIMULATOR_API SimulationDepthExceededException : public AutomatonException {
  public:
	SimulationDepthExceededException(const int &depth)
	    : AutomatonException("Simulation depth of " + std::to_string(depth) +
	                              "has been exceeded for the provided input") {}
};