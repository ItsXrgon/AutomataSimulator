#include "AutomataSimulator/c_api/NonDeterministicTuringMachine_c.h"

struct NTM_t {
	NonDeterministicTuringMachine instance;
};

NTMHandle NTM_create(AutomatonError *error) {
	return wrap_result<NTMHandle>([&]() { return reinterpret_cast<NTMHandle>(new NonDeterministicTuringMachine()); },
	                              error, {});
}

void NTM_destroy(NTMHandle npda, AutomatonError *error) {
	wrap_result([&]() { delete reinterpret_cast<NonDeterministicTuringMachine *>(npda); }, error);
}

const StringArray NTM_getInput(NTMHandle npda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getInput()); },
	    error);
}

void NTM_setInput(NTMHandle npda, const char **readSymbol, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(readSymbol, readSymbol + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->setInput(input_vec);
	    },
	    error);
}

void NTM_addInput(NTMHandle npda, const char **readSymbol, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(readSymbol, readSymbol + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->addInput(input_vec);
	    },
	    error);
}

const bool NTM_stateExists(NTMHandle npda, const char *key, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicTuringMachine *>(npda)->stateExists(key); }, error);
}

const bool NTM_inputAlphabetSymbolExists(NTMHandle npda, const char *symbol, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<NonDeterministicTuringMachine *>(npda)->inputAlphabetSymbolExists(symbol); },
	    error);
}

void NTM_addState(NTMHandle npda, const char *label, const bool isAccept, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->addState(label, isAccept); }, error);
}

void NTM_updateStateLabel(NTMHandle npda, const char *key, const char *label, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->updateStateLabel(key, label); },
	            error);
}

const char *NTM_getCurrentState(NTMHandle npda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getCurrentState()); },
	    error);
}

void NTM_setCurrentState(NTMHandle npda, const char *state, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->setCurrentState(state); }, error);
}

const TMStateHandle NTM_getState(NTMHandle npda, const char *key, AutomatonError *error) {
	return wrap_result<TMStateHandle>(
	    [&]() {
		    return reinterpret_cast<TMStateHandle>(
		        new TMState(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getState(key)));
	    },
	    error);
}

const TMStateArray NTM_getStates(NTMHandle npda, AutomatonError *error) {
	return wrap_result<TMStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getStates();

		    TMStateArray array;
		    array.length = states.size();
		    array.data = new TMStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<TMStateHandle>(new TMState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void NTM_removeState(NTMHandle npda, const char *key, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeState(key, strict); }, error);
}

void NTM_removeStates(NTMHandle npda, const char **keys, const size_t length, const bool strict,
                      AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeStates(keys_vec, strict);
	    },
	    error);
}

void NTM_clearStates(NTMHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearStates(); }, error);
}

void NTM_setInputAlphabet(NTMHandle npda, const char **inputAlphabet, const size_t length, const bool strict,
                          AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->setInputAlphabet(alphabet, strict);
	    },
	    error);
}

void NTM_addInputAlphabet(NTMHandle npda, const char **inputAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->addInputAlphabet(alphabet);
	    },
	    error);
}

const StringArray NTM_getInputAlphabet(NTMHandle npda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getInputAlphabet());
	    },
	    error);
}

void NTM_removeInputAlphabetSymbol(NTMHandle npda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeInputAlphabetSymbol(symbol, strict); },
	    error);
}

void NTM_removeInputAlphabetSymbols(NTMHandle npda, const char **symbols, const size_t length, const bool strict,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeInputAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void NTM_clearInputAlphabet(NTMHandle npda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearInputAlphabet(strict); }, error);
}

void NTM_setTapeAlphabet(NTMHandle npda, const char **stackAlphabet, const size_t length, const bool strict,
                         AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->setTapeAlphabet(alphabet, strict);
	    },
	    error);
}

void NTM_addTapeAlphabet(NTMHandle npda, const char **stackAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->addTapeAlphabet(alphabet);
	    },
	    error);
}

const StringArray NTM_getTapeAlphabet(NTMHandle npda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getTapeAlphabet());
	    },
	    error);
}

void NTM_removeTapeAlphabetSymbol(NTMHandle npda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeTapeAlphabetSymbol(symbol, strict); },
	    error);
}

void NTM_removeTapeAlphabetSymbols(NTMHandle npda, const char **symbols, const size_t length, const bool strict,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeTapeAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void NTM_clearTapeAlphabet(NTMHandle npda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearTapeAlphabet(strict); }, error);
}

const char *NTM_getStartState(NTMHandle npda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getStartState()); },
	    error);
}

void NTM_setStartState(NTMHandle npda, const char *key, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->setStartState(key); }, error);
}

TMTransitionHandle NTM_getTransition(NTMHandle npda, const char *key, AutomatonError *error) {
	return wrap_result<TMTransitionHandle>(
	    [&]() {
		    return reinterpret_cast<TMTransitionHandle>(
		        new TMTransition(reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getTransition(key)));
	    },
	    error);
}

void NTM_addTransition(NTMHandle dpda, const char *fromStateKey, const char *toStateKey, const char *readSymbol,
                       const char *writeSymbol, const char *direction, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    const TMDirection dir = TMDirectionHelper::fromString(direction);
		    reinterpret_cast<NonDeterministicTuringMachine *>(dpda)->addTransition(fromStateKey, toStateKey, readSymbol,
		                                                                           writeSymbol, dir);
	    },
	    error);
}

void NTM_updateTransitionReadSymbol(NTMHandle npda, const char *transitionKey, const char *readSymbol,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->updateTransitionReadSymbol(transitionKey,
		                                                                                        readSymbol);
	    },
	    error);
}

void NTM_updateTransitionFromState(NTMHandle npda, const char *transitionKey, const char *fromStateKey,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->updateTransitionFromState(transitionKey,
		                                                                                       fromStateKey);
	    },
	    error);
}

void NTM_updateTransitionToState(NTMHandle npda, const char *transitionKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->updateTransitionToState(transitionKey, toStateKey);
	    },
	    error);
}

void NTM_updateTransitionWriteSymbol(NTMHandle npda, const char *transitionKey, const char *writeSymbol,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->updateTransitionWriteSymbol(transitionKey,
		                                                                                         writeSymbol);
	    },
	    error);
}

void NTM_updateTransitionDirection(NTMHandle npda, const char *transitionKey, const char *direction,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    const TMDirection dir = TMDirectionHelper::fromString(direction);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->updateTransitionDirection(transitionKey, dir);
	    },
	    error);
}

void NTM_removeTransition(NTMHandle npda, const char *transitionKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeTransition(transitionKey); },
	            error);
}

void NTM_clearTransitionsBetween(NTMHandle npda, const char *fromStateKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearTransitionsBetween(fromStateKey, toStateKey);
	    },
	    error);
}

void NTM_clearStateTransitions(NTMHandle npda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearStateTransitions(stateKey); },
	            error);
}

void NTM_clearTransitions(NTMHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearTransitions(); }, error);
}

void NTM_addAcceptState(NTMHandle npda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->addAcceptState(stateKey); }, error);
}

void NTM_addAcceptStates(NTMHandle npda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->addAcceptStates(keys_vec);
	    },
	    error);
}

void NTM_removeAcceptState(NTMHandle npda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeAcceptState(stateKey); }, error);
}

void NTM_removeAcceptStates(NTMHandle npda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<NonDeterministicTuringMachine *>(npda)->removeAcceptStates(keys_vec);
	    },
	    error);
}

void NTM_clearAcceptStates(NTMHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->clearAcceptStates(); }, error);
}

const TMStateArray NTM_getAcceptStates(NTMHandle npda, AutomatonError *error) {
	return wrap_result<TMStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getAcceptStates();

		    TMStateArray array;
		    array.length = states.size();
		    array.data = new TMStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<TMStateHandle>(new TMState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void NTM_reset(NTMHandle npda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(npda)->reset(); }, error);
}

const bool NTM_isAccepting(NTMHandle npda, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<NonDeterministicTuringMachine *>(npda)->isAccepting(); },
	                         error);
}

bool NTM_processInput(NTMHandle npda, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<NonDeterministicTuringMachine *>(npda)->processInput(); },
	                         error);
}

const bool NTM_simulate(NTMHandle npda, const char **readSymbol, const size_t length, const int simulationDepth,
                        AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    std::vector<std::string> input_vec(readSymbol, readSymbol + length);
		    return reinterpret_cast<NonDeterministicTuringMachine *>(npda)->simulate(input_vec, simulationDepth);
	    },
	    error);
}

const TMStateArray NTM_getPossibleCurrentStates(NTMHandle npda, AutomatonError *error) {
	return wrap_result<TMStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<NonDeterministicTuringMachine *>(npda)->getPossibleCurrentStates();

		    TMStateArray array;
		    array.length = states.size();
		    array.data = new TMStateHandle[array.length];

		    for (size_t i = 0; i < array.length; ++i) {
			    array.data[i] = reinterpret_cast<TMStateHandle>(new TMState(states[i]));
		    }

		    return array;
	    },
	    error);
}

void NTM_setTape(NTMHandle ntm, const char **&tape, const size_t &length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> vec(tape, tape + length);
		    std::list<std::string> tapeList(vec.begin(), vec.end());
		    reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->setTape(tapeList);
	    },
	    error);
}

const StringArray NTM_getTape(NTMHandle ntm, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    const auto &tapeList = reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->getTape();
		    std::vector<std::string> vec(tapeList.begin(), tapeList.end());
		    return convertToStringArray(vec);
	    },
	    error);
}

void NTM_setTapeHead(NTMHandle ntm, int headIndex, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->setTapeHead(headIndex); }, error);
}

const int NTM_getTapeHead(NTMHandle ntm, AutomatonError *error) {
	return wrap_result<int>([&]() { return reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->getTapehead(); },
	                        error);
}

void NTM_resetTape(NTMHandle ntm, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->resetTape(); }, error);
}

void NTM_moveTapeHead(NTMHandle ntm, char *direction, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    const TMDirection &dir = TMDirectionHelper::fromString(direction);
		    reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->moveTapeHead(dir);
	    },
	    error);
}

void NTM_writeTape(NTMHandle ntm, char *symbol, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->writeTape(symbol); }, error);
}

const char *NTM_readTape(NTMHandle ntm, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->readTape()); }, error);
}

const bool NTM_checkNextState(NTMHandle ntm, const char *key, AutomatonError *error) {
	return wrap_result<const bool>(
	    [&]() { return reinterpret_cast<NonDeterministicTuringMachine *>(ntm)->checkNextState(key); }, error);
}