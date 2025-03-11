#define AUTOMATASIMULATOR_EXPORTS
#include "FATransition.h"

FATransition::FATransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), input(input) {
	key = fromStateKey + "-" + toStateKey + "-" + input;
}

FATransition::FATransition(const FATransition &other)
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input), key(other.key) {}

FATransition &FATransition::operator=(const FATransition &other) {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		input = other.input;
		key = other.key;
	}
	return *this;
}

FATransition::FATransition(FATransition &&other) noexcept
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input), key(other.key) {}

FATransition &FATransition::operator=(FATransition &&other) noexcept {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		input = other.input;
		key = other.key;
	}
	return *this;
}

FATransition::~FATransition() {}

std::string FATransition::getKey() const {
	return key;
}

void FATransition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
}

std::string FATransition::getFromStateKey() const {
	return fromStateKey;
}

void FATransition::setToStateKey(const std::string &toState) {
	this->toStateKey = toState;
}

std::string FATransition::getToStateKey() const {
	return toStateKey;
}

void FATransition::setInput(const std::string &input) {
	this->input = input;
}

std::string FATransition::getInput() const {
	return input;
}

std::string FATransition::toString() const {
	return "Transition: [" + fromStateKey + " -> " + toStateKey + " | Input: " + input + "]";
}
