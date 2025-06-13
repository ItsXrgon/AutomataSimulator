#pragma once
#include "../TMState.h"
#include "../TMTransition.h"
#include "../config.h"
#include "TMTransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct TMState_t *TMStateHandle;

typedef struct {
	TMStateHandle *data;
	size_t length;
} TMStateArray;

AUTOMATASIMULATOR_EXPORT TMStateHandle TMState_create(const char *label, bool isAccept);

AUTOMATASIMULATOR_EXPORT void TMState_destroy(TMStateHandle state);

AUTOMATASIMULATOR_EXPORT const char *TMState_getKey(TMStateHandle state);

AUTOMATASIMULATOR_EXPORT void TMState_setLabel(TMStateHandle state, const char *label);

AUTOMATASIMULATOR_EXPORT const char *TMState_getLabel(TMStateHandle state);

AUTOMATASIMULATOR_EXPORT const bool TMState_getIsAccept(TMStateHandle state);

AUTOMATASIMULATOR_EXPORT void TMState_setIsAccept(TMStateHandle state, bool isAccept);

AUTOMATASIMULATOR_EXPORT const bool TMState_transitionExists(TMStateHandle state, const char *key);

AUTOMATASIMULATOR_EXPORT void TMState_addTransition(TMStateHandle state, const char *toStateKey, const char *readSymbol,
                                                     const char *writeSymbol, const char *direction);

AUTOMATASIMULATOR_EXPORT const TMTransitionHandle TMState_getTransition(TMStateHandle state, const char *key);

AUTOMATASIMULATOR_EXPORT const char *TMState_getTransitionReadSymbol(TMStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void TMState_setTransitionReadSymbol(TMStateHandle state, const char *transitionKey,
                                                          const char *readSymbol);

AUTOMATASIMULATOR_EXPORT const char *TMState_getTransitionWriteSymbol(TMStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void TMState_setTransitionWriteSymbol(TMStateHandle state, const char *transitionKey,
                                                                const char *writeSymbol);

AUTOMATASIMULATOR_EXPORT const char *TMState_getTransitionDirection(TMStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void TMState_setTransitionDirection(TMStateHandle state, const char *transitionKey,
                                                               const char *direction);

AUTOMATASIMULATOR_EXPORT const char *TMState_getTransitionToState(TMStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void TMState_setTransitionToState(TMStateHandle state, const char *transitionKey,
                                                            const char *toState);

AUTOMATASIMULATOR_EXPORT void TMState_removeTransition(TMStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void TMState_clearTransitionsTo(TMStateHandle state, const char *toStateKey);

AUTOMATASIMULATOR_EXPORT const TMTransitionArray TMState_getTransitions(TMStateHandle state);

AUTOMATASIMULATOR_EXPORT void TMState_clearTransitions(TMStateHandle state);

AUTOMATASIMULATOR_EXPORT const char *TMState_toString(TMStateHandle state);

AUTOMATASIMULATOR_EXTERN_C_END