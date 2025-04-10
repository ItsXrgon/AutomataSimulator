#define AUTOMATASIMULATOR_EXPORTS
#include "TMTransition.h"

TMTransition::TMTransition(const std::string &fromStateKey, const std::string &toStateKey,
                           const std::string &readSymbol, const std::string &writeSymbol, TMDirection direction)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), readSymbol(readSymbol), writeSymbol(writeSymbol),
      direction(direction) {
	key = generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

TMTransition::TMTransition(const TMTransition &other)
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), key(other.key), readSymbol(other.readSymbol),
      writeSymbol(other.writeSymbol), direction(other.direction) {}

TMTransition &TMTransition::operator=(const TMTransition &other) {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		key = other.key;
		readSymbol = other.readSymbol;
		writeSymbol = other.writeSymbol;
		direction = other.direction;
	}
	return *this;
}

TMTransition::TMTransition(TMTransition &&other) noexcept
    : fromStateKey(other.fromStateKey), toStateKey(other.toStateKey), readSymbol(other.readSymbol),
      writeSymbol(other.writeSymbol), direction(other.direction), key(other.key) {}

TMTransition &TMTransition::operator=(TMTransition &&other) noexcept {
	if (this != &other) {
		fromStateKey = other.fromStateKey;
		toStateKey = other.toStateKey;
		key = other.key;
		readSymbol = other.readSymbol;
		writeSymbol = other.writeSymbol;
		direction = other.direction;
	}
	return *this;
}

bool TMTransition::operator==(const TMTransition &other) const {
	return fromStateKey == other.fromStateKey && toStateKey == other.toStateKey && readSymbol == other.readSymbol &&
	       writeSymbol == other.writeSymbol && direction == other.direction;
}

TMTransition::~TMTransition() {}

std::string TMTransition::generateTransitionKey(const std::string &fromStateKey, const std::string &toStateKey,
                                                const std::string &readSymbol, const std::string &writeSymbol,
                                                TMDirection direction) {
	return fromStateKey + "-" + toStateKey + "-" + readSymbol + "-" + writeSymbol + "-" +
	       TMDirectionHelper::toString(direction);
}

void TMTransition::validateTransitionKeyFormat(const std::string &key) {
	int delimiterCount = std::count(key.begin(), key.end(), '-');
	if (delimiterCount != 4) {
		throw TransitionNotFoundException("Invalid TM transition key format: " + key);
	}
}

std::string TMTransition::getFromStateFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t first = key.find('-');
	return key.substr(0, first);
}

std::string TMTransition::getToStateFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t first = key.find('-');
	size_t second = key.find('-', first + 1);
	return key.substr(first + 1, second - (first + 1));
}

std::string TMTransition::getReadSymbolFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t second = key.find('-', key.find('-') + 1);
	size_t third = key.find('-', second + 1);
	return key.substr(second + 1, third - (second + 1));
}

std::string TMTransition::getWriteSymbolFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t third = key.find('-', key.find('-', key.find('-') + 1) + 1);
	size_t fourth = key.find('-', third + 1);
	return key.substr(third + 1, fourth - (third + 1));
}

TMDirection TMTransition::getDirectionFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t fourth = key.find_last_of('-');
	std::string directionStr = key.substr(fourth + 1);
	return TMDirectionHelper::fromString(directionStr);
}

std::string TMTransition::getKey() const {
	return key;
}

std::string TMTransition::getFromStateKey() const {
	return fromStateKey;
}

void TMTransition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
	key = generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

std::string TMTransition::getToStateKey() const {
	return toStateKey;
}

void TMTransition::setToStateKey(const std::string &toStateKey) {
	this->toStateKey = toStateKey;
	key = generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

std::string TMTransition::getReadSymbol() const {
	return readSymbol;
}

void TMTransition::setReadSymbol(const std::string &readSymbol) {
	this->readSymbol = readSymbol;
	key = generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

std::string TMTransition::getWriteSymbol() const {
	return writeSymbol;
}

void TMTransition::setWriteSymbol(const std::string &writeSymbol) {
	this->writeSymbol = writeSymbol;
	key = generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

TMDirection TMTransition::getDirection() const {
	return direction;
}

void TMTransition::setDirection(TMDirection direction) {
	this->direction = direction;
	key = generateTransitionKey(fromStateKey, toStateKey, readSymbol, writeSymbol, direction);
}

std::string TMTransition::toString() const {
	return "TM Transition: [" + fromStateKey + " -> " + toStateKey + " | Read: " + readSymbol +
	       " | Write: " + writeSymbol + " | Move: " + TMDirectionHelper::toString(direction) + "]";
}
