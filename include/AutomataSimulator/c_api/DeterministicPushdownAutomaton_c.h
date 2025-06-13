#pragma once
#include "../DeterministicPushdownAutomaton.h"
#include "../config.h"
#include "PDAState_c.h"
#include "PDATransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct DPDA_t *DPDAHandle;

AUTOMATASIMULATOR_EXPORT DPDAHandle DPDA_create(AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_destroy(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DPDA_getInput(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_setInput(DPDAHandle dpda, const char **input, const size_t length,
                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addInput(DPDAHandle dpda, const char **input, const size_t length,
                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const int DPDA_getInputHead(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_setInputHead(DPDAHandle dpda, const int head, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DPDA_stateExists(DPDAHandle dpda, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DPDA_inputAlphabetSymbolExists(DPDAHandle dpda, const char *symbol,
                                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addState(DPDAHandle dpda, const char *label, const bool isAccept = false,
                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_updateStateLabel(DPDAHandle dpda, const char *key, const char *label,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DPDA_getCurrentState(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_setCurrentState(DPDAHandle dpda, const char *state, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateHandle DPDA_getState(DPDAHandle dpda, const char *key,
                                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateArray DPDA_getStates(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeState(DPDAHandle dpda, const char *key, const bool strict = true,
                                               AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeStates(DPDAHandle dpda, const char **keys, const size_t length,
                                                const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearStates(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_setInputAlphabet(DPDAHandle dpda, const char **inputAlphabet, const size_t length,
                                                    const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addInputAlphabet(DPDAHandle dpda, const char **inputAlphabet, const size_t length,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DPDA_getInputAlphabet(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeInputAlphabetSymbol(DPDAHandle dpda, const char *symbol,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeInputAlphabetSymbols(DPDAHandle dpda, const char **symbols,
                                                              const size_t length, const bool strict = true,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearInputAlphabet(DPDAHandle dpda, const bool strict = true,
                                                      AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_setStackAlphabet(DPDAHandle dpda, const char **stackAlphabet, const size_t length,
                                                    const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addStackAlphabet(DPDAHandle dpda, const char **stackAlphabet, const size_t length,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DPDA_getStackAlphabet(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeStackAlphabetSymbol(DPDAHandle dpda, const char *symbol,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeStackAlphabetSymbols(DPDAHandle dpda, const char **symbols,
                                                              const size_t length, const bool strict = true,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearStackAlphabet(DPDAHandle dpda, const bool strict = true,
                                                      AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DPDA_getStartState(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_setStartState(DPDAHandle dpda, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT PDATransitionHandle DPDA_getTransition(DPDAHandle dpda, const char *key,
                                                                AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addTransition(DPDAHandle dpda, const char *fromStateKey, const char *toStateKey,
                                                 const char *input, const char *stackSymbol, const char *pushSymbol,
                                                 AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_updateTransitionInput(DPDAHandle dpda, const char *transitionKey, const char *input,
                                                         AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_updateTransitionFromState(DPDAHandle dpda, const char *transitionKey,
                                                             const char *fromStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_updateTransitionToState(DPDAHandle dpda, const char *transitionKey,
                                                           const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_updateTransitionStackSymbol(DPDAHandle dpda, const char *transitionKey,
                                                               const char *stackSymbol,
                                                               AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_updateTransitionPushSymbol(DPDAHandle dpda, const char *transitionKey,
                                                              const char *pushSymbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeTransition(DPDAHandle dpda, const char *transitionKey,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearTransitionsBetween(DPDAHandle dpda, const char *fromStateKey,
                                                           const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearStateTransitions(DPDAHandle dpda, const char *stateKey,
                                                         AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearTransitions(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addAcceptState(DPDAHandle dpda, const char *stateKey,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_addAcceptStates(DPDAHandle dpda, const char **keys, const size_t length,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeAcceptState(DPDAHandle dpda, const char *stateKey,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_removeAcceptStates(DPDAHandle dpda, const char **keys, const size_t length,
                                                      AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_clearAcceptStates(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateArray DPDA_getAcceptStates(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_reset(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DPDA_isAccepting(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT bool DPDA_processInput(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DPDA_simulate(DPDAHandle dpda, const char **input, const size_t length,
                                                  const int simulationDepth = 50, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT StringArray DPDA_getStack(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_pushStack(DPDAHandle dpda, const char *symbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DPDA_popStack(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DPDA_peekStack(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DPDA_resetStack(DPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DPDA_checkNextState(DPDAHandle dpda, const char *key,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXTERN_C_END