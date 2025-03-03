#define AUTOMATASIMULATOR_EXPORTS
#include "State.h"
#include "../PushdownAutomataException.h"

State::State(const std::string &label, const bool &isAccept) {
	this->label = label;
	this->isAccept = isAccept;
	key = label;
	transitions = std::vector<Transition>();
}

State::State(const State &other)
    : key(other.key), label(other.label), isAccept(other.isAccept), transitions(other.transitions) {}

State &State::operator=(const State &other) {
	if (this != &other) {
		key = other.key;
		label = other.label;
		isAccept = other.isAccept;
		transitions = other.transitions;
	}
	return *this;
}

State::State(State &&other) noexcept
    : key(std::move(other.key)), label(std::move(other.label)), isAccept(other.isAccept),
      transitions(std::move(other.transitions)) {}

State &State::operator=(State &&other) noexcept {
	if (this != &other) {
		key = std::move(other.key);
		label = std::move(other.label);
		isAccept = other.isAccept;
		transitions = std::move(other.transitions);

		other.isAccept = false;
	}
	return *this;
}

State::~State() {}

std::string State::getKey() const {
	return key;
}

void State::setLabel(const std::string &label) {
	this->label = label;
}

std::string State::getLabel() const {
	return label;
}

void State::setIsAccept(const bool &isAccept) {
	this->isAccept = isAccept;
}

bool State::getIsAccept() const {
	return isAccept;
}

void State::addTransitionTo(const std::string &input, const std::string &toStateKey) {
	Transition transition = Transition(key, toStateKey, input);
	transitions.push_back(transition);
}

std::vector<Transition> State::getTransitions() const {
	return this->transitions;
}

void State::removeTransitionTo(const std::string &input, const std::string &toStateKey) {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		if (it->getInput() == input && it->getToStateKey() == toStateKey) {
			transitions.erase(it);
			return;
		}
	}
	throw TransitionNotFoundException("Transition not found: " + key + " -> " + input + " -> " + toStateKey);
}

void State::clearTransitionsTo(const std::string &toStateKey) {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		if (it->getToStateKey() == toStateKey) {
			transitions.erase(it);
			return;
		}
	}
}

void State::clearTransitions() {
	transitions.clear();
}