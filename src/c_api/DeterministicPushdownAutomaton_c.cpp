#include "AutomataSimulator/c_api/DeterministicPushdownAutomaton_c.h"

struct DPDA_t {
	DeterministicPushdownAutomaton instance;
};

DPDAHandle DPDA_create(AutomatonError *error) {
	return wrap_result<DPDAHandle>([&]() { return reinterpret_cast<DPDAHandle>(new DeterministicPushdownAutomaton()); },
	                               error, {});
}

void DPDA_destroy(DPDAHandle dpda, AutomatonError *error) {
	wrap_result([&]() { delete reinterpret_cast<DeterministicPushdownAutomaton *>(dpda); }, error);
}

const StringArray DPDA_getInput(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getInput()); },
	    error);
}

void DPDA_setInput(DPDAHandle dpda, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->setInput(input_vec);
	    },
	    error);
}

void DPDA_addInput(DPDAHandle dpda, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addInput(input_vec);
	    },
	    error);
}

const int DPDA_getInputHead(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<int>([&]() { return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getInputHead(); },
	                        error);
}

void DPDA_setInputHead(DPDAHandle dpda, const int head, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->setInputHead(head); }, error);
}

const bool DPDA_stateExists(DPDAHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->stateExists(key); }, error);
}

const bool DPDA_inputAlphabetSymbolExists(DPDAHandle dpda, const char *symbol, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->inputAlphabetSymbolExists(symbol); },
	    error);
}

void DPDA_addState(DPDAHandle dpda, const char *label, const bool isAccept, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addState(label, isAccept); }, error);
}

void DPDA_updateStateLabel(DPDAHandle dpda, const char *key, const char *label, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->updateStateLabel(key, label); },
	            error);
}

const char *DPDA_getCurrentState(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getCurrentState()); },
	    error);
}

void DPDA_setCurrentState(DPDAHandle dpda, const char *state, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->setCurrentState(state); }, error);
}

const PDAStateHandle DPDA_getState(DPDAHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<PDAStateHandle>(
	    [&]() {
		    return reinterpret_cast<PDAStateHandle>(
		        new PDAState(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getState(key)));
	    },
	    error);
}

const PDAStateArray DPDA_getStates(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<PDAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getStates();

		    PDAStateArray array;
		    array.length = states.size();
		    array.data = new PDAStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<PDAStateHandle>(new PDAState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void DPDA_removeState(DPDAHandle dpda, const char *key, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeState(key, strict); }, error);
}

void DPDA_removeStates(DPDAHandle dpda, const char **keys, const size_t length, const bool strict,
                       AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeStates(keys_vec, strict);
	    },
	    error);
}

void DPDA_clearStates(DPDAHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearStates(); }, error);
}

void DPDA_setInputAlphabet(DPDAHandle dpda, const char **inputAlphabet, const size_t length, const bool strict,
                           AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->setInputAlphabet(alphabet, strict);
	    },
	    error);
}

void DPDA_addInputAlphabet(DPDAHandle dpda, const char **inputAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addInputAlphabet(alphabet);
	    },
	    error);
}

const StringArray DPDA_getInputAlphabet(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getInputAlphabet());
	    },
	    error);
}

void DPDA_removeInputAlphabetSymbol(DPDAHandle dpda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeInputAlphabetSymbol(symbol, strict); },
	    error);
}

void DPDA_removeInputAlphabetSymbols(DPDAHandle dpda, const char **symbols, const size_t length, const bool strict,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeInputAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void DPDA_clearInputAlphabet(DPDAHandle dpda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearInputAlphabet(strict); }, error);
}

void DPDA_setStackAlphabet(DPDAHandle dpda, const char **stackAlphabet, const size_t length, const bool strict,
                           AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->setStackAlphabet(alphabet, strict);
	    },
	    error);
}

void DPDA_addStackAlphabet(DPDAHandle dpda, const char **stackAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addStackAlphabet(alphabet);
	    },
	    error);
}

const StringArray DPDA_getStackAlphabet(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getStackAlphabet());
	    },
	    error);
}

void DPDA_removeStackAlphabetSymbol(DPDAHandle dpda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeStackAlphabetSymbol(symbol, strict); },
	    error);
}

void DPDA_removeStackAlphabetSymbols(DPDAHandle dpda, const char **symbols, const size_t length, const bool strict,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeStackAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void DPDA_clearStackAlphabet(DPDAHandle dpda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearStackAlphabet(strict); }, error);
}

const char *DPDA_getStartState(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getStartState()); },
	    error);
}

void DPDA_setStartState(DPDAHandle dpda, const char *key, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->setStartState(key); }, error);
}

void DPDA_addTransition(DPDAHandle dpda, const char *fromStateKey, const char *toStateKey, const char *input,
                        const char *stackSymbol, const char *pushSymbol, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addTransition(fromStateKey, toStateKey, input,
		                                                                            stackSymbol, pushSymbol);
	    },
	    error);
}

PDATransitionHandle DPDA_getTransition(DPDAHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<PDATransitionHandle>(
	    [&]() {
		    return reinterpret_cast<PDATransitionHandle>(
		        new PDATransition(reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getTransition(key)));
	    },
	    error);
}

void DPDA_updateTransitionInput(DPDAHandle dpda, const char *transitionKey, const char *input, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->updateTransitionInput(transitionKey, input);
	    },
	    error);
}

void DPDA_updateTransitionFromState(DPDAHandle dpda, const char *transitionKey, const char *fromStateKey,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->updateTransitionFromState(transitionKey,
		                                                                                        fromStateKey);
	    },
	    error);
}

void DPDA_updateTransitionToState(DPDAHandle dpda, const char *transitionKey, const char *toStateKey,
                                  AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->updateTransitionToState(transitionKey,
		                                                                                      toStateKey);
	    },
	    error);
}

void DPDA_updateTransitionStackSymbol(DPDAHandle npda, const char *transitionKey, const char *stackSymbol,
                                      AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(npda)->updateTransitionStackSymbol(transitionKey,
		                                                                                          stackSymbol);
	    },
	    error);
}

void DPDA_updateTransitionPushSymbol(DPDAHandle npda, const char *transitionKey, const char *pushSymbol,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(npda)->updateTransitionPushSymbol(transitionKey,
		                                                                                         pushSymbol);
	    },
	    error);
}

void DPDA_removeTransition(DPDAHandle dpda, const char *transitionKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeTransition(transitionKey); },
	            error);
}

void DPDA_clearTransitionsBetween(DPDAHandle dpda, const char *fromStateKey, const char *toStateKey,
                                  AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearTransitionsBetween(fromStateKey, toStateKey);
	    },
	    error);
}

void DPDA_clearStateTransitions(DPDAHandle dpda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearStateTransitions(stateKey); },
	            error);
}

void DPDA_clearTransitions(DPDAHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearTransitions(); }, error);
}

void DPDA_addAcceptState(DPDAHandle dpda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addAcceptState(stateKey); }, error);
}

void DPDA_addAcceptStates(DPDAHandle dpda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->addAcceptStates(keys_vec);
	    },
	    error);
}

void DPDA_removeAcceptState(DPDAHandle dpda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeAcceptState(stateKey); },
	            error);
}

void DPDA_removeAcceptStates(DPDAHandle dpda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->removeAcceptStates(keys_vec);
	    },
	    error);
}

void DPDA_clearAcceptStates(DPDAHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->clearAcceptStates(); }, error);
}

const PDAStateArray DPDA_getAcceptStates(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<PDAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->getAcceptStates();

		    PDAStateArray array;
		    array.length = states.size();
		    array.data = new PDAStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<PDAStateHandle>(new PDAState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void DPDA_reset(DPDAHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->reset(); }, error);
}

const bool DPDA_isAccepting(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->isAccepting(); },
	                         error);
}

bool DPDA_processInput(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->processInput(); },
	                         error);
}

const bool DPDA_simulate(DPDAHandle dpda, const char **input, const size_t length, const int simulationDepth,
                         AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->simulate(input_vec, simulationDepth);
	    },
	    error);
}

StringArray DPDA_getStack(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    auto *automaton = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda);

		    std::vector<std::string> stackContents;

		    std::stack<std::string> tempStack = automaton->getStack();
		    while (!tempStack.empty()) {
			    stackContents.push_back(tempStack.top());
			    tempStack.pop();
		    }
		    std::reverse(stackContents.begin(), stackContents.end());
		    return convertToStringArray(stackContents);
	    },
	    error);
}

void DPDA_pushStack(DPDAHandle dpda, const char *symbol, AutomatonError *error) {
	return wrap_result(
	    [&]() {
		    auto *automaton = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda);
		    automaton->pushStack(symbol);
	    },
	    error);
}

const char *DPDA_popStack(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() {
		    auto *automaton = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda);
		    std::string popped = automaton->popStack();

		    return convertToCString(popped);
	    },
	    error);
}

const char *DPDA_peekStack(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() {
		    auto *automaton = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda);
		    const std::string &top = automaton->peekStack();

		    return convertToCString(top);
	    },
	    error);
}

void DPDA_resetStack(DPDAHandle dpda, AutomatonError *error) {
	return wrap_result(
	    [&]() {
		    auto *automaton = reinterpret_cast<DeterministicPushdownAutomaton *>(dpda);
		    automaton->resetStack();
	    },
	    error);
}


const bool DPDA_checkNextState(DPDAHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<const bool>(
	    [&]() { return reinterpret_cast<DeterministicPushdownAutomaton *>(dpda)->checkNextState(key); }, error);
}