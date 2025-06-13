#pragma once
#include "../DeterministicTuringMachine.h"
#include "../config.h"
#include "TMState_c.h"
#include "TMTransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct DTM_t *DTMHandle;

AUTOMATASIMULATOR_EXPORT DTMHandle DTM_create(AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_destroy(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DTM_getInput(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setInput(DTMHandle dtm, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addInput(DTMHandle dtm, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DTM_stateExists(DTMHandle dtm, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DTM_inputAlphabetSymbolExists(DTMHandle dtm, const char *symbol,
                                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addState(DTMHandle dtm, const char *label, const bool isAccept = false,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_updateStateLabel(DTMHandle dtm, const char *key, const char *label,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DTM_getCurrentState(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setCurrentState(DTMHandle dtm, const char *state, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateHandle DTM_getState(DTMHandle dtm, const char *key,
                                                          AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateArray DTM_getStates(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeState(DTMHandle dtm, const char *key, const bool strict = true,
                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeStates(DTMHandle dtm, const char **keys, const size_t length,
                                               const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearStates(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setInputAlphabet(DTMHandle dtm, const char **inputAlphabet, const size_t length,
                                                   const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addInputAlphabet(DTMHandle dtm, const char **inputAlphabet, const size_t length,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DTM_getInputAlphabet(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeInputAlphabetSymbol(DTMHandle dtm, const char *symbol, const bool strict = true,
                                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeInputAlphabetSymbols(DTMHandle dtm, const char **symbols, const size_t length,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearInputAlphabet(DTMHandle dtm, const bool strict = true,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setTapeAlphabet(DTMHandle dtm, const char **stackAlphabet, const size_t length,
                                                  const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addTapeAlphabet(DTMHandle dtm, const char **stackAlphabet, const size_t length,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DTM_getTapeAlphabet(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeTapeAlphabetSymbol(DTMHandle dtm, const char *symbol, const bool strict = true,
                                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeTapeAlphabetSymbols(DTMHandle dtm, const char **symbols, const size_t length,
                                                            const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearTapeAlphabet(DTMHandle dtm, const bool strict = true,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DTM_getStartState(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setStartState(DTMHandle dtm, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT TMTransitionHandle DTM_getTransition(DTMHandle dtm, const char *key,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addTransition(DTMHandle dtm, const char *fromStateKey, const char *toStateKey,
                                                const char *readSymbol, const char *writeSymbol, const char *direction,
                                                AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_updateTransitionReadSymbol(DTMHandle dtm, const char *transitionKey,
                                                             const char *readSymbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_updateTransitionFromState(DTMHandle dtm, const char *transitionKey,
                                                            const char *fromStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_updateTransitionToState(DTMHandle dtm, const char *transitionKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_updateTransitionWriteSymbol(DTMHandle dtm, const char *transitionKey,
                                                              const char *stackSymbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_updateTransitionDirection(DTMHandle dtm, const char *transitionKey,
                                                            const char *direction, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeTransition(DTMHandle dtm, const char *transitionKey,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearTransitionsBetween(DTMHandle dtm, const char *fromStateKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearStateTransitions(DTMHandle dtm, const char *stateKey,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearTransitions(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addAcceptState(DTMHandle dtm, const char *stateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_addAcceptStates(DTMHandle dtm, const char **keys, const size_t length,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeAcceptState(DTMHandle dtm, const char *stateKey,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_removeAcceptStates(DTMHandle dtm, const char **keys, const size_t length,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_clearAcceptStates(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateArray DTM_getAcceptStates(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_reset(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DTM_isAccepting(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT bool DTM_processInput(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DTM_simulate(DTMHandle dtm, const char **input, const size_t length,
                                                 const int simulationDepth = 50, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setTape(DTMHandle dtm, const char **&tape, const size_t &length,
                                          AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DTM_getTape(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_setTapeHead(DTMHandle dtm, int headIndex, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const int DTM_getTapeHead(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_resetTape(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_moveTapeHead(DTMHandle dtm, char *direction, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DTM_writeTape(DTMHandle dtm, char *symbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DTM_readTape(DTMHandle dtm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DTM_checkNextState(DTMHandle dtm, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXTERN_C_END