#pragma once
#include "../NonDeterministicTuringMachine.h"
#include "../config.h"
#include "TMState_c.h"
#include "TMTransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct NTM_t *NTMHandle;

AUTOMATASIMULATOR_EXPORT NTMHandle NTM_create(AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_destroy(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NTM_getInput(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setInput(NTMHandle ntm, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addInput(NTMHandle ntm, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NTM_stateExists(NTMHandle ntm, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NTM_inputAlphabetSymbolExists(NTMHandle ntm, const char *symbol,
                                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addState(NTMHandle ntm, const char *label, const bool isAccept = false,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_updateStateLabel(NTMHandle ntm, const char *key, const char *label,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NTM_getCurrentState(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setCurrentState(NTMHandle ntm, const char *state, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateHandle NTM_getState(NTMHandle ntm, const char *key,
                                                          AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateArray NTM_getStates(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeState(NTMHandle ntm, const char *key, const bool strict = true,
                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeStates(NTMHandle ntm, const char **keys, const size_t length,
                                               const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearStates(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setInputAlphabet(NTMHandle ntm, const char **inputAlphabet, const size_t length,
                                                   const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addInputAlphabet(NTMHandle ntm, const char **inputAlphabet, const size_t length,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NTM_getInputAlphabet(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeInputAlphabetSymbol(NTMHandle ntm, const char *symbol, const bool strict = true,
                                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeInputAlphabetSymbols(NTMHandle ntm, const char **symbols, const size_t length,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearInputAlphabet(NTMHandle ntm, const bool strict = true,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setTapeAlphabet(NTMHandle ntm, const char **stackAlphabet, const size_t length,
                                                  const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addTapeAlphabet(NTMHandle ntm, const char **stackAlphabet, const size_t length,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NTM_getTapeAlphabet(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeTapeAlphabetSymbol(NTMHandle ntm, const char *symbol, const bool strict = true,
                                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeTapeAlphabetSymbols(NTMHandle ntm, const char **symbols, const size_t length,
                                                            const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearTapeAlphabet(NTMHandle ntm, const bool strict = true,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NTM_getStartState(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setStartState(NTMHandle ntm, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT TMTransitionHandle NTM_getTransition(NTMHandle ntm, const char *key,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addTransition(NTMHandle ntm, const char *fromStateKey, const char *toStateKey,
                                                const char *readSymbol, const char *writeSymbol, const char *direction,
                                                AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_updateTransitionReadSymbol(NTMHandle ntm, const char *transitionKey,
                                                             const char *readSymbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_updateTransitionFromState(NTMHandle ntm, const char *transitionKey,
                                                            const char *fromStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_updateTransitionToState(NTMHandle ntm, const char *transitionKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_updateTransitionWriteSymbol(NTMHandle ntm, const char *transitionKey,
                                                              const char *stackSymbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_updateTransitionDirection(NTMHandle ntm, const char *transitionKey,
                                                            const char *direction, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeTransition(NTMHandle ntm, const char *transitionKey,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearTransitionsBetween(NTMHandle ntm, const char *fromStateKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearStateTransitions(NTMHandle ntm, const char *stateKey,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearTransitions(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addAcceptState(NTMHandle ntm, const char *stateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_addAcceptStates(NTMHandle ntm, const char **keys, const size_t length,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeAcceptState(NTMHandle ntm, const char *stateKey,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_removeAcceptStates(NTMHandle ntm, const char **keys, const size_t length,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_clearAcceptStates(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateArray NTM_getAcceptStates(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_reset(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NTM_isAccepting(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT bool NTM_processInput(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NTM_simulate(NTMHandle ntm, const char **input, const size_t length,
                                                 const int simulationDepth = 50, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const TMStateArray NTM_getPossibleCurrentStates(NTMHandle ntm,
                                                                         AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setTape(NTMHandle ntm, const char **&tape, const size_t &length,
                                          AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NTM_getTape(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_setTapeHead(NTMHandle ntm, int headIndex, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT int const NTM_getTapeHead(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_resetTape(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_moveTapeHead(NTMHandle ntm, char *direction, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NTM_writeTape(NTMHandle ntm, char *symbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NTM_readTape(NTMHandle ntm, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NTM_checkNextState(NTMHandle ntm, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXTERN_C_END