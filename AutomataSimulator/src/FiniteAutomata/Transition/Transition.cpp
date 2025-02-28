#include "Transition.h"
#include "../State/State.h"

Transition::Transition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), input(input) {
	key = fromStateKey + "-" + toStateKey + "-" + input;
}

Transition::Transition(const Transition &other)
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(other.input), key(other.key) {}

Transition &Transition::operator=(const Transition &other) {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		input = other.input;
		key = other.key;
	}
	return *this;
}

Transition::Transition(Transition &&other) noexcept
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), input(std::move(other.input)),
      key(std::move(other.key)) {}

Transition &Transition::operator=(Transition &&other) noexcept {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		input = std::move(other.input);
		key = std::move(other.key);
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

void Transition::setInput(std::string input) {
	this->input = input;
}

std::string Transition::getInput() const {
	return input;
}