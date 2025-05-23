#include "AutomataSimulator/FAState.h"


FAState::FAState() : key(""), label(""), isAccept(false), transitionsCacheInvalidated(false) {}

FAState::FAState(const std::string &label, const bool &isAccept)
    : key(label), label(label), isAccept(isAccept), transitionsCacheInvalidated(false) {}

FAState::FAState(const FAState &other)
    : key(other.key), label(other.label), isAccept(other.isAccept), transitions(other.transitions),
      transitionsCacheInvalidated(other.transitionsCacheInvalidated) {}

FAState &FAState::operator=(const FAState &other) {
	if (this != &other) {
		key = other.key;
		label = other.label;
		isAccept = other.isAccept;
		transitions = other.transitions;
		transitionsCacheInvalidated = true;
	}
	return *this;
}

FAState::FAState(FAState &&other) noexcept
    : key(std::move(other.key)), label(std::move(other.label)), isAccept(other.isAccept),
      transitionsCacheInvalidated(true), transitions(std::move(other.transitions)) {}

FAState &FAState::operator=(FAState &&other) noexcept {
	if (this != &other) {
		key = std::move(other.key);
		label = std::move(other.label);
		isAccept = other.isAccept;
		transitions = std::move(other.transitions);
		transitionsCacheInvalidated = true;
	}
	return *this;
}

bool FAState::operator==(const FAState &other) const {
	return key == other.key && label == other.label && isAccept == other.isAccept && transitions == other.transitions;
}

FAState::~FAState() {}

FATransition *FAState::getTransitionInternal(const std::string &key) {
	auto it = transitions.find(key);
	if (it == transitions.end()) {
		throw TransitionNotFoundException(key);
	}
	return &(it->second);
}

bool FAState::transitionExists(const std::string &key) const {
	return transitions.find(key) != transitions.end();
}

std::string FAState::getKey() const {
	return key;
}

void FAState::setLabel(const std::string &label) {
	this->label = label;
	this->key = label;

	std::unordered_map<std::string, FATransition> newTransitions;

	// Loop over all transitions to update their from state key.
	for (const auto &pair : transitions) {
		FATransition transition = pair.second;
		transition.setFromStateKey(key);
		newTransitions[transition.getKey()] = transition;
	}
	transitions = newTransitions;
	transitionsCacheInvalidated = true;
}

std::string FAState::getLabel() const {
	return label;
}

void FAState::setIsAccept(const bool &isAccept) {
	this->isAccept = isAccept;
}

bool FAState::getIsAccept() const {
	return isAccept;
}

void FAState::addTransition(const std::string &toStateKey, const std::string &input) {
	std::string transitionKey = FATransition::generateTransitionKey(key, toStateKey, input);

	// Check if transition already exists
	if (transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " + toStateKey);
	}

	FATransition transition = FATransition(key, toStateKey, input);
	transitions[transition.getKey()] = transition;
	transitionsCacheInvalidated = true;
}

FATransition FAState::getTransition(const std::string& key) {
	auto it = transitions.find(key);
	// Check if state exists
	if (it == transitions.end()) {
		throw TransitionNotFoundException(key);
	}
	return it->second;
}

void FAState::setTransitionInput(const std::string &transitionKey, const std::string &input) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = FATransition::generateTransitionKey(key, getTransitionToState(transitionKey), input);

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " +
		                                 getTransitionToState(transitionKey));
	}

	FATransition *transition = getTransitionInternal(transitionKey);
	transition->setInput(input);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string FAState::getTransitionInput(const std::string &transitionKey) {
	FATransition *transition = getTransitionInternal(transitionKey);
	return transition->getInput();
}

void FAState::setTransitionToState(const std::string &transitionKey, const std::string &toState) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string input = getTransitionInput(transitionKey);
	std::string newTransitionKey = FATransition::generateTransitionKey(key, toState, input);

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " + toState);
	}

	FATransition *transition = getTransitionInternal(transitionKey);
	transition->setToStateKey(toState);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string FAState::getTransitionToState(const std::string &transitionKey) {
	FATransition *transition = getTransitionInternal(transitionKey);
	return transition->getToStateKey();
}

std::vector<FATransition> FAState::getTransitions() {
	if (transitionsCacheInvalidated) {
		cachedTransitions.clear();
		for (const auto &pair : transitions) {
			cachedTransitions.push_back(pair.second);
		}
		transitionsCacheInvalidated = false;
	}
	return cachedTransitions;
}

void FAState::removeTransition(const std::string &transitionKey) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	transitions.erase(transitionKey);
	transitionsCacheInvalidated = true;
}

void FAState::clearTransitionsTo(const std::string &toStateKey) {
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

void FAState::clearTransitions() {
	transitions.clear();
	transitionsCacheInvalidated = true;
}

std::string FAState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}
