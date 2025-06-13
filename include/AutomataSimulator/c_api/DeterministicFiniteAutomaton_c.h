#pragma once
#include "../DeterministicFiniteAutomaton.h"
#include "../config.h"
#include "FAState_c.h"
#include "FATransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct DFA_t *DFAHandle;

AUTOMATASIMULATOR_EXPORT DFAHandle DFA_create(AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_destroy(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DFA_getInput(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_setInput(DFAHandle dfa, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_addInput(DFAHandle dfa, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const int DFA_getInputHead(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_setInputHead(DFAHandle dfa, const int head, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DFA_stateExists(DFAHandle dfa, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DFA_inputAlphabetSymbolExists(DFAHandle dfa, const char *symbol,
                                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_addState(DFAHandle dfa, const char *label, const bool isAccept = false,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_updateStateLabel(DFAHandle dfa, const char *key, const char *label,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DFA_getCurrentState(DFAHandle dfa, AutomatonError *error = nullptr);


AUTOMATASIMULATOR_EXPORT void DFA_setCurrentState(DFAHandle dfa, const char *state, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateHandle DFA_getState(DFAHandle dfa, const char *key,
                                                          AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateArray DFA_getStates(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeState(DFAHandle dfa, const char *key, const bool strict = true,
                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeStates(DFAHandle dfa, const char **keys, const size_t length,
                                               const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_clearStates(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_setInputAlphabet(DFAHandle dfa, const char **inputAlphabet, const size_t length,
                                                   const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_addInputAlphabet(DFAHandle dfa, const char **inputAlphabet, const size_t length,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray DFA_getInputAlphabet(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeInputAlphabetSymbol(DFAHandle dfa, const char *symbol, const bool strict = true,
                                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeInputAlphabetSymbols(DFAHandle dfa, const char **symbols, const size_t length,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_clearInputAlphabet(DFAHandle dfa, const bool strict = true,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *DFA_getStartState(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_setStartState(DFAHandle dfa, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FATransitionHandle DFA_getTransition(DFAHandle dfa, const char *key,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_addTransition(DFAHandle dfa, const char *fromStateKey, const char *toStateKey,
                                                const char *input, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_updateTransitionInput(DFAHandle dfa, const char *transitionKey, const char *input,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_updateTransitionFromState(DFAHandle dfa, const char *transitionKey,
                                                            const char *fromStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_updateTransitionToState(DFAHandle dfa, const char *transitionKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeTransition(DFAHandle dfa, const char *transitionKey,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_clearTransitionsBetween(DFAHandle dfa, const char *fromStateKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_clearStateTransitions(DFAHandle dfa, const char *stateKey,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_clearTransitions(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_addAcceptState(DFAHandle dfa, const char *stateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_addAcceptStates(DFAHandle dfa, const char **keys, const size_t length,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeAcceptState(DFAHandle dfa, const char *stateKey,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_removeAcceptStates(DFAHandle dfa, const char **keys, const size_t length,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_clearAcceptStates(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateArray DFA_getAcceptStates(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void DFA_reset(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DFA_isAccepting(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT bool DFA_processInput(DFAHandle dfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DFA_simulate(DFAHandle dfa, const char **input, const size_t length,
                                                 const int simulationDepth = 50, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool DFA_checkNextState(DFAHandle dfa, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXTERN_C_END