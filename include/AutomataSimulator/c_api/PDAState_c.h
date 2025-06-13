#pragma once
#include "../PDAState.h"
#include "../PDATransition.h"
#include "../config.h"
#include "PDATransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct PDAState_t *PDAStateHandle;

typedef struct {
	PDAStateHandle *data;
	size_t length;
} PDAStateArray;

AUTOMATASIMULATOR_EXPORT PDAStateHandle PDAState_create(const char *label, bool isAccept);

AUTOMATASIMULATOR_EXPORT void PDAState_destroy(PDAStateHandle state);

AUTOMATASIMULATOR_EXPORT const char *PDAState_getKey(PDAStateHandle state);

AUTOMATASIMULATOR_EXPORT void PDAState_setLabel(PDAStateHandle state, const char *label);

AUTOMATASIMULATOR_EXPORT const char *PDAState_getLabel(PDAStateHandle state);

AUTOMATASIMULATOR_EXPORT const bool PDAState_getIsAccept(PDAStateHandle state);

AUTOMATASIMULATOR_EXPORT void PDAState_setIsAccept(PDAStateHandle state, bool isAccept);

AUTOMATASIMULATOR_EXPORT const bool PDAState_transitionExists(PDAStateHandle state, const char *key);

AUTOMATASIMULATOR_EXPORT void PDAState_addTransition(PDAStateHandle state, const char *toStateKey, const char *input,
                                                     const char *stackSymbol, const char *pushSymbol);

AUTOMATASIMULATOR_EXPORT const PDATransitionHandle PDAState_getTransition(PDAStateHandle state, const char *key);

AUTOMATASIMULATOR_EXPORT const char *PDAState_getTransitionInput(PDAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void PDAState_setTransitionInput(PDAStateHandle state, const char *transitionKey,
                                                          const char *input);

AUTOMATASIMULATOR_EXPORT const char *PDAState_getTransitionStackSymbol(PDAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void PDAState_setTransitionStackSymbol(PDAStateHandle state, const char *transitionKey,
                                                                const char *stackSymbol);

AUTOMATASIMULATOR_EXPORT const char *PDAState_getTransitionPushSymbol(PDAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void PDAState_setTransitionPushSymbol(PDAStateHandle state, const char *transitionKey,
                                                               const char *pushSymbol);

AUTOMATASIMULATOR_EXPORT const char *PDAState_getTransitionToState(PDAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void PDAState_setTransitionToState(PDAStateHandle state, const char *transitionKey,
                                                            const char *toState);

AUTOMATASIMULATOR_EXPORT void PDAState_removeTransition(PDAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void PDAState_clearTransitionsTo(PDAStateHandle state, const char *toStateKey);

AUTOMATASIMULATOR_EXPORT const PDATransitionArray PDAState_getTransitions(PDAStateHandle state);

AUTOMATASIMULATOR_EXPORT void PDAState_clearTransitions(PDAStateHandle state);

AUTOMATASIMULATOR_EXPORT const char *PDAState_toString(PDAStateHandle state);

AUTOMATASIMULATOR_EXTERN_C_END