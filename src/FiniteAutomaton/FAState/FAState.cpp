#define AUTOMATASIMULATOR_EXPORTS
#include "FAState.h"
#include "../FiniteAutomatonException.h"

FAState::FAState(const std::string &label, const bool &isAccept) : key(label), label(label), isAccept(isAccept) {}

FAState::FAState(const FAState &other)
    : key(other.key), label(other.label), isAccept(other.isAccept), transitions(other.transitions) {}

FAState &FAState::operator=(const FAState &other) {
	if (this != &other) {
		key = other.key;
		label = other.label;
		isAccept = other.isAccept;
		transitions = other.transitions;
	}
	return *this;
}

FAState::FAState(FAState &&other) noexcept
    : key(std::move(other.key)), label(std::move(other.label)), isAccept(other.isAccept),
      transitions(std::move(other.transitions)) {}

FAState &FAState::operator=(FAState &&other) noexcept {
	if (this != &other) {
		key = std::move(other.key);
		label = std::move(other.label);
		isAccept = other.isAccept;
		transitions = std::move(other.transitions);
	}
	return *this;
}

FAState::~FAState() {}

FATransition *FAState::getTransition(const std::string &key) {
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

void FAState::addTransitionTo(const std::string &input, const std::string &toStateKey) {
	std::string transitionKey = FATransition::generateTransitionKey(key, toStateKey, input);
	if (transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " + toStateKey);
	}

	FATransition transition = FATransition(key, toStateKey, input);
	transitions[transition.getKey()] = transition;
}

void FAState::setTransitionInput(const std::string &transitionKey, const std::string &input) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string newTransitionKey = FATransition::generateTransitionKey(key, getTransitionToState(transitionKey), input);

	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " +
		                                 getTransitionToState(transitionKey));
	}

	FATransition *transition = getTransition(transitionKey);
	transition->setInput(input);
}

std::string FAState::getTransitionInput(const std::string &transitionKey) {
	FATransition *transition = getTransition(transitionKey);
	return transition->getInput();
}

void FAState::setTransitionToState(const std::string &transitionKey, const std::string &toState) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = FATransition::generateTransitionKey(key, toState, getTransitionInput(transitionKey));

	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " + toState);
	}

	FATransition *transition = getTransition(transitionKey);
	transition->setToStateKey(toState);
}

std::string FAState::getTransitionToState(const std::string &transitionKey) {
	FATransition *transition = getTransition(transitionKey);
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

void FAState::removeTransition(const std::string& transitionKey) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	transitions.erase(transitionKey);
}

void FAState::clearTransitionsTo(const std::string &toStateKey) {
	for (const auto &pair : transitions) {
		if (pair.second.getToStateKey() == toStateKey) {
			transitions.erase(pair.first);
		}
	}
}

void FAState::clearTransitions() {
	transitions.clear();
}

std::string FAState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}
