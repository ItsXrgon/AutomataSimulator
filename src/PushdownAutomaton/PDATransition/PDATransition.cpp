#include "AutomataSimulator/PDATransition.h"

PDATransition::PDATransition() : fromStateKey(""), toStateKey(""), input(""), stackSymbol(""), key("") {}

PDATransition::PDATransition(const std::string &fromStateKey, const std::string &toStateKey, const std::string &input,
                             const std::string &stackSymbol, const std::string &pushSymbol)
    : fromStateKey(fromStateKey), toStateKey(toStateKey), input(input), stackSymbol(stackSymbol),
      pushSymbol(pushSymbol) {
	key = generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
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

bool PDATransition::operator==(const PDATransition &other) const {
	return fromStateKey == other.fromStateKey && toStateKey == other.toStateKey && input == other.input &&
	       stackSymbol == other.stackSymbol && pushSymbol == other.pushSymbol;
}

PDATransition::~PDATransition() {}

std::string PDATransition::generateTransitionKey(const std::string &fromStateKey, const std::string &toStateKey,
                                                 const std::string &input, const std::string &stackSymbol,
                                                 const std::string &pushSymbol) {
	return fromStateKey + "-" + toStateKey + "-" + input + "-" + stackSymbol + "-" + pushSymbol;
}

void PDATransition::validateTransitionKeyFormat(const std::string &key) {
	int delimiterCount = std::count(key.begin(), key.end(), '-');
	if (delimiterCount != 4) {
		throw TransitionNotFoundException("Invalid PDA transition key format: " + key);
	}
}

std::string PDATransition::getFromStateFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t first = key.find('-');
	return key.substr(0, first);
}

std::string PDATransition::getToStateFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t first = key.find('-');
	size_t second = key.find('-', first + 1);
	return key.substr(first + 1, second - first - 1);
}

std::string PDATransition::getInputFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t first = key.find('-');
	size_t second = key.find('-', first + 1);
	size_t third = key.find('-', second + 1);
	return key.substr(second + 1, third - second - 1);
}

std::string PDATransition::getStackSymbolFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t third = key.find('-', key.find('-', key.find('-') + 1) + 1);
	size_t fourth = key.find('-', third + 1);
	return key.substr(third + 1, fourth - third - 1);
}

std::string PDATransition::getPushSymbolFromKey(const std::string &key) {
	validateTransitionKeyFormat(key);
	size_t lastDelimiter = key.find_last_of('-');
	return key.substr(lastDelimiter + 1);
}

std::string PDATransition::getKey() const {
	return key;
}

void PDATransition::setFromStateKey(const std::string &fromStateKey) {
	this->fromStateKey = fromStateKey;
	key = generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
}

std::string PDATransition::getFromStateKey() const {
	return fromStateKey;
}

void PDATransition::setToStateKey(const std::string &toState) {
	this->toStateKey = toState;
	key = generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
}

std::string PDATransition::getToStateKey() const {
	return toStateKey;
}

void PDATransition::setInput(const std::string &input) {
	this->input = input;
	key = generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
}

std::string PDATransition::getInput() const {
	return input;
}

void PDATransition::setStackSymbol(const std::string &stackSymbol) {
	this->stackSymbol = stackSymbol;
	key = generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
}

std::string PDATransition::getStackSymbol() const {
	return stackSymbol;
}

void PDATransition::setPushSymbol(const std::string &pushSymbol) {
	this->pushSymbol = pushSymbol;
	key = generateTransitionKey(fromStateKey, toStateKey, input, stackSymbol, pushSymbol);
}

std::string PDATransition::getPushSymbol() const {
	return pushSymbol;
}

std::string PDATransition::toString() const {
	return "Transition: [" + fromStateKey + " -> " + toStateKey + " | Input: " + input +
	       " | Stack Symbol: " + stackSymbol + " | Push Symbol: " + pushSymbol + "]";
}
