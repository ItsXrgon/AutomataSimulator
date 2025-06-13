#pragma once
#include "../PDATransition.h"
#include "../config.h"
#include "c_util.h"

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct PDATransition_t *PDATransitionHandle;

typedef struct {
	PDATransitionHandle *data;
	size_t length;
} PDATransitionArray;

AUTOMATASIMULATOR_EXPORT PDATransitionHandle PDATransition_create(char *fromStateKey, char *toStateKey, char *input,
                                                                  char *stackSymbol, char *pushSymbol);

AUTOMATASIMULATOR_EXPORT void PDATransition_destroy(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_generateTransitionKey(char *fromStateKey, char *toStateKey,
                                                                         char *input, char *stackSymbol,
                                                                         char *pushSymbol);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getFromStateFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getToStateFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getInputFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getStackSymbolFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getPushSymbolFromKey(char *key);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getKey(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void PDATransition_setFromStateKey(PDATransitionHandle transition, char *fromStateKey);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getFromStateKey(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void PDATransition_setToStateKey(PDATransitionHandle transition, char *toStateKey);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getToStateKey(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void PDATransition_setInput(PDATransitionHandle transition, char *input);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getInput(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void PDATransition_setStackSymbol(PDATransitionHandle transition, char *stackSymbol);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getStackSymbol(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT void PDATransition_setPushSymbol(PDATransitionHandle transition, char *pushSymbol);

AUTOMATASIMULATOR_EXPORT const char *PDATransition_getPushSymbol(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXPORT const char *toString(PDATransitionHandle transition);

AUTOMATASIMULATOR_EXTERN_C_END