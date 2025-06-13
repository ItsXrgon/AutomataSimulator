#include "AutomataSimulator/c_api/PDAState_c.h"

struct PDAState_t {
	PDAState instance;
};

PDAStateHandle PDAState_create(char *label, bool isAccept) {
	return new PDAState_t{PDAState(label, isAccept)};
}

void PDAState_destroy(PDAStateHandle state) {
	delete state;
}

const char *PDAState_getKey(PDAStateHandle state) {
	return convertToCString(state->instance.getKey());
}

void PDAState_setLabel(PDAStateHandle state, char *label) {
	state->instance.setLabel(label);
}

const char *PDAState_getLabel(PDAStateHandle state) {
	return convertToCString(state->instance.getLabel());
}

const bool PDAState_getIsAccept(PDAStateHandle state) {
	return state->instance.getIsAccept();
}

void PDAState_setIsAccept(PDAStateHandle state, bool isAccept) {
	state->instance.setIsAccept(isAccept);
}

const bool PDAState_transitionExists(PDAStateHandle state, char *key) {
	return state->instance.transitionExists(key);
}

void PDAState_addTransition(PDAStateHandle state, char *toStateKey, char *input, char *stackSymbol, char *pushSymbol) {
	state->instance.addTransition(toStateKey, input, stackSymbol, pushSymbol);
}

const PDATransitionHandle PDAState_getTransition(PDAStateHandle state, char *key) {
	return reinterpret_cast<PDATransitionHandle>(new PDATransition(state->instance.getTransition(key)));
}

const char *PDAState_getTransitionInput(PDAStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionInput(key));
}

void PDAState_setTransitionInput(PDAStateHandle state, char *key, char *input) {
	state->instance.setTransitionInput(key, input);
}

const char *PDAState_getTransitionToState(PDAStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionToState(key));
}

void PDAState_setTransitionToState(PDAStateHandle state, char *key, char *toState) {
	state->instance.setTransitionToState(key, toState);
}

const char *PDAState_getTransitionStackSymbol(PDAStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionStackSymbol(key));
}

void PDAState_setTransitionStackSymbol(PDAStateHandle state, char *key, char *stackSymbol) {
	state->instance.setTransitionStackSymbol(key, stackSymbol);
}

const char *PDAState_getTransitionPushSymbol(PDAStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionPushSymbol(key));
}

void PDAState_setTransitionPushSymbol(PDAStateHandle state, char *key, char *pushSymbol) {
	state->instance.setTransitionPushSymbol(key, pushSymbol);
}

void PDAState_removeTransition(PDAStateHandle state, char *key) {
	state->instance.removeTransition(key);
}

void PDAState_clearTransitionsTo(PDAStateHandle state, char *toStateKey) {
	state->instance.clearTransitionsTo(toStateKey);
}

const PDATransitionArray PDAState_getTransitions(PDAStateHandle state) {
	auto transitions = state->instance.getTransitions();

	PDATransitionArray array;
	array.length = transitions.size();
	array.data = new PDATransitionHandle[array.length];

	for (size_t i = 0; i < array.length; ++i) {
		array.data[i] = reinterpret_cast<PDATransitionHandle>(new PDATransition(transitions[i]));
	}

	return array;
}

void PDAState_clearTransitions(PDAStateHandle state) {
	state->instance.clearTransitions();
}

const char *PDAState_toString(PDAStateHandle state) {
	return convertToCString(state->instance.toString());
}
