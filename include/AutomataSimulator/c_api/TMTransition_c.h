#pragma once
#include "../TMTransition.h"
#include "../config.h"
#include "c_util.h"

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct TMTransition_t *TMTransitionHandle;

typedef struct {
	TMTransitionHandle *data;
	size_t length;
} TMTransitionArray;

AUTOMATASIMULATOR_EXPORT TMTransitionHandle TMTransition_create(char *fromStateKey, char *toStateKey, char *readSymbol,
                                                                char *writeSymbol, char *direction);

AUTOMATASIMULATOR_EXPORT void TMTransition_destroy(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_generateTransitionKey(char *fromStateKey, char *toStateKey,
                                                                         char *readSymbol, char *writeSymbol,
                                                                        char *direction);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getFromStateFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getToStateFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getReadSymbolFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getWriteSymbolFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getDirectionFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getKey(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void TMTransition_setFromStateKey(TMTransitionHandle transition, char *fromStateKey);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getFromStateKey(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void TMTransition_setToStateKey(TMTransitionHandle transition, char *toStateKey);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getToStateKey(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void TMTransition_setReadSymbol(TMTransitionHandle transition, char *readSymbol);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getReadSymbol(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void TMTransition_setWriteSymbol(TMTransitionHandle transition, char *writeSymbol);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getWriteSymbol(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void TMTransition_setDirection(TMTransitionHandle transition, char *direction);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_getDirection(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXPORT const char *TMTransition_toString(TMTransitionHandle transition);

AUTOMATASIMULATOR_EXTERN_C_END