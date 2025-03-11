#define AUTOMATASIMULATOR_EXPORTS
#include "FAState.h"
#include "../FiniteAutomatonException.h"

FAState::FAState(const std::string &label, const bool &isAccept) {
	this->label = label;
	this->isAccept = isAccept;
	key = label;
	transitions = std::vector<FATransition>();
}

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

		other.isAccept = false;
	}
	return *this;
}

FAState::~FAState() {}

std::string FAState::getKey() const {
	return key;
}

void FAState::setLabel(const std::string &label) {
	this->label = label;
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
	FATransition transition = FATransition(key, toStateKey, input);
	transitions.push_back(transition);
}

std::vector<FATransition> FAState::getTransitions() const {
	return this->transitions;
}

void FAState::removeTransitionTo(const std::string &input, const std::string &toStateKey) {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		if (it->getInput() == input && it->getToStateKey() == toStateKey) {
			transitions.erase(it);
			return;
		}
	}
	throw TransitionNotFoundException("Transition not found: " + key + " -> " + input + " -> " + toStateKey);
}

void FAState::clearTransitionsTo(const std::string &toStateKey) {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		if (it->getToStateKey() == toStateKey) {
			transitions.erase(it);
			return;
		}
	}
}

void FAState::clearTransitions() {
	transitions.clear();
}

std::string FAState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}
