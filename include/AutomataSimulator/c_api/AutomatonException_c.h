#include "../AutomatonException.h"

typedef enum {
	AUTOMATON_OK = 0,
	AUTOMATON_ERR_STATE_NOT_FOUND,
	AUTOMATON_ERR_INPUT_SYMBOL_NOT_FOUND,
	AUTOMATON_ERR_STACK_SYMBOL_NOT_FOUND,
	AUTOMATON_ERR_TAPE_SYMBOL_NOT_FOUND,
	AUTOMATON_ERR_TRANSITION_NOT_FOUND,
	AUTOMATON_ERR_INVALID_ALPHABET,
	AUTOMATON_ERR_INVALID_START_STATE,
	AUTOMATON_ERR_INVALID_TRANSITION,
	AUTOMATON_ERR_INVALID_DEFINITION,
	AUTOMATON_ERR_UNKNOWN
} AutomatonErrorCode;

typedef struct {
	AutomatonErrorCode code;
	char *message;
} AutomatonError;

// Initialize a default error (success)
inline AutomatonError create_success_error() {
	AutomatonError error;
	error.code = AUTOMATON_OK;
	error.message = nullptr;
	return error;
}

// Create an error with message
inline AutomatonError create_error(AutomatonErrorCode code, const char *message) {
	AutomatonError error;
	error.code = code;
	error.message = message ? strdup(message) : nullptr;
	return error;
}

// Free error resources
inline void free_automaton_error(AutomatonError *error) {
	if (error && error->message) {
		free(error->message);
		error->message = nullptr;
	}
}

// Map C++ exceptions to AutomatonError
inline AutomatonError handle_exception_with_message(const std::exception &ex) {
	AutomatonErrorCode code = AUTOMATON_ERR_UNKNOWN;

	// Map exception types to error codes
	if (dynamic_cast<const StateNotFoundException *>(&ex))
		code = AUTOMATON_ERR_STATE_NOT_FOUND;
	if (dynamic_cast<const InputAlphabetSymbolNotFoundException *>(&ex))
		code = AUTOMATON_ERR_INPUT_SYMBOL_NOT_FOUND;
	if (dynamic_cast<const StackAlphabetSymbolNotFoundException *>(&ex))
		code = AUTOMATON_ERR_STACK_SYMBOL_NOT_FOUND;
	if (dynamic_cast<const TapeAlphabetSymbolNotFoundException *>(&ex))
		code = AUTOMATON_ERR_TAPE_SYMBOL_NOT_FOUND;
	if (dynamic_cast<const TransitionNotFoundException *>(&ex))
		code = AUTOMATON_ERR_TRANSITION_NOT_FOUND;
	if (dynamic_cast<const InvalidAlphabetException *>(&ex))
		code = AUTOMATON_ERR_INVALID_ALPHABET;
	if (dynamic_cast<const InvalidStartStateException *>(&ex))
		code = AUTOMATON_ERR_INVALID_START_STATE;
	if (dynamic_cast<const InvalidTransitionException *>(&ex))
		code = AUTOMATON_ERR_INVALID_TRANSITION;
	if (dynamic_cast<const InvalidAutomatonDefinitionException *>(&ex))
		code = AUTOMATON_ERR_INVALID_DEFINITION;

	return create_error(code, ex.what());
}