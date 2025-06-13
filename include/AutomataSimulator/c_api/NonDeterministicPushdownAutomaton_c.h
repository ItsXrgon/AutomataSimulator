#pragma once
#include "../NonDeterministicPushdownAutomaton.h"
#include "../config.h"
#include "PDAState_c.h"
#include "PDATransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct NPDA_t *NPDAHandle;

AUTOMATASIMULATOR_EXPORT NPDAHandle NPDA_create(AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_destroy(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NPDA_getInput(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_setInput(NPDAHandle npda, const char **input, const size_t length,
                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addInput(NPDAHandle npda, const char **input, const size_t length,
                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const int NPDA_getInputHead(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_setInputHead(NPDAHandle npda, const int head, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NPDA_stateExists(NPDAHandle npda, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NPDA_inputAlphabetSymbolExists(NPDAHandle npda, const char *symbol,
                                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addState(NPDAHandle npda, const char *label, const bool isAccept = false,
                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_updateStateLabel(NPDAHandle npda, const char *key, const char *label,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NPDA_getCurrentState(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_setCurrentState(NPDAHandle npda, const char *state, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateHandle NPDA_getState(NPDAHandle npda, const char *key,
                                                            AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateArray NPDA_getStates(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeState(NPDAHandle npda, const char *key, const bool strict = true,
                                               AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeStates(NPDAHandle npda, const char **keys, const size_t length,
                                                const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearStates(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_setInputAlphabet(NPDAHandle npda, const char **inputAlphabet, const size_t length,
                                                    const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addInputAlphabet(NPDAHandle npda, const char **inputAlphabet, const size_t length,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NPDA_getInputAlphabet(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeInputAlphabetSymbol(NPDAHandle npda, const char *symbol,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeInputAlphabetSymbols(NPDAHandle npda, const char **symbols,
                                                              const size_t length, const bool strict = true,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearInputAlphabet(NPDAHandle npda, const bool strict = true,
                                                      AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_setStackAlphabet(NPDAHandle npda, const char **stackAlphabet, const size_t length,
                                                    const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addStackAlphabet(NPDAHandle npda, const char **stackAlphabet, const size_t length,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const StringArray NPDA_getStackAlphabet(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeStackAlphabetSymbol(NPDAHandle npda, const char *symbol,
                                                             const bool strict = true, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeStackAlphabetSymbols(NPDAHandle npda, const char **symbols,
                                                              const size_t length, const bool strict = true,
                                                              AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearStackAlphabet(NPDAHandle npda, const bool strict = true,
                                                      AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NPDA_getStartState(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_setStartState(NPDAHandle npda, const char *key, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT PDATransitionHandle NPDA_getTransition(NPDAHandle npda, const char *key,
                                                                AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addTransition(NPDAHandle npda, const char *fromStateKey, const char *toStateKey,
                                                 const char *input, const char *stackSymbol, const char *pushSymbol,
                                                 AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_updateTransitionInput(NPDAHandle npda, const char *transitionKey, const char *input,
                                                         AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_updateTransitionFromState(NPDAHandle npda, const char *transitionKey,
                                                             const char *fromStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_updateTransitionToState(NPDAHandle npda, const char *transitionKey,
                                                           const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_updateTransitionStackSymbol(NPDAHandle npda, const char *transitionKey,
                                                               const char *stackSymbol,
                                                               AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_updateTransitionPushSymbol(NPDAHandle npda, const char *transitionKey,
                                                              const char *pushSymbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeTransition(NPDAHandle npda, const char *transitionKey,
                                                    AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearTransitionsBetween(NPDAHandle npda, const char *fromStateKey,
                                                           const char *toStateKey, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearStateTransitions(NPDAHandle npda, const char *stateKey,
                                                         AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearTransitions(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addAcceptState(NPDAHandle npda, const char *stateKey,
                                                  AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_addAcceptStates(NPDAHandle npda, const char **keys, const size_t length,
                                                   AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeAcceptState(NPDAHandle npda, const char *stateKey,
                                                     AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_removeAcceptStates(NPDAHandle npda, const char **keys, const size_t length,
                                                      AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_clearAcceptStates(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateArray NPDA_getAcceptStates(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_reset(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NPDA_isAccepting(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT bool NPDA_processInput(NPDAHandle npda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NPDA_simulate(NPDAHandle npda, const char **input, const size_t length,
                                                  const int simulationDepth = 50, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const PDAStateArray NPDA_getPossibleCurrentStates(NPDAHandle npda,
                                                                           AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT StringArray NPDA_getStack(NPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_pushStack(NPDAHandle dpda, const char *symbol, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NPDA_popStack(NPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const char *NPDA_peekStack(NPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT void NPDA_resetStack(NPDAHandle dpda, AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXPORT const bool NPDA_checkNextState(NPDAHandle dpda, const char *key,
                                                        AutomatonError *error = nullptr);

AUTOMATASIMULATOR_EXTERN_C_END