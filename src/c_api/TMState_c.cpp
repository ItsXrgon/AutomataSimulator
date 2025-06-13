#include "AutomataSimulator/c_api/TMState_c.h"

struct TMState_t {
	TMState instance;
};

TMStateHandle TMState_create(char *label, bool isAccept) {
	return new TMState_t{TMState(label, isAccept)};
}

void TMState_destroy(TMStateHandle state) {
	delete state;
}

const char *TMState_getKey(TMStateHandle state) {
	return convertToCString(state->instance.getKey());
}

void TMState_setLabel(TMStateHandle state, char *label) {
	state->instance.setLabel(label);
}

const char *TMState_getLabel(TMStateHandle state) {
	return convertToCString(state->instance.getLabel());
}

const bool TMState_getIsAccept(TMStateHandle state) {
	return state->instance.getIsAccept();
}

void TMState_setIsAccept(TMStateHandle state, bool isAccept) {
	state->instance.setIsAccept(isAccept);
}

const bool TMState_transitionExists(TMStateHandle state, char *key) {
	return state->instance.transitionExists(key);
}

void TMState_addTransition(TMStateHandle state, char *toStateKey, char *readSymbol, char *writeSymbol,
                           char *direction) {
	const TMDirection dir = TMDirectionHelper::fromString(direction);
	state->instance.addTransition(toStateKey, readSymbol, writeSymbol, dir);
}

const TMTransitionHandle TMState_getTransition(TMStateHandle state, char *key) {
	return reinterpret_cast<TMTransitionHandle>(new TMTransition(state->instance.getTransition(key)));
}

const char *TMState_getTransitionReadSymbol(TMStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionReadSymbol(key));
}

void TMState_setTransitionReadSymbol(TMStateHandle state, char *key, char *input) {
	state->instance.setTransitionReadSymbol(key, input);
}

const char *TMState_getTransitionToState(TMStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionToState(key));
}

void TMState_setTransitionToState(TMStateHandle state, char *key, char *toState) {
	state->instance.setTransitionToState(key, toState);
}

const char *TMState_getTransitionWriteSymbol(TMStateHandle state, char *key) {
	return convertToCString(state->instance.getTransitionWriteSymbol(key));
}

void TMState_setTransitionWriteSymbol(TMStateHandle state, char *key, char *writeSymbol) {
	state->instance.setTransitionWriteSymbol(key, writeSymbol);
}

const char *TMState_getTransitionDirection(TMStateHandle state, char *key) {
	return convertToCString(TMDirectionHelper::toString(state->instance.getTransitionDirection(key)));
}

void TMState_setTransitionDirection(TMStateHandle state, char *key, char *direction) {
	const TMDirection dir = TMDirectionHelper::fromString(direction);
	state->instance.setTransitionDirection(key, dir);
}

void TMState_removeTransition(TMStateHandle state, char *key) {
	state->instance.removeTransition(key);
}

void TMState_clearTransitionsTo(TMStateHandle state, char *toStateKey) {
	state->instance.clearTransitionsTo(toStateKey);
}

const TMTransitionArray TMState_getTransitions(TMStateHandle state) {
	auto transitions = state->instance.getTransitions();

	TMTransitionArray array;
	array.length = transitions.size();
	array.data = new TMTransitionHandle[array.length];

	for (size_t i = 0; i < array.length; ++i) {
		array.data[i] = reinterpret_cast<TMTransitionHandle>(new TMTransition(transitions[i]));
	}

	return array;
}

void TMState_clearTransitions(TMStateHandle state) {
	state->instance.clearTransitions();
}

const char *TMState_toString(TMStateHandle state) {
	return convertToCString(state->instance.toString());
}
