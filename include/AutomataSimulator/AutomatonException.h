#pragma once
#include "config.h"
#include <stdexcept>
#include <string>

/**
 * @brief Base class for custom exceptions related to finite automata constraints.
 */
class AutomatonException : std::exception {
  private:
	std::string message;

  public:
	inline AutomatonException(const std::string &msg) : message(msg) {}
	inline virtual ~AutomatonException() = default;

	inline const char *what() const noexcept override {
		return message.c_str();
	}
};

/**
 * @brief Thrown when a state is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StateNotFoundException : public AutomatonException {
  public:
	inline StateNotFoundException(const std::string &stateKey) : AutomatonException("State not found: " + stateKey) {}
	inline virtual ~StateNotFoundException() = default;
};

/**
 * @brief Thrown when a input alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API InputAlphabetSymbolNotFoundException : public AutomatonException {
  public:
	inline InputAlphabetSymbolNotFoundException(const std::string &symbol)
	    : AutomatonException("Input alphabet symbol not found: " + symbol) {}
	inline virtual ~InputAlphabetSymbolNotFoundException() = default;
};

/**
 * @brief Thrown when a stack alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API StackAlphabetSymbolNotFoundException : public AutomatonException {
  public:
	inline StackAlphabetSymbolNotFoundException(const std::string &symbol)
	    : AutomatonException("Stack alphabet symbol not found: " + symbol) {}
	inline virtual ~StackAlphabetSymbolNotFoundException() = default;
};

/**
 * @brief Thrown when a tape alphabet symbol is not found in the automaton.
 */
class AUTOMATASIMULATOR_API TapeAlphabetSymbolNotFoundException : public AutomatonException {
  public:
	inline TapeAlphabetSymbolNotFoundException(const std::string &symbol)
	    : AutomatonException("Tape alphabet symbol not found: " + symbol) {}
	inline virtual ~TapeAlphabetSymbolNotFoundException() = default;
};

/**
 * @brief Thrown when a transition is not found in the automaton.
 */
class AUTOMATASIMULATOR_API TransitionNotFoundException : public AutomatonException {
  public:
	inline TransitionNotFoundException(const std::string &fromStateKey, const std::string &toStateKey,
	                            const std::string &input)
	    : AutomatonException("Transition not found: " + fromStateKey + " -> " + input + " -> " + toStateKey) {}

	inline TransitionNotFoundException(const std::string &key)
	    : AutomatonException("Transition with key " + key + " not found") {}

	inline virtual ~TransitionNotFoundException() = default;
};

/**
 * @brief Thrown when the alphabet is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidAlphabetException : public AutomatonException {
  public:
	inline InvalidAlphabetException(const std::string &msg) : AutomatonException("Invalid alphabet: " + msg) {}
	inline virtual ~InvalidAlphabetException() = default;
};

/**
 * @brief Thrown when the start state is not set or invalid.
 */
class AUTOMATASIMULATOR_API InvalidStartStateException : public AutomatonException {
  public:
	inline InvalidStartStateException(const std::string &msg) : AutomatonException("Invalid start state: " + msg) {}
	inline virtual ~InvalidStartStateException() = default;
};

/**
 * @brief Thrown when a transition is invalid or missing.
 */
class AUTOMATASIMULATOR_API InvalidTransitionException : public AutomatonException {
  public:
	inline InvalidTransitionException(const std::string &msg) : AutomatonException("Invalid transition: " + msg) {}
	inline virtual ~InvalidTransitionException() = default;
};

/**
 * @brief Thrown when the automaton definition is invalid.
 */
class AUTOMATASIMULATOR_API InvalidAutomatonDefinitionException : public AutomatonException {
  public:
	inline  InvalidAutomatonDefinitionException(const std::string &msg)
	    : AutomatonException("Invalid automaton definition: " + msg) {}
	inline  virtual ~InvalidAutomatonDefinitionException() = default;
};