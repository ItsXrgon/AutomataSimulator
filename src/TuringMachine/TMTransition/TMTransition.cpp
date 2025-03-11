#define AUTOMATASIMULATOR_EXPORTS
#include "TMTransition.h"

// Constructor
TMTransition::TMTransition(const std::string &fromStateKey, const std::string &toStateKey,
                           const std::string &readSymbol, const std::string &writeSymbol, Direction direction)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), readSymbol(readSymbol), writeSymbol(writeSymbol),
      direction(direction) {
	key = fromStateKey + "-" + toStateKey + "-" + readSymbol + "-" + writeSymbol + "-" +
	      DirectionHelper::toString(direction);
}

// Copy Constructor
TMTransition::TMTransition(const TMTransition &other)
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), readSymbol(other.readSymbol),
      writeSymbol(other.writeSymbol), direction(other.direction), key(other.key) {}

// Copy Assignment Operator
TMTransition &TMTransition::operator=(const TMTransition &other) {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		readSymbol = other.readSymbol;
		writeSymbol = other.writeSymbol;
		direction = other.direction;
		key = other.key;
	}
	return *this;
}

// Move Constructor
TMTransition::TMTransition(TMTransition &&other) noexcept
    : fromStateKey(std::move(other.fromStateKey)), toStateKey(std::move(other.toStateKey)),
      readSymbol(std::move(other.readSymbol)), writeSymbol(std::move(other.writeSymbol)), direction(other.direction),
      key(std::move(other.key)) {}

// Move Assignment Operator
TMTransition &TMTransition::operator=(TMTransition &&other) noexcept {
	if (this != &other) {
		fromStateKey = std::move(other.fromStateKey);
		toStateKey = std::move(other.toStateKey);
		readSymbol = std::move(other.readSymbol);
		writeSymbol = std::move(other.writeSymbol);
		direction = other.direction;
		key = std::move(other.key);
	}
	return *this;
}

// Destructor
TMTransition::~TMTransition() {}

// Getters
std::string TMTransition::getKey() const {
	return key;
}

std::string TMTransition::getFromStateKey() const {
	return fromStateKey;
}

std::string TMTransition::getToStateKey() const {
	return toStateKey;
}

void TMTransition::setInput(const std::string &input) {
	this->input = input;
}

std::string TMTransition::getInput() const {
	return input;
}

std::string TMTransition::getReadSymbol() const {
	return readSymbol;
}

std::string TMTransition::getWriteSymbol() const {
	return writeSymbol;
}

Direction TMTransition::getDirection() const {
	return direction;
}

// Setters
void TMTransition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
}

void TMTransition::setToStateKey(const std::string &toStateKey) {
	this->toStateKey = toStateKey;
}

void TMTransition::setReadSymbol(const std::string &readSymbol) {
	this->readSymbol = readSymbol;
}

void TMTransition::setWriteSymbol(const std::string &writeSymbol) {
	this->writeSymbol = writeSymbol;
}

void TMTransition::setDirection(Direction direction) {
	this->direction = direction;
}

// ToString Representation
std::string TMTransition::toString() const {
	return "TM Transition: [" + fromStateKey + " -> " + toStateKey + " | Read: " + readSymbol +
	       " | Write: " + writeSymbol + " | Move: " + DirectionHelper::toString(direction) + "]";
}
