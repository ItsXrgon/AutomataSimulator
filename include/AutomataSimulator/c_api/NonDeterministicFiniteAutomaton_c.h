#pragma once
#include "../NonDeterministicFiniteAutomaton.h"
#include "../config.h"
#include "FAState_c.h"
#include "FATransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct NFA_t *NFAHandle;

AUTOMATASIMULATOR_EXPORT NFAHandle NFA_create(AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_destroy(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NFA_getInput(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_setInput(NFAHandle nfa, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_addInput(NFAHandle nfa, const char **input, const size_t length,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const int NFA_getInputHead(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_setInputHead(NFAHandle nfa, const int head, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NFA_stateExists(NFAHandle nfa, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NFA_inputAlphabetSymbolExists(NFAHandle nfa, const char *symbol,
                                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_addState(NFAHandle nfa, const char *label, const bool isAccept = false,
                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_updateStateLabel(NFAHandle nfa, const char *key, const char *label,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NFA_getCurrentState(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_setCurrentState(NFAHandle nfa, const char *state, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateHandle NFA_getState(NFAHandle nfa, const char *key,
                                                          AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateArray NFA_getStates(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeState(NFAHandle nfa, const char *key, const bool strict = true,
                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeStates(NFAHandle nfa, const char **keys, const size_t length,
                                               const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_clearStates(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_setInputAlphabet(NFAHandle nfa, const char **inputAlphabet, const size_t length,
                                                   const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_addInputAlphabet(NFAHandle nfa, const char **inputAlphabet, const size_t length,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NFA_getInputAlphabet(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeInputAlphabetSymbol(NFAHandle nfa, const char *symbol, const bool strict = true,
                                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeInputAlphabetSymbols(NFAHandle nfa, const char **symbols, const size_t length,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_clearInputAlphabet(NFAHandle nfa, const bool strict = true,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NFA_getStartState(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_setStartState(NFAHandle nfa, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT FATransitionHandle NFA_getTransition(NFAHandle dfa, const char *key,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_addTransition(NFAHandle nfa, const char *fromStateKey, const char *toStateKey,
                                                const char *input, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_updateTransitionInput(NFAHandle nfa, const char *transitionKey, const char *input,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_updateTransitionFromState(NFAHandle nfa, const char *transitionKey,
                                                            const char *fromStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_updateTransitionToState(NFAHandle nfa, const char *transitionKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeTransition(NFAHandle nfa, const char *transitionKey,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_clearTransitionsBetween(NFAHandle nfa, const char *fromStateKey,
                                                          const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_clearStateTransitions(NFAHandle nfa, const char *stateKey,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_clearTransitions(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_addAcceptState(NFAHandle nfa, const char *stateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_addAcceptStates(NFAHandle nfa, const char **keys, const size_t length,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeAcceptState(NFAHandle nfa, const char *stateKey,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_removeAcceptStates(NFAHandle nfa, const char **keys, const size_t length,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_clearAcceptStates(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateArray NFA_getAcceptStates(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NFA_reset(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NFA_isAccepting(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT bool NFA_processInput(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NFA_simulate(NFAHandle nfa, const char **input, const size_t length,
                                                 const int simulationDepth = 50, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const FAStateArray getPossibleCurrentStates(NFAHandle nfa, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXTERN_C_END