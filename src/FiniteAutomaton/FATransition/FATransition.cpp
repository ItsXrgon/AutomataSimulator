#include "AutomataSimulator/FATransition.h"

FATransition::FATransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), input(input) {
	key = generateTransitionKey(fromStateKey, toStateKey, input);
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

bool FATransition::operator==(const FATransition &other) const {
	return fromStateKey == other.fromStateKey && toStateKey == other.toStateKey && input == other.input;
}


FATransition::~FATransition() {}

std::string FATransition::generateTransitionKey(const std::string &fromStateKey, const std::string &toStateKey,
                                                const std::string &input) {
	return fromStateKey + "-" + toStateKey + "-" + input;
}

void FATransition::validateTransitionKeyFormat(const std::string &key) {
	// Expected format: fromState-toState-input
	int delimiterCount = std::count(key.begin(), key.end(), '-');
	if (delimiterCount != 2) {
		throw TransitionNotFoundException("Invalid FA transition key format: " + key);
	}
}

std::string FATransition::getFromStateFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	return key.substr(0, key.find('-'));
}

std::string FATransition::getToStateFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t first = key.find('-');
	size_t last = key.find_last_of('-');
	return key.substr(first + 1, last - first - 1);
}

std::string FATransition::getInputFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	return key.substr(key.find_last_of('-') + 1);
}

std::string FATransition::getKey() const {
	return key;
}

void FATransition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
	key = generateTransitionKey(fromStateKey, toStateKey, input);
}

std::string FATransition::getFromStateKey() const {
	return fromStateKey;
}

void FATransition::setToStateKey(const std::string &toStateKey) {
	this->toStateKey = toStateKey;
	key = generateTransitionKey(fromStateKey, toStateKey, input);
}

std::string FATransition::getToStateKey() const {
	return toStateKey;
}

void FATransition::setInput(const std::string &input) {
	this->input = input;
	key = generateTransitionKey(fromStateKey, toStateKey, input);
}

std::string FATransition::getInput() const {
	return input;
}

std::string FATransition::toString() const {
	return "Transition: [" + fromStateKey + " -> " + toStateKey + " | Input: " + input + "]";
}
