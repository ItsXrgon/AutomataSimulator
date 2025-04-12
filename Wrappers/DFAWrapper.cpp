#pragma once
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

AUTOMATASIMULATOR_API void CreateDFA();
AUTOMATASIMULATOR_API void DestroyDFA();

AUTOMATASIMULATOR_API void SetDFAInput(const char **input, int length);
AUTOMATASIMULATOR_API void AddDFAInput(const char **input, int length);

AUTOMATASIMULATOR_API void SetDFAAlphabet(const char **alphabet, int length, bool strict);
AUTOMATASIMULATOR_API void AddDFAAlphabet(const char **alphabet, int length);

AUTOMATASIMULATOR_API void AddDFATransition(const char *from, const char *to, const char *input);
AUTOMATASIMULATOR_API void UpdateDFATransitionInput(const char *transitionKey, const char *input);
AUTOMATASIMULATOR_API void UpdateDFATransitionFromState(const char *transitionKey, const char *fromStateKey);

AUTOMATASIMULATOR_API bool ProcessDFAInput();
AUTOMATASIMULATOR_API bool SimulateDFA(const char **input, int length, int depth);

#ifdef __cplusplus
}
#endif
