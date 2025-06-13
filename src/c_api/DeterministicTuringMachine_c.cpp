#include "AutomataSimulator/c_api/DeterministicTuringMachine_c.h"

struct DTM_t {
	DeterministicTuringMachine instance;
};

DTMHandle DTM_create(AutomatonError *error) {
	return wrap_result<DTMHandle>([&]() { return reinterpret_cast<DTMHandle>(new DeterministicTuringMachine()); },
	                              error, {});
}

void DTM_destroy(DTMHandle dpda, AutomatonError *error) {
	wrap_result([&]() { delete reinterpret_cast<DeterministicTuringMachine *>(dpda); }, error);
}

const StringArray DTM_getInput(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getInput()); },
	    error);
}

void DTM_setInput(DTMHandle dpda, const char **readSymbol, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(readSymbol, readSymbol + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->setInput(input_vec);
	    },
	    error);
}

void DTM_addInput(DTMHandle dpda, const char **readSymbol, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> input_vec(readSymbol, readSymbol + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->addInput(input_vec);
	    },
	    error);
}

const bool DTM_stateExists(DTMHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicTuringMachine *>(dpda)->stateExists(key); },
	                         error);
}

const bool DTM_inputAlphabetSymbolExists(DTMHandle dpda, const char *symbol, AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() { return reinterpret_cast<DeterministicTuringMachine *>(dpda)->inputAlphabetSymbolExists(symbol); },
	    error);
}

void DTM_addState(DTMHandle dpda, const char *label, const bool isAccept, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->addState(label, isAccept); }, error);
}

void DTM_updateStateLabel(DTMHandle dpda, const char *key, const char *label, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->updateStateLabel(key, label); }, error);
}

const char *DTM_getCurrentState(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getCurrentState()); },
	    error);
}

void DTM_setCurrentState(DTMHandle dpda, const char *state, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->setCurrentState(state); }, error);
}

const TMStateHandle DTM_getState(DTMHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<TMStateHandle>(
	    [&]() {
		    return reinterpret_cast<TMStateHandle>(
		        new TMState(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getState(key)));
	    },
	    error);
}

const TMStateArray DTM_getStates(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<TMStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<DeterministicTuringMachine *>(dpda)->getStates();

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

void DTM_removeState(DTMHandle dpda, const char *key, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeState(key, strict); }, error);
}

void DTM_removeStates(DTMHandle dpda, const char **keys, const size_t length, const bool strict,
                      AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeStates(keys_vec, strict);
	    },
	    error);
}

void DTM_clearStates(DTMHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearStates(); }, error);
}

void DTM_setInputAlphabet(DTMHandle dpda, const char **inputAlphabet, const size_t length, const bool strict,
                          AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->setInputAlphabet(alphabet, strict);
	    },
	    error);
}

void DTM_addInputAlphabet(DTMHandle dpda, const char **inputAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(inputAlphabet, inputAlphabet + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->addInputAlphabet(alphabet);
	    },
	    error);
}

const StringArray DTM_getInputAlphabet(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    return convertToStringArray(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getInputAlphabet());
	    },
	    error);
}

void DTM_removeInputAlphabetSymbol(DTMHandle dpda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeInputAlphabetSymbol(symbol, strict); },
	    error);
}

void DTM_removeInputAlphabetSymbols(DTMHandle dpda, const char **symbols, const size_t length, const bool strict,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeInputAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void DTM_clearInputAlphabet(DTMHandle dpda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearInputAlphabet(strict); }, error);
}

void DTM_setTapeAlphabet(DTMHandle dpda, const char **stackAlphabet, const size_t length, const bool strict,
                         AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->setTapeAlphabet(alphabet, strict);
	    },
	    error);
}

void DTM_addTapeAlphabet(DTMHandle dpda, const char **stackAlphabet, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> alphabet(stackAlphabet, stackAlphabet + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->addTapeAlphabet(alphabet);
	    },
	    error);
}

const StringArray DTM_getTapeAlphabet(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() { return convertToStringArray(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getTapeAlphabet()); },
	    error);
}

void DTM_removeTapeAlphabetSymbol(DTMHandle dpda, const char *symbol, const bool strict, AutomatonError *error) {
	wrap_result(
	    [&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeTapeAlphabetSymbol(symbol, strict); },
	    error);
}

void DTM_removeTapeAlphabetSymbols(DTMHandle dpda, const char **symbols, const size_t length, const bool strict,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> symbols_vec(symbols, symbols + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeTapeAlphabetSymbols(symbols_vec, strict);
	    },
	    error);
}

void DTM_clearTapeAlphabet(DTMHandle dpda, const bool strict, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearTapeAlphabet(strict); }, error);
}

const char *DTM_getStartState(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getStartState()); },
	    error);
}

void DTM_setStartState(DTMHandle dpda, const char *key, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->setStartState(key); }, error);
}

void DTM_addTransition(DTMHandle dpda, const char *fromStateKey, const char *toStateKey, const char *readSymbol,
                       const char *writeSymbol, const char *direction, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    const TMDirection dir = TMDirectionHelper::fromString(direction);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->addTransition(fromStateKey, toStateKey, readSymbol,
		                                                                        writeSymbol, dir);
	    },
	    error);
}

TMTransitionHandle DTM_getTransition(DTMHandle dpda, const char *key, AutomatonError *error) {
	return wrap_result<TMTransitionHandle>(
	    [&]() {
		    return reinterpret_cast<TMTransitionHandle>(
		        new TMTransition(reinterpret_cast<DeterministicTuringMachine *>(dpda)->getTransition(key)));
	    },
	    error);
}

void DTM_updateTransitionReadSymbol(DTMHandle dpda, const char *transitionKey, const char *readSymbol,
                                    AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->updateTransitionReadSymbol(transitionKey, readSymbol);
	    },
	    error);
}

void DTM_updateTransitionFromState(DTMHandle dpda, const char *transitionKey, const char *fromStateKey,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->updateTransitionFromState(transitionKey,
		                                                                                    fromStateKey);
	    },
	    error);
}

void DTM_updateTransitionToState(DTMHandle dpda, const char *transitionKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->updateTransitionToState(transitionKey, toStateKey);
	    },
	    error);
}

void DTM_updateTransitionWriteSymbol(DTMHandle npda, const char *transitionKey, const char *writeSymbol,
                                     AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicTuringMachine *>(npda)->updateTransitionWriteSymbol(transitionKey,
		                                                                                      writeSymbol);
	    },
	    error);
}

void DTM_updateTransitionDirection(DTMHandle npda, const char *transitionKey, const char *direction,
                                   AutomatonError *error) {
	wrap_result(
	    [&]() {
		    const TMDirection dir = TMDirectionHelper::fromString(direction);
		    reinterpret_cast<DeterministicTuringMachine *>(npda)->updateTransitionDirection(transitionKey, dir);
	    },
	    error);
}

void DTM_removeTransition(DTMHandle dpda, const char *transitionKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeTransition(transitionKey); },
	            error);
}

void DTM_clearTransitionsBetween(DTMHandle dpda, const char *fromStateKey, const char *toStateKey,
                                 AutomatonError *error) {
	wrap_result(
	    [&]() {
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearTransitionsBetween(fromStateKey, toStateKey);
	    },
	    error);
}

void DTM_clearStateTransitions(DTMHandle dpda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearStateTransitions(stateKey); },
	            error);
}

void DTM_clearTransitions(DTMHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearTransitions(); }, error);
}

void DTM_addAcceptState(DTMHandle dpda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->addAcceptState(stateKey); }, error);
}

void DTM_addAcceptStates(DTMHandle dpda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->addAcceptStates(keys_vec);
	    },
	    error);
}

void DTM_removeAcceptState(DTMHandle dpda, const char *stateKey, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeAcceptState(stateKey); }, error);
}

void DTM_removeAcceptStates(DTMHandle dpda, const char **keys, const size_t length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> keys_vec(keys, keys + length);
		    reinterpret_cast<DeterministicTuringMachine *>(dpda)->removeAcceptStates(keys_vec);
	    },
	    error);
}

void DTM_clearAcceptStates(DTMHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->clearAcceptStates(); }, error);
}

const TMStateArray DTM_getAcceptStates(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<TMStateArray>(
	    [&]() {
		    auto states = reinterpret_cast<DeterministicTuringMachine *>(dpda)->getAcceptStates();

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

void DTM_reset(DTMHandle dpda, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dpda)->reset(); }, error);
}

const bool DTM_isAccepting(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicTuringMachine *>(dpda)->isAccepting(); },
	                         error);
}

bool DTM_processInput(DTMHandle dpda, AutomatonError *error) {
	return wrap_result<bool>([&]() { return reinterpret_cast<DeterministicTuringMachine *>(dpda)->processInput(); },
	                         error);
}

const bool DTM_simulate(DTMHandle dpda, const char **readSymbol, const size_t length, const int simulationDepth,
                        AutomatonError *error) {
	return wrap_result<bool>(
	    [&]() {
		    std::vector<std::string> input_vec(readSymbol, readSymbol + length);
		    return reinterpret_cast<DeterministicTuringMachine *>(dpda)->simulate(input_vec, simulationDepth);
	    },
	    error);
}

void DTM_setTape(DTMHandle dtm, const char **&tape, const size_t &length, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    std::vector<std::string> vec(tape, tape + length);
		    std::list<std::string> tapeList(vec.begin(), vec.end());
		    reinterpret_cast<DeterministicTuringMachine *>(dtm)->setTape(tapeList);
	    },
	    error);
}

const StringArray DTM_getTape(DTMHandle dtm, AutomatonError *error) {
	return wrap_result<StringArray>(
	    [&]() {
		    const auto &tapeList = reinterpret_cast<DeterministicTuringMachine *>(dtm)->getTape();
		    std::vector<std::string> vec(tapeList.begin(), tapeList.end());
		    return convertToStringArray(vec);
	    },
	    error);
}

void DTM_setTapeHead(DTMHandle dtm, int headIndex, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dtm)->setTapeHead(headIndex); }, error);
}

const int DTM_getTapeHead(DTMHandle dtm, AutomatonError *error) {
	return wrap_result<int>([&]() { return reinterpret_cast<DeterministicTuringMachine *>(dtm)->getTapehead(); },
	                        error);
}

void DTM_resetTape(DTMHandle dtm, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dtm)->resetTape(); }, error);
}

void DTM_moveTapeHead(DTMHandle dtm, char *direction, AutomatonError *error) {
	wrap_result(
	    [&]() {
		    const TMDirection &dir = TMDirectionHelper::fromString(direction);
		    reinterpret_cast<DeterministicTuringMachine *>(dtm)->moveTapeHead(dir);
	    },
	    error);
}

void DTM_writeTape(DTMHandle dtm, char *symbol, AutomatonError *error) {
	wrap_result([&]() { reinterpret_cast<DeterministicTuringMachine *>(dtm)->writeTape(symbol); }, error);
}

const char *DTM_readTape(DTMHandle dtm, AutomatonError *error) {
	return wrap_result<const char *>(
	    [&]() { return convertToCString(reinterpret_cast<DeterministicTuringMachine *>(dtm)->readTape()); }, error);
}

const bool DTM_checkNextState(DTMHandle dtm, const char *key, AutomatonError *error) {
	return wrap_result<const bool>(
	    [&]() { return reinterpret_cast<DeterministicTuringMachine *>(dtm)->checkNextState(key); }, error);
}