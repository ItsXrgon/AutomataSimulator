#define AUTOMATASIMULATOR_EXPORTS
#include "Transition.h"

Transition::Transition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
                       const std::string &stackSymbol, const std::string &pushSymbol)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), input(input), stackSymbol(stackSymbol),
      pushSymbol(pushSymbol) {
	key = fromStateKey + "-" + toStateKey + "-" + input + "-" + stackSymbol + "-" + pushSymbol;
}

Transition::Transition(const Transition &other)
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input), key(other.key) {}

Transition &Transition::operator=(const Transition &other) {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		input = other.input;
		key = other.key;
		stackSymbol = other.stackSymbol;
		pushSymbol = other.pushSymbol;
	}
	return *this;
}

Transition::Transition(Transition &&other) noexcept
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input),
      stackSymbol(other.stackSymbol), pushSymbol(other.pushSymbol), key(other.key) {}

Transition &Transition::operator=(Transition &&other) noexcept {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		input = other.input;
		key = other.key;
		stackSymbol = other.stackSymbol;
		pushSymbol = other.pushSymbol;
	}
	return *this;
}

Transition::~Transition() {}

std::string Transition::getKey() const {
	return key;
}

void Transition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
}

std::string Transition::getFromStateKey() const {
	return fromStateKey;
}

void Transition::setToStateKey(const std::string &toState) {
	this->toStateKey = toState;
}

std::string Transition::getToStateKey() const {
	return toStateKey;
}

void Transition::setInput(const std::string &input) {
	this->input = input;
}

std::string Transition::getInput() const {
	return input;
}

void Transition::setStackSymbol(const std::string &stackSymbol) {
	this->stackSymbol = stackSymbol;
}

std::string Transition::getStackSymbol() const {
	return stackSymbol;
}

void Transition::setPushSymbol(const std::string &pushSymbol) {
	this->pushSymbol = pushSymbol;
}

std::string Transition::getPushSymbol() const {
	return pushSymbol;
}
