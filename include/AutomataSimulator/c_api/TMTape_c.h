#pragma once
#include "../TMDirection.h"
#include "../TMTape.h"
#include "../config.h"
#include "c_util.h"
#include <stddef.h>

AUTOMATASIMULATOR_EXTERN_C_BEGIN

typedef struct TMTape_t *TMTapeHandle;

AUTOMATASIMULATOR_EXPORT TMTapeHandle TMTape_create();

AUTOMATASIMULATOR_EXPORT TMTapeHandle TMTape_createWithBlankSymbol(const char *blankSymbol);

AUTOMATASIMULATOR_EXPORT void TMTape_destroy(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT void TMTape_loadInput(TMTapeHandle tape, const StringArray *input);

AUTOMATASIMULATOR_EXPORT void TMTape_setBlankSymbol(TMTapeHandle tape, const char *blankSymbol);

AUTOMATASIMULATOR_EXPORT const char *TMTape_getBlankSymbol(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT void TMTape_setHeadPosition(TMTapeHandle tape, int position);

AUTOMATASIMULATOR_EXPORT int TMTape_getHeadPosition(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT const char *TMTape_read(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT void TMTape_write(TMTapeHandle tape, const char *symbol);

AUTOMATASIMULATOR_EXPORT void TMTape_moveLeft(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT void TMTape_moveRight(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT void TMTape_move(TMTapeHandle tape, const char *direction);

AUTOMATASIMULATOR_EXPORT bool TMTape_isEmpty(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT void TMTape_reset(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT bool TMTape_isAtLeftEnd(TMTapeHandle tape);

AUTOMATASIMULATOR_EXPORT const char *TMTape_toString(TMTapeHandle tape);

AUTOMATASIMULATOR_EXTERN_C_END
