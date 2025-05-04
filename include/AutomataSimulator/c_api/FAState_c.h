#pragma once
#include "../FAState.h"
#include "../FATransition.h"
#include "../config.h"
#include "FATransition_c.h"
#include "c_util.h"
#include <vector>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct FAState_t *FAStateHandle;

typedef struct {
	FAStateHandle *data;
	size_t length;
} FAStateArray;

AUTOMATASIMULATOR_EXPORT FAStateHandle FAState_create(const char *label, bool isAccept);

AUTOMATASIMULATOR_EXPORT void FAState_destroy(FAStateHandle state);

AUTOMATASIMULATOR_EXPORT const char *FAState_getKey(FAStateHandle state);

AUTOMATASIMULATOR_EXPORT void FAState_setLabel(FAStateHandle state, const char *label);

AUTOMATASIMULATOR_EXPORT const char *FAState_getLabel(FAStateHandle state);

AUTOMATASIMULATOR_EXPORT const bool FAState_getIsAccept(FAStateHandle state);

AUTOMATASIMULATOR_EXPORT void FAState_setIsAccept(FAStateHandle state, bool isAccept);

AUTOMATASIMULATOR_EXPORT const bool FAState_transitionExists(FAStateHandle state, const char *key);

AUTOMATASIMULATOR_EXPORT void FAState_addTransition(FAStateHandle state, const char *toStateKey, const char *input);

AUTOMATASIMULATOR_EXPORT const FATransitionHandle FAState_getTransition(FAStateHandle state, const char *key);

AUTOMATASIMULATOR_EXPORT const char *FAState_getTransitionInput(FAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void FAState_setTransitionInput(FAStateHandle state, const char *transitionKey,
                                                         const char *input);

AUTOMATASIMULATOR_EXPORT const char *FAState_getTransitionToState(FAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void FAState_setTransitionToState(FAStateHandle state, const char *transitionKey,
                                                           const char *toState);

AUTOMATASIMULATOR_EXPORT void FAState_removeTransition(FAStateHandle state, const char *transitionKey);

AUTOMATASIMULATOR_EXPORT void FAState_clearTransitionsTo(FAStateHandle state, const char *toStateKey);

AUTOMATASIMULATOR_EXPORT const FATransitionArray FAState_getTransitions(FAStateHandle state);

AUTOMATASIMULATOR_EXPORT void FAState_clearTransitions(FAStateHandle state);

AUTOMATASIMULATOR_EXPORT const char *FAState_toString(FAStateHandle state);

AUTOMATASIMULATOR_EXTERN_C_END