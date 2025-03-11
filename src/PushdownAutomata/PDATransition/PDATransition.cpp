#define AUTOMATASIMULATOR_EXPORTS
#include "PDATransition.h"

PDATransition::PDATransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
                             const std::string &stackSymbol, const std::string &pushSymbol)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), input(input), stackSymbol(stackSymbol),
      pushSymbol(pushSymbol) {
	key = fromStateKey + "-" + toStateKey + "-" + input + "-" + stackSymbol + "-" + pushSymbol;
}

PDATransition::PDATransition(const PDATransition &other)
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input), key(other.key),
      stackSymbol(other.stackSymbol), pushSymbol(other.pushSymbol) {}

PDATransition &PDATransition::operator=(const PDATransition &other) {
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

PDATransition::PDATransition(PDATransition &&other) noexcept
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input),
      stackSymbol(other.stackSymbol), pushSymbol(other.pushSymbol), key(other.key) {}

PDATransition &PDATransition::operator=(PDATransition &&other) noexcept {
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

PDATransition::~PDATransition() {}

std::string PDATransition::getKey() const {
	return key;
}

void PDATransition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
}

std::string PDATransition::getFromStateKey() const {
	return fromStateKey;
}

void PDATransition::setToStateKey(const std::string &toState) {
	this->toStateKey = toState;
}

std::string PDATransition::getToStateKey() const {
	return toStateKey;
}

void PDATransition::setInput(const std::string &input) {
	this->input = input;
}

std::string PDATransition::getInput() const {
	return input;
}

void PDATransition::setStackSymbol(const std::string &stackSymbol) {
	this->stackSymbol = stackSymbol;
}

std::string PDATransition::getStackSymbol() const {
	return stackSymbol;
}

void PDATransition::setPushSymbol(const std::string &pushSymbol) {
	this->pushSymbol = pushSymbol;
}

std::string PDATransition::getPushSymbol() const {
	return pushSymbol;
}

std::string PDATransition::toString() const {
	return "Transition: [" + fromStateKey + " -> " + toStateKey + " | Input: " + input +
	       " | Stack Symbol: " + stackSymbol + " | Push Symbol: " + pushSymbol + "]";
}
