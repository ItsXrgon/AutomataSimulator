#include "AutomataSimulator/c_api/NonDeterministicPushdownAutomaton_c.h"

struct NPDA_t {
	NonDeterministicPushdownAutomaton instance;
};

NPDAHandle NPDA_create(AutomatonError *error) {
	return wrap_result<NPDAHandle>(
	    [&]() { return reinterpret_cast<NPDAHandle>(new NonDeterministicPushdownAutomaton()); }, error, {});
}

void NPDA_destroy(NPDAHandle npda, AutomatonError *error) {
	wrap_result([&]() { delete reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda); }, error);
}

const StringArray NPDA_getInput(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getInput()); },
	    error);
}

void NPDA_setInput(NPDAHandle npda, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->setInput(input_vec);
	    },
	    error);
}

void NPDA_addInput(NPDAHandle npda, const char **input, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->addInput(input_vec);
	    },
	    error);
}

const int NPDA_getInputHead(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<int>(
	    [&]() { return reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getInputHead(); }, error);
}

void NPDA_setInputHead(NPDAHandle npda, const int head, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->setInputHead(head); }, error);
}

const bool NPDA_stateExists(NPDAHandle npda, const char *key, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->stateExists(key); }, error);
}

const bool NPDA_inputAlphabetSymbolExists(NPDAHandle npda, const char *symbol, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    return reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->inputAlphabetSymbolExists(symbol);
	    },
	    error);
}

void NPDA_addState(NPDAHandle npda, const char *label, const bool isAccept, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->addState(label, isAccept); },
	            error);
}

void NPDA_updateStateLabel(NPDAHandle npda, const char *key, const char *label, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->updateStateLabel(key, label); },
	            error);
}

const char *NPDA_getCurrentState(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() {
		    return convertToCString(reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getCurrentState());
	    },
	    error);
}

void NPDA_setCurrentState(NPDAHandle npda, const char *state, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->setCurrentState(state); }, error);
}

const PDAStateHandle NPDA_getState(NPDAHandle npda, const char *key, AutomatonError *error) {
	return wrap_result<PDAStateHandle>(
	    [&]() {
		    return reinterpret_cast<PDAStateHandle>(
		        new PDAState(reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getState(key)));
	    },
	    error);
}

const PDAStateArray NPDA_getStates(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<PDAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getStates();

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

void NPDA_removeState(NPDAHandle npda, const char *key, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeState(key, strict); },
	            error);
}

void NPDA_removeStates(NPDAHandle npda, const char **keys, const size_t length, const bool strict,
                       AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeStates(keys_vec, strict);
	    },
	    error);
}

void NPDA_clearStates(NPDAHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearStates(); }, error);
}

void NPDA_setInputAlphabet(NPDAHandle npda, const char **inputAlphabet, const size_t length, const bool strict,
                           AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->setInputAlphabet(alphabet, strict);
	    },
	    error);
}

void NPDA_addInputAlphabet(NPDAHandle npda, const char **inputAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->addInputAlphabet(alphabet);
	    },
	    error);
}

const StringArray NPDA_getInputAlphabet(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(
		        reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getInputAlphabet());
	    },
	    error);
}

void NPDA_removeInputAlphabetSymbol(NPDAHandle npda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeInputAlphabetSymbol(symbol, strict);
	    },
	    error);
}

void NPDA_removeInputAlphabetSymbols(NPDAHandle npda, const char **symbols, const size_t length, const bool strict,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeInputAlphabetSymbols(symbols_vec,
		                                                                                            strict);
	    },
	    error);
}

void NPDA_clearInputAlphabet(NPDAHandle npda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearInputAlphabet(strict); },
	            error);
}

void NPDA_setStackAlphabet(NPDAHandle npda, const char **stackAlphabet, const size_t length, const bool strict,
                           AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->setStackAlphabet(alphabet, strict);
	    },
	    error);
}

void NPDA_addStackAlphabet(NPDAHandle npda, const char **stackAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->addStackAlphabet(alphabet);
	    },
	    error);
}

const StringArray NPDA_getStackAlphabet(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(
		        reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getStackAlphabet());
	    },
	    error);
}

void NPDA_removeStackAlphabetSymbol(NPDAHandle npda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeStackAlphabetSymbol(symbol, strict);
	    },
	    error);
}

void NPDA_removeStackAlphabetSymbols(NPDAHandle npda, const char **symbols, const size_t length, const bool strict,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeStackAlphabetSymbols(symbols_vec,
		                                                                                            strict);
	    },
	    error);
}

void NPDA_clearStackAlphabet(NPDAHandle npda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearStackAlphabet(strict); },
	            error);
}

const char *NPDA_getStartState(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() {
		    return convertToCString(reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getStartState());
	    },
	    error);
}

void NPDA_setStartState(NPDAHandle npda, const char *key, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->setStartState(key); }, error);
}

PDATransitionHandle NPDA_getTransition(NPDAHandle npda, const char *key, AutomatonError *error) {
	return wrap_result<PDATransitionHandle>(
	    [&]() {
		    return reinterpret_cast<PDATransitionHandle>(
		        new PDATransition(reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getTransition(key)));
	    },
	    error);
}

void NPDA_addTransition(NPDAHandle dpda, const char *fromStateKey, const char *toStateKey, const char *input,
                        const char *stackSymbol, const char *pushSymbol, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda)->addTransition(fromStateKey, toStateKey, input,
		                                                                               stackSymbol, pushSymbol);
	    },
	    error);
}

void NPDA_updateTransitionInput(NPDAHandle npda, const char *transitionKey, const char *input, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->updateTransitionInput(transitionKey, input);
	    },
	    error);
}

void NPDA_updateTransitionFromState(NPDAHandle npda, const char *transitionKey, const char *fromStateKey,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->updateTransitionFromState(transitionKey,
		                                                                                           fromStateKey);
	    },
	    error);
}

void NPDA_updateTransitionToState(NPDAHandle npda, const char *transitionKey, const char *toStateKey,
                                  AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->updateTransitionToState(transitionKey,
		                                                                                         toStateKey);
	    },
	    error);
}

void NPDA_updateTransitionStackSymbol(NPDAHandle npda, const char *transitionKey, const char *stackSymbol,
                                      AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->updateTransitionStackSymbol(transitionKey,
		                                                                                             stackSymbol);
	    },
	    error);
}

void NPDA_updateTransitionPushSymbol(NPDAHandle npda, const char *transitionKey, const char *pushSymbol,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->updateTransitionPushSymbol(transitionKey,
		                                                                                            pushSymbol);
	    },
	    error);
}

void NPDA_removeTransition(NPDAHandle npda, const char *transitionKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeTransition(transitionKey); },
	            error);
}

void NPDA_clearTransitionsBetween(NPDAHandle npda, const char *fromStateKey, const char *toStateKey,
                                  AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearTransitionsBetween(fromStateKey,
		                                                                                         toStateKey);
	    },
	    error);
}

void NPDA_clearStateTransitions(NPDAHandle npda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearStateTransitions(stateKey); },
	            error);
}

void NPDA_clearTransitions(NPDAHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearTransitions(); }, error);
}

void NPDA_addAcceptState(NPDAHandle npda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->addAcceptState(stateKey); },
	            error);
}

void NPDA_addAcceptStates(NPDAHandle npda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->addAcceptStates(keys_vec);
	    },
	    error);
}

void NPDA_removeAcceptState(NPDAHandle npda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeAcceptState(stateKey); },
	            error);
}

void NPDA_removeAcceptStates(NPDAHandle npda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->removeAcceptStates(keys_vec);
	    },
	    error);
}

void NPDA_clearAcceptStates(NPDAHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->clearAcceptStates(); }, error);
}

const PDAStateArray NPDA_getAcceptStates(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<PDAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getAcceptStates();

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

void NPDA_reset(NPDAHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->reset(); }, error);
}

const bool NPDA_isAccepting(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->isAccepting(); }, error);
}

bool NPDA_processInput(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->processInput(); }, error);
}

const bool NPDA_simulate(NPDAHandle npda, const char **input, const size_t length, const int simulationDepth,
                         AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    std::vector<std::string> input_vec(input, input + length);
		    return reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->simulate(input_vec, simulationDepth);
	    },
	    error);
}

const PDAStateArray NPDA_getPossibleCurrentStates(NPDAHandle npda, AutomatonError *error) {
	return wrap_result<PDAStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicPushdownAutomaton *>(npda)->getPossibleCurrentStates();

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

StringArray NPDA_getStack(NPDAHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    auto *automaton = reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda);

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

void NPDA_pushStack(NPDAHandle dpda, const char *symbol, AutomatonError *error) {
	return wrap_result(
	    [&]() {
		    auto *automaton = reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda);
		    automaton->pushStack(symbol);
	    },
	    error);
}

const char *NPDA_popStack(NPDAHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() {
		    auto *automaton = reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda);
		    std::string popped = automaton->popStack();

		    return convertToCString(popped);
	    },
	    error);
}

const char *NPDA_peekStack(NPDAHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() {
		    auto *automaton = reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda);
		    const std::string &top = automaton->peekStack();

		    return convertToCString(top);
	    },
	    error);
}

void NPDA_resetStack(NPDAHandle dpda, AutomatonError *error) {
	return wrap_result(
	    [&]() {
		    auto *automaton = reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda);
		    automaton->resetStack();
	    },
	    error);
}

const bool NPDA_checkNextState(NPDAHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<const bool>(
	    [&]() { return reinterpret_cast<NonDeterministicPushdownAutomaton *>(dpda)->checkNextState(key); }, error);
}