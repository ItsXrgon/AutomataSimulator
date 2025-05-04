#include "AutomataSimulator/TMState.h"

TMState::TMState() : key(""), label(""), isAccept(false), transitionsCacheInvalidated(false) {}

TMState::TMState(const std::string &label, const bool &isAccept)
    : key(label), label(label), isAccept(isAccept), transitionsCacheInvalidated(false) {}

TMState::TMState(const TMState &other)
    : key(other.key), label(other.label), isAccept(other.isAccept), transitions(other.transitions),
      transitionsCacheInvalidated(other.transitionsCacheInvalidated) {}

TMState &TMState::operator=(const TMState &other) {
	if (this != &other) {
		key = other.key;
		label = other.label;
		isAccept = other.isAccept;
		transitions = other.transitions;
		transitionsCacheInvalidated = other.transitionsCacheInvalidated;
	}
	return *this;
}

TMState::TMState(TMState &&other) noexcept
    : key(std::move(other.key)), label(std::move(other.label)), isAccept(other.isAccept),
      transitions(std::move(other.transitions)), transitionsCacheInvalidated(other.transitionsCacheInvalidated) {}

TMState &TMState::operator=(TMState &&other) noexcept {
	if (this != &other) {
		key = std::move(other.key);
		label = std::move(other.label);
		isAccept = other.isAccept;
		transitions = std::move(other.transitions);
		transitionsCacheInvalidated = other.transitionsCacheInvalidated;
	}
	return *this;
}

bool TMState::operator==(const TMState &other) const {
	return key == other.key && label == other.label && isAccept == other.isAccept && transitions == other.transitions;
}

TMState::~TMState() {}

TMTransition *TMState::getTransitionInternal(const std::string &key) {
	auto it = transitions.find(key);
	if (it == transitions.end()) {
		throw TransitionNotFoundException(key);
	}
	return &(it->second);
}

bool TMState::transitionExists(const std::string &key) const {
	return transitions.find(key) != transitions.end();
}

std::string TMState::getKey() const {
	return key;
}

void TMState::setLabel(const std::string &label) {
	this->label = label;
	this->key = label;

	std::unordered_map<std::string, TMTransition> newTransitions;

	// Loop over all transitions to update their from state key.
	for (const auto &pair : transitions) {
		TMTransition transition = pair.second;
		transition.setFromStateKey(key);
		newTransitions[transition.getKey()] = transition;
	}
	transitions = newTransitions;
	transitionsCacheInvalidated = true;
}

std::string TMState::getLabel() const {
	return label;
}

void TMState::setIsAccept(const bool &isAccept) {
	this->isAccept = isAccept;
}

bool TMState::getIsAccept() const {
	return isAccept;
}

void TMState::addTransition(const std::string &toStateKey, const std::string &readSymbol,
                            const std::string &writeSymbol, const TMDirection &direction) {
	std::string transitionKey =
	    TMTransition::generateTransitionKey(key, toStateKey, readSymbol, writeSymbol, direction);

	// Check if transition already exists
	if (transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + toStateKey +
		                                 " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                 " | direction: " + TMDirectionHelper::toString(direction));
	}

	TMTransition transition = TMTransition(key, toStateKey, readSymbol, writeSymbol, direction);
	transitions[transition.getKey()] = transition;
	transitionsCacheInvalidated = true;
}

TMTransition TMState::getTransition(const std::string &key) {
	auto it = transitions.find(key);
	// Check if state exists
	if (it == transitions.end()) {
		throw TransitionNotFoundException(key);
	}
	return it->second;
}

void TMState::setTransitionToState(const std::string &transitionKey, const std::string &toStateKey) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = TMTransition::generateTransitionKey(
	    key, toStateKey, getTransitionReadSymbol(transitionKey), getTransitionWriteSymbol(transitionKey),
	    getTransitionDirection(transitionKey));

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
		std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
		TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

		throw InvalidTransitionException("Transition already exists: " + key + " -> " + toStateKey +
		                                 " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                 " | direction: " + TMDirectionHelper::toString(direction));
	}

	TMTransition *transition = getTransitionInternal(transitionKey);
	transition->setToStateKey(toStateKey);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string TMState::getTransitionToState(const std::string &transitionKey) {
	TMTransition *transition = getTransitionInternal(transitionKey);
	return transition->getToStateKey();
}

std::string TMState::getTransitionReadSymbol(const std::string &transitionKey) {
	TMTransition *transition = getTransitionInternal(transitionKey);
	return transition->getReadSymbol();
}

void TMState::setTransitionReadSymbol(const std::string &transitionKey, const std::string &stackSymbol) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = TMTransition::generateTransitionKey(
	    key, getTransitionToState(transitionKey), stackSymbol, getTransitionWriteSymbol(transitionKey),
	    getTransitionDirection(transitionKey));

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionToState(transitionKey) + ", " +
		                                 TMDirectionHelper::toString(getTransitionDirection(transitionKey)));
		;
	}
	TMTransition *transition = getTransitionInternal(transitionKey);
	transition->setReadSymbol(stackSymbol);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string TMState::getTransitionWriteSymbol(const std::string &transitionKey) {
	TMTransition *transition = getTransitionInternal(transitionKey);
	return transition->getWriteSymbol();
}

void TMState::setTransitionWriteSymbol(const std::string &transitionKey, const std::string &pushSymbol) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = TMTransition::generateTransitionKey(
	    key, getTransitionToState(transitionKey), getTransitionReadSymbol(transitionKey), pushSymbol,
	    getTransitionDirection(transitionKey));

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
		std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
		std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);
		TMDirection direction = TMTransition::getDirectionFromKey(transitionKey);

		throw InvalidTransitionException("Transition already exists: " + key + " -> " + toStateKey +
		                                 " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                 " | direction: " + TMDirectionHelper::toString(direction));
	}
	TMTransition *transition = getTransitionInternal(transitionKey);
	transition->setWriteSymbol(pushSymbol);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

TMDirection TMState::getTransitionDirection(const std::string &transitionKey) {
	TMTransition *transition = getTransitionInternal(transitionKey);
	return transition->getDirection();
}

void TMState::setTransitionDirection(const std::string &transitionKey, const TMDirection &direction) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string newTransitionKey = TMTransition::generateTransitionKey(
	    key, getTransitionToState(transitionKey), getTransitionReadSymbol(transitionKey),
	    getTransitionWriteSymbol(transitionKey), direction);

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		std::string toStateKey = TMTransition::getToStateFromKey(transitionKey);
		std::string readSymbol = TMTransition::getReadSymbolFromKey(transitionKey);
		std::string writeSymbol = TMTransition::getWriteSymbolFromKey(transitionKey);

		throw InvalidTransitionException("Transition already exists: " + key + " -> " + toStateKey +
		                                 " | read symbol: " + readSymbol + " | write symbol: " + writeSymbol +
		                                 " | direction: " + TMDirectionHelper::toString(direction));
	}
	TMTransition *transition = getTransitionInternal(transitionKey);
	transition->setDirection(direction);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::vector<TMTransition> TMState::getTransitions() {
	if (transitionsCacheInvalidated) {
		cachedTransitions.clear();
		for (const auto &pair : transitions) {
			cachedTransitions.push_back(pair.second);
		}
		transitionsCacheInvalidated = false;
	}
	return cachedTransitions;
}

void TMState::removeTransition(const std::string &transitionKey) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	transitions.erase(transitionKey);
	transitionsCacheInvalidated = true;
}

void TMState::clearTransitionsTo(const std::string &toStateKey) {
	auto it = transitions.begin();
	while (it != transitions.end()) {
		if (it->second.getToStateKey() == toStateKey) {
			it = transitions.erase(it);
		} else {
			++it;
		}
	}
	transitionsCacheInvalidated = true;
}

void TMState::clearTransitions() {
	transitions.clear();
	transitionsCacheInvalidated = true;
}

std::string TMState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}