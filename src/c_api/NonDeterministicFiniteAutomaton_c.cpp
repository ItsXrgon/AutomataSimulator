#include "AutomataSimulator/c_api/NonDeterministicFiniteAutomaton_c.h"

struct NFA_t {
	NonDeterministicFiniteAutomaton instance;
};

NFAHandle NFA_create(AutomatonError *error) {
	return wrap_result<NFAHandle>([&]() { return reinterpret_cast<NFAHandle>(new NonDeterministicFiniteAutomaton()); },
	                              error, {});
}

void NFA_destroy(NFAHandle nfa, AutomatonError *error) {
	wrap_result([&]() { delete reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa); }, error);
}

const StringArray NFA_getInput(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getInput()); },
	    error);
}

void NFA_setInput(NFAHandle nfa, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->setInput(input_vec);
	    },
	    error);
}

void NFA_addInput(NFAHandle nfa, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->addInput(input_vec);
	    },
	    error);
}

const int NFA_getInputHead(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<int>([&]() { return reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getInputHead(); },
	                        error);
}

void NFA_setInputHead(NFAHandle nfa, const int head, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->setInputHead(head); }, error);
}

const bool NFA_stateExists(NFAHandle nfa, const char *key, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->stateExists(key); }, error);
}

const bool NFA_inputAlphabetSymbolExists(NFAHandle nfa, const char *symbol, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->inputAlphabetSymbolExists(symbol); },
	    error);
}

void NFA_addState(NFAHandle nfa, const char *label, const bool isAccept, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->addState(label, isAccept); }, error);
}

void NFA_updateStateLabel(NFAHandle nfa, const char *key, const char *label, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->updateStateLabel(key, label); },
	            error);
}

const char *NFA_getCurrentState(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getCurrentState()); },
	    error);
}

void NFA_setCurrentState(NFAHandle nfa, const char *state, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->setCurrentState(state); }, error);
}

const FAStateHandle NFA_getState(NFAHandle nfa, const char *key, AutomatonError *error) {
	return wrap_result<FAStateHandle>(
	    [&]() {
		    return reinterpret_cast<FAStateHandle>(
		        new FAState(reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getState(key)));
	    },
	    error);
}

const FAStateArray NFA_getStates(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<FAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getStates();

		    FAStateArray array;
		    array.length = states.size();
		    array.data = new FAStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<FAStateHandle>(new FAState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void NFA_removeState(NFAHandle nfa, const char *key, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeState(key, strict); }, error);
}

void NFA_removeStates(NFAHandle nfa, const char **keys, const size_t length, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeStates(keys_vec, strict);
	    },
	    error);
}

void NFA_clearStates(NFAHandle nfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->clearStates(); }, error);
}

void NFA_setInputAlphabet(NFAHandle nfa, const char **inputAlphabet, const size_t length, const bool strict,
                          AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->setInputAlphabet(alphabet, strict);
	    },
	    error);
}

void NFA_addInputAlphabet(NFAHandle nfa, const char **inputAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->addInputAlphabet(alphabet);
	    },
	    error);
}

const StringArray NFA_getInputAlphabet(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getInputAlphabet());
	    },
	    error);
}

void NFA_removeInputAlphabetSymbol(NFAHandle nfa, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeInputAlphabetSymbol(symbol, strict); },
	    error);
}

void NFA_removeInputAlphabetSymbols(NFAHandle nfa, const char **symbols, const size_t length, const bool strict,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeInputAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void NFA_clearInputAlphabet(NFAHandle nfa, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->clearInputAlphabet(strict); }, error);
}

const char *NFA_getStartState(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getStartState()); },
	    error);
}

void NFA_setStartState(NFAHandle nfa, const char *key, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->setStartState(key); }, error);
}

FATransitionHandle DFA_getTransition(NFAHandle nfa, const char *key, AutomatonError *error) {
	return wrap_result<FATransitionHandle>(
	    [&]() {
		    return reinterpret_cast<FATransitionHandle>(
		        new FATransition(reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getTransition(key)));
	    },
	    error);
}

void NFA_addTransition(NFAHandle nfa, const char *fromStateKey, const char *toStateKey, const char *input,
                       AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->addTransition(fromStateKey, toStateKey, input);
	    },
	    error);
}

void NFA_updateTransitionInput(NFAHandle nfa, const char *transitionKey, const char *input, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->updateTransitionInput(transitionKey, input);
	    },
	    error);
}

void NFA_updateTransitionFromState(NFAHandle nfa, const char *transitionKey, const char *fromStateKey,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->updateTransitionFromState(transitionKey,
		                                                                                        fromStateKey);
	    },
	    error);
}

void NFA_updateTransitionToState(NFAHandle nfa, const char *transitionKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->updateTransitionToState(transitionKey,
		                                                                                      toStateKey);
	    },
	    error);
}

void NFA_removeTransition(NFAHandle nfa, const char *transitionKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeTransition(transitionKey); },
	            error);
}

void NFA_clearTransitionsBetween(NFAHandle nfa, const char *fromStateKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->clearTransitionsBetween(fromStateKey, toStateKey);
	    },
	    error);
}

void NFA_clearStateTransitions(NFAHandle nfa, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->clearStateTransitions(stateKey); },
	            error);
}

void NFA_clearTransitions(NFAHandle nfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->clearTransitions(); }, error);
}

void NFA_addAcceptState(NFAHandle nfa, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->addAcceptState(stateKey); }, error);
}

void NFA_addAcceptStates(NFAHandle nfa, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->addAcceptStates(keys_vec);
	    },
	    error);
}

void NFA_removeAcceptState(NFAHandle nfa, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeAcceptState(stateKey); },
	            error);
}

void NFA_removeAcceptStates(NFAHandle nfa, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->removeAcceptStates(keys_vec);
	    },
	    error);
}

void NFA_clearAcceptStates(NFAHandle nfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->clearAcceptStates(); }, error);
}

const FAStateArray NFA_getAcceptStates(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<FAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getAcceptStates();

		    FAStateArray array;
		    array.length = states.size();
		    array.data = new FAStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<FAStateHandle>(new FAState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void NFA_reset(NFAHandle nfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->reset(); }, error);
}

const bool NFA_isAccepting(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->isAccepting(); },
	                         error);
}

bool NFA_processInput(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->processInput(); },
	                         error);
}

const bool NFA_simulate(NFAHandle nfa, const char **input, const size_t length, const int simulationDepth,
                        AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    return reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->simulate(input_vec, simulationDepth);
	    },
	    error);
}

const FAStateArray getPossibleCurrentStates(NFAHandle nfa, AutomatonError *error) {
	return wrap_result<FAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicFiniteAutomaton *>(nfa)->getPossibleCurrentStates();

		    FAStateArray array;
		    array.length = states.size();
		    array.data = new FAStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<FAStateHandle>(new FAState(states[i]));
		    }

		    return array;
	    },
	    error);
}
