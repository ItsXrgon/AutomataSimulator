#include "AutomataSimulator/c_api/TMTransition_c.h"

struct TMTransition_t {
	TMTransition instance;
};

TMTransitionHandle TMTransition_create(char *fromStateKey, char *toStateKey, char *readSymbol, char *writeSymbol,
                                       char *direction) {
	const TMDirection dir = TMDirectionHelper::fromString(direction);
	return new TMTransition_t{TMTransition(fromStateKey, toStateKey, readSymbol, writeSymbol, dir)};
}

void TMTransition_destroy(TMTransitionHandle transition) {
	delete transition;
}

const char *TMTransition_generateTransitionKey(char *fromStateKey, char *toStateKey, char *readSymbol,
                                               char *writeSymbol, char *direction) {
	const TMDirection dir = TMDirectionHelper::fromString(direction);
	return convertToCString(
	    TMTransition::generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, dir));
}

const char *TMTransition_getFromStateFromKey(char *key) {
	return convertToCString(TMTransition::getFromStateFromKey(key));
}

const char *TMTransition_getToStateFromKey(char *key) {
	return convertToCString(TMTransition::getToStateFromKey(key));
}

const char *TMTransition_getReadSymbolFromKey(char *key) {
	return convertToCString(TMTransition::getReadSymbolFromKey(key));
}

const char *TMTransition_getWriteSymbolFromKey(char *key) {
	return convertToCString(TMTransition::getWriteSymbolFromKey(key));
}

const char *TMTransition_getDirectionFromKey(char *key) {
	return convertToCString(TMDirectionHelper::toString(TMTransition::getDirectionFromKey(key)));
}

const char *TMTransition_getKey(TMTransitionHandle transition) {
	return convertToCString(transition->instance.getKey());
}

void TMTransition_setFromStateKey(TMTransitionHandle transition, char *fromStateKey) {
	transition->instance.setFromStateKey(fromStateKey);
}

const char *TMTransition_getFromStateKey(TMTransitionHandle transition) {
	return convertToCString(transition->instance.getFromStateKey());
}

void TMTransition_setToStateKey(TMTransitionHandle transition, char *toStateKey) {
	transition->instance.setToStateKey(toStateKey);
}

const char *TMTransition_getToStateKey(TMTransitionHandle transition) {
	return convertToCString(transition->instance.getToStateKey());
}

void TMTransition_setReadSymbol(TMTransitionHandle transition, char *input) {
	transition->instance.setReadSymbol(input);
}

const char *TMTransition_getReadSymbol(TMTransitionHandle transition) {
	return convertToCString(transition->instance.getReadSymbol());
}

void TMTransition_setWriteSymbol(TMTransitionHandle transition, char *stackSymbol) {
	transition->instance.setWriteSymbol(stackSymbol);
}

const char *TMTransition_getWriteSymbol(TMTransitionHandle transition) {
	return convertToCString(transition->instance.getWriteSymbol());
}

void TMTransition_setDirection(TMTransitionHandle transition, char *direction) {
	const TMDirection dir = TMDirectionHelper::fromString(direction);
	transition->instance.setDirection(dir);
}

const char *TMTransition_getDirection(TMTransitionHandle transition) {
	return convertToCString(TMDirectionHelper::toString(transition->instance.getDirection()));
}

const char *TMTransition_toString(TMTransitionHandle transition) {
	return convertToCString(transition->instance.toString());
}