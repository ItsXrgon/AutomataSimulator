#include "AutomataSimulator/c_api/FAState_c.h"

struct FAState_t {
	FAState instance;
};

FAStateHandle FAState_create(char *label, bool isAccept) {
	return new FAState_t{FAState(label, isAccept)};
}

void FAState_destroy(FAStateHandle state) {
	delete state;
}

const char *FAState_getKey(FAStateHandle state) {
	return convertToCString(state->instance.getKey());
}

void FAState_setLabel(FAStateHandle state, char *label) {
	state->instance.setLabel(label);
}

const char *FAState_getLabel(FAStateHandle state) {
	return convertToCString(state->instance.getLabel());
}

const bool FAState_getIsAccept(FAStateHandle state) {
	return state->instance.getIsAccept();
}

void FAState_setIsAccept(FAStateHandle state, bool isAccept) {
	state->instance.setIsAccept(isAccept);
}

const bool FAState_transitionExists(FAStateHandle state, char *key) {
	return state->instance.transitionExists(key);
}

void FAState_addTransition(FAStateHandle state, char *toStateKey, char *input) {
	state->instance.addTransition(toStateKey, input);
}

const FATransitionHandle FAState_getTransition(FAStateHandle state, char *key) {
	return reinterpret_cast<FATransitionHandle>(new FATransition(state->instance.getTransition(key)));
}

const char *FAState_getTransitionInput(FAStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionInput(key));
}

void FAState_setTransitionInput(FAStateHandle state, char *key, char *input) {
	state->instance.setTransitionInput(key, input);
}

const char *FAState_getTransitionToState(FAStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionToState(key));
}

void FAState_setTransitionToState(FAStateHandle state, char *key, char *toState) {
	state->instance.setTransitionToState(key, toState);
}

void FAState_removeTransition(FAStateHandle state, char *key) {
	state->instance.removeTransition(key);
}

void FAState_clearTransitionsTo(FAStateHandle state, char *toStateKey) {
	state->instance.clearTransitionsTo(toStateKey);
}

const FATransitionArray FAState_getTransitions(FAStateHandle state) {
	auto transitions = state->instance.getTransitions();

	FATransitionArray array;
	array.length = transitions.size();
	array.data = new FATransitionHandle[array.length];

	for (size_t i = 0; i < array.length; ++i) {
		array.data[i] = reinterpret_cast<FATransitionHandle>(new FATransition(transitions[i]));
	}

	return array;
}

void FAState_clearTransitions(FAStateHandle state) {
	state->instance.clearTransitions();
}

const char *FAState_toString(FAStateHandle state) {
	return convertToCString(state->instance.toString());
}
