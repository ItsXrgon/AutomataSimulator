#pragma once
#include "../FATransition.h"
#include "../config.h"
#include "c_util.h"

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct FATransition_t *FATransitionHandle;

typedef struct {
	FATransitionHandle *data;
	size_t length;
} FATransitionArray;

AUTOMATASIMULATOR_EXPORT FATransitionHandle FATransition_create(char *fromStateKey, char *toStateKey, char *input);

AUTOMATASIMULATOR_EXPORT void FATransition_destroy(FATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT const char *FATransition_generateTransitionKey(char *fromStateKey, char *toStateKey,
                                                                        char *input);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getFromStateFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getToStateFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getInputFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getKey(FATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void FATransition_setFromStateKey(FATransitionHandle transition, char *fromStateKey);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getFromStateKey(FATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void FATransition_setToStateKey(FATransitionHandle transition, char *toStateKey);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getToStateKey(FATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void FATransition_setInput(FATransitionHandle transition, char *input);

AUTOMATASIMULATOR_EXPORT const char *FATransition_getInput(FATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT const char *toString(FATransitionHandle transition);

AUTOMATASIMULATOR_EXTERN_C_END