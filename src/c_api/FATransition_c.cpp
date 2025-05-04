#include "AutomataSimulator/c_api/FATransition_c.h"

struct FATransition_t {
	FATransition instance;
};

FATransitionHandle FATransition_create(char *fromStateKey, char *toStateKey, char *input) {
	return new FATransition_t{FATransition(fromStateKey, toStateKey, input)};
}

void FATransition_destroy(FATransitionHandle transition) {
	delete transition;
}

const char *FATransition_generateTransitionKey(char *fromStateKey, char *toStateKey, char *input) {
	return convertToCString(FATransition::generateTransitionKey(fromStateKey, toStateKey, input));
}

const char *FATransition_getFromStateFromKey(char *key) {
	return convertToCString(FATransition::getFromStateFromKey(key));
}

const char *FATransition_getToStateFromKey(char *key) {
	return convertToCString(FATransition::getToStateFromKey(key));
}

const char *FATransition_getInputFromKey(char *key) {
	return convertToCString(FATransition::getInputFromKey(key));
}

const char *FATransition_getKey(FATransitionHandle transition) {
	return convertToCString(transition->instance.getKey());
}

void FATransition_setFromStateKey(FATransitionHandle transition, char *fromStateKey) {
	transition->instance.setFromStateKey(fromStateKey);
}

const char *FATransition_getFromStateKey(FATransitionHandle transition) {
	return convertToCString(transition->instance.getFromStateKey());
}

void FATransition_setToStateKey(FATransitionHandle transition, char *toStateKey) {
	transition->instance.setToStateKey(toStateKey);
}

const char *FATransition_getToStateKey(FATransitionHandle transition) {
	return convertToCString(transition->instance.getToStateKey());
}

void FATransition_setInput(FATransitionHandle transition, char *input) {
	transition->instance.setInput(input);
}

const char *FATransition_getInput(FATransitionHandle transition) {
	return convertToCString(transition->instance.getInput());
}

const char *toString(FATransitionHandle transition) {
	return convertToCString(transition->instance.toString());
}