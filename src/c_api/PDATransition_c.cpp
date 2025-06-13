#include "AutomataSimulator/c_api/PDATransition_c.h"

struct PDATransition_t {
	PDATransition instance;
};

PDATransitionHandle PDATransition_create(char *fromStateKey, char *toStateKey, char *input, char *stackSymbol,
                                         char *pushSymbol) {
	return new PDATransition_t{PDATransition(fromStateKey, toStateKey, input, stackSymbol, pushSymbol)};
}

void PDATransition_destroy(PDATransitionHandle transition) {
	delete transition;
}

const char *PDATransition_generateTransitionKey(char *fromStateKey, char *toStateKey, char *input, char *stackSymbol,
                                                char *pushSymbol) {
	return convertToCString(
	    PDATransition::generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol));
}

const char *PDATransition_getFromStateFromKey(char *key) {
	return convertToCString(PDATransition::getFromStateFromKey(key));
}

const char *PDATransition_getToStateFromKey(char *key) {
	return convertToCString(PDATransition::getToStateFromKey(key));
}

const char *PDATransition_getInputFromKey(char *key) {
	return convertToCString(PDATransition::getInputFromKey(key));
}

const char *PDATransition_getKey(PDATransitionHandle transition) {
	return convertToCString(transition->instance.getKey());
}

void PDATransition_setFromStateKey(PDATransitionHandle transition, char *fromStateKey) {
	transition->instance.setFromStateKey(fromStateKey);
}

const char *PDATransition_getFromStateKey(PDATransitionHandle transition) {
	return convertToCString(transition->instance.getFromStateKey());
}

void PDATransition_setToStateKey(PDATransitionHandle transition, char *toStateKey) {
	transition->instance.setToStateKey(toStateKey);
}

const char *PDATransition_getToStateKey(PDATransitionHandle transition) {
	return convertToCString(transition->instance.getToStateKey());
}

void PDATransition_setInput(PDATransitionHandle transition, char *input) {
	transition->instance.setInput(input);
}

const char *PDATransition_getInput(PDATransitionHandle transition) {
	return convertToCString(transition->instance.getInput());
}

void PDATransition_setStackSymbol(PDATransitionHandle transition, char *stackSymbol) {
	transition->instance.setStackSymbol(stackSymbol);
}

const char *PDATransition_getStackSymbol(PDATransitionHandle transition) {
	return convertToCString(transition->instance.getStackSymbol());
}

void PDATransition_setPushSymbol(PDATransitionHandle transition, char *pushSymbol) {
	transition->instance.setPushSymbol(pushSymbol);
}

const char *PDATransition_getPushSymbol(PDATransitionHandle transition) {
	return convertToCString(transition->instance.getPushSymbol());
}

const char *PDATransition_toString(PDATransitionHandle transition) {
	return convertToCString(transition->instance.toString());
}