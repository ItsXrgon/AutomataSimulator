#include "AutomataSimulator/c_api/DeterministicFiniteAutomaton_c.h"

struct DFA_t {
	DeterministicFiniteAutomaton instance;
};

DFAHandle DFA_create(AutomatonError *error) {
	return wrap_result<DFAHandle>([&]() { return reinterpret_cast<DFAHandle>(new DeterministicFiniteAutomaton()); },
	                              error, {});
}

void DFA_destroy(DFAHandle dfa, AutomatonError *error) {
	wrap_result([&]() { delete reinterpret_cast<DeterministicFiniteAutomaton *>(dfa); }, error);
}

const StringArray DFA_getInput(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getInput()); },
	    error);
}

void DFA_setInput(DFAHandle dfa, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->setInput(input_vec);
	    },
	    error);
}

void DFA_addInput(DFAHandle dfa, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->addInput(input_vec);
	    },
	    error);
}

const int DFA_getInputHead(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<int>([&]() { return reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getInputHead(); },
	                        error);
}

void DFA_setInputHead(DFAHandle dfa, const int head, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->setInputHead(head); }, error);
}

const bool DFA_stateExists(DFAHandle dfa, const char *key, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->stateExists(key); },
	                         error);
}

const bool DFA_inputAlphabetSymbolExists(DFAHandle dfa, const char *symbol, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->inputAlphabetSymbolExists(symbol); },
	    error);
}

void DFA_addState(DFAHandle dfa, const char *label, const bool isAccept, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->addState(label, isAccept); }, error);
}

void DFA_updateStateLabel(DFAHandle dfa, const char *key, const char *label, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->updateStateLabel(key, label); }, error);
}

const char *DFA_getCurrentState(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getCurrentState()); },
	    error);
}

void DFA_setCurrentState(DFAHandle dfa, const char *state, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->setCurrentState(state); }, error);
}

const FAStateHandle DFA_getState(DFAHandle dfa, const char *key, AutomatonError *error) {
	return wrap_result<FAStateHandle>(
	    [&]() {
		    return reinterpret_cast<FAStateHandle>(
		        new FAState(reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getState(key)));
	    },
	    error);
}

const FAStateArray DFA_getStates(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<FAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getStates();

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

void DFA_removeState(DFAHandle dfa, const char *key, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeState(key, strict); }, error);
}

void DFA_removeStates(DFAHandle dfa, const char **keys, const size_t length, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeStates(keys_vec, strict);
	    },
	    error);
}

void DFA_clearStates(DFAHandle dfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->clearStates(); }, error);
}

void DFA_setInputAlphabet(DFAHandle dfa, const char **inputAlphabet, const size_t length, const bool strict,
                          AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->setInputAlphabet(alphabet, strict);
	    },
	    error);
}

void DFA_addInputAlphabet(DFAHandle dfa, const char **inputAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->addInputAlphabet(alphabet);
	    },
	    error);
}

const StringArray DFA_getInputAlphabet(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getInputAlphabet());
	    },
	    error);
}

void DFA_removeInputAlphabetSymbol(DFAHandle dfa, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeInputAlphabetSymbol(symbol, strict); },
	    error);
}

void DFA_removeInputAlphabetSymbols(DFAHandle dfa, const char **symbols, const size_t length, const bool strict,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeInputAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void DFA_clearInputAlphabet(DFAHandle dfa, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->clearInputAlphabet(strict); }, error);
}

const char *DFA_getStartState(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getStartState()); },
	    error);
}

void DFA_setStartState(DFAHandle dfa, const char *key, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->setStartState(key); }, error);
}

void DFA_addTransition(DFAHandle dfa, const char *fromStateKey, const char *toStateKey, const char *input,
                       AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->addTransition(fromStateKey, toStateKey, input);
	    },
	    error);
}

FATransitionHandle DFA_getTransition(DFAHandle dfa, const char *key, AutomatonError *error) {  
   return wrap_result<FATransitionHandle>(  
       [&]() {  
           return reinterpret_cast<FATransitionHandle>(  
               new FATransition(reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getTransition(key)));  
       },  
       error);  
}

void DFA_updateTransitionInput(DFAHandle dfa, const char *transitionKey, const char *input, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->updateTransitionInput(transitionKey, input); },
	    error);
}

void DFA_updateTransitionFromState(DFAHandle dfa, const char *transitionKey, const char *fromStateKey,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->updateTransitionFromState(transitionKey,
		                                                                                     fromStateKey);
	    },
	    error);
}

void DFA_updateTransitionToState(DFAHandle dfa, const char *transitionKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->updateTransitionToState(transitionKey, toStateKey);
	    },
	    error);
}

void DFA_removeTransition(DFAHandle dfa, const char *transitionKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeTransition(transitionKey); },
	            error);
}

void DFA_clearTransitionsBetween(DFAHandle dfa, const char *fromStateKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->clearTransitionsBetween(fromStateKey, toStateKey);
	    },
	    error);
}

void DFA_clearStateTransitions(DFAHandle dfa, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->clearStateTransitions(stateKey); },
	            error);
}

void DFA_clearTransitions(DFAHandle dfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->clearTransitions(); }, error);
}

void DFA_addAcceptState(DFAHandle dfa, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->addAcceptState(stateKey); }, error);
}

void DFA_addAcceptStates(DFAHandle dfa, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->addAcceptStates(keys_vec);
	    },
	    error);
}

void DFA_removeAcceptState(DFAHandle dfa, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeAcceptState(stateKey); }, error);
}

void DFA_removeAcceptStates(DFAHandle dfa, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->removeAcceptStates(keys_vec);
	    },
	    error);
}

void DFA_clearAcceptStates(DFAHandle dfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->clearAcceptStates(); }, error);
}

const FAStateArray DFA_getAcceptStates(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<FAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->getAcceptStates();

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

void DFA_reset(DFAHandle dfa, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->reset(); }, error);
}

const bool DFA_isAccepting(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->isAccepting(); },
	                         error);
}

bool DFA_processInput(DFAHandle dfa, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->processInput(); },
	                         error);
}

const bool DFA_simulate(DFAHandle dfa, const char **input, const size_t length, const int simulationDepth,
                        AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    return reinterpret_cast<DeterministicFiniteAutomaton *>(dfa)->simulate(input_vec, simulationDepth);
	    },
	    error);
}
