#define AUTOMATASIMULATOR_EXPORTS
#include "PDAState.h"
#include "../PushdownAutomatonException.h"

PDAState::PDAState(const std::string &label, const bool &isAccept) : key(label), label(label), isAccept(isAccept) {}

PDAState::PDAState(const PDAState &other)
    : key(other.key), label(other.label), isAccept(other.isAccept), transitions(other.transitions) {}

PDAState &PDAState::operator=(const PDAState &other) {
	if (this != &other) {
		key = other.key;
		label = other.label;
		isAccept = other.isAccept;
		transitions = other.transitions;
	}
	return *this;
}

PDAState::PDAState(PDAState &&other) noexcept
    : key(std::move(other.key)), label(std::move(other.label)), isAccept(other.isAccept),
      transitions(std::move(other.transitions)) {}

PDAState &PDAState::operator=(PDAState &&other) noexcept {
	if (this != &other) {
		key = std::move(other.key);
		label = std::move(other.label);
		isAccept = other.isAccept;
		transitions = std::move(other.transitions);
	}
	return *this;
}

PDAState::~PDAState() {}

PDATransition *PDAState::getTransition(const std::string &key) {
	auto it = transitions.find(key);
	if (it == transitions.end()) {
		throw TransitionNotFoundException(key);
	}
	return &(it->second);
}

bool PDAState::transitionExists(const std::string &key) const {
	return transitions.find(key) != transitions.end();
}

std::string PDAState::getKey() const {
	return key;
}

void PDAState::setLabel(const std::string &label) {
	this->label = label;
	this->key = label;

	std::unordered_map<std::string, PDATransition> newTransitions;

	for (const auto &pair : transitions) {
		PDATransition transition = pair.second;
		transition.setFromStateKey(key);
		newTransitions[transition.getKey()] = transition;
	}
	transitions = newTransitions;
	transitionsCacheInvalidated = true;
}

std::string PDAState::getLabel() const {
	return label;
}

void PDAState::setIsAccept(const bool &isAccept) {
	this->isAccept = isAccept;
}

bool PDAState::getIsAccept() const {
	return isAccept;
}

void PDAState::addTransitionTo(const std::string &input, const std::string &toStateKey, const std::string &stackSymbol,
                               const std::string &pushSymbol) {
	std::string transitionKey = PDATransition::generateTransitionKey(key, toStateKey, input, stackSymbol, pushSymbol);
	if (transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " + toStateKey +
		                                 " -> " + stackSymbol + " -> " + pushSymbol);
	}

	PDATransition transition = PDATransition(key, toStateKey, input, stackSymbol, pushSymbol);
	transitions[transition.getKey()] = transition;
}

void PDAState::setTransitionInput(const std::string &transitionKey, const std::string &input) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string newTransitionKey = PDATransition::generateTransitionKey(key, getTransitionToState(transitionKey), input,
	                                                                    getTransitionStackSymbol(transitionKey),
	                                                                    getTransitionPushSymbol(transitionKey));

	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " +
		                                 getTransitionToState(transitionKey));
	}

	PDATransition *transition = getTransition(transitionKey);
	transition->setInput(input);
}

std::string PDAState::getTransitionInput(const std::string &transitionKey) {
	PDATransition *transition = getTransition(transitionKey);
	return transition->getInput();
}

void PDAState::setTransitionToState(const std::string &transitionKey, const std::string &toState) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = PDATransition::generateTransitionKey(key, toState, getTransitionInput(transitionKey),
	                                                                    getTransitionStackSymbol(transitionKey),
	                                                                    getTransitionPushSymbol(transitionKey));

	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " + toState);
	}

	PDATransition *transition = getTransition(transitionKey);
	transition->setToStateKey(toState);
}

std::string PDAState::getTransitionToState(const std::string &transitionKey) {
	PDATransition *transition = getTransition(transitionKey);
	return transition->getToStateKey();
}

std::string PDAState::getTransitionStackSymbol(const std::string &transitionKey) {
	PDATransition *transition = getTransition(transitionKey);
	return transition->getStackSymbol();
}

void PDAState::setTransitionStackSymbol(const std::string& transitionKey, const std::string& stackSymbol) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string newTransitionKey = PDATransition::generateTransitionKey(key, getTransitionToState(transitionKey),
	                                                                    getTransitionInput(transitionKey), stackSymbol,
	                                                                    getTransitionPushSymbol(transitionKey));
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " +
		                                 getTransitionToState(transitionKey));
	}
	PDATransition *transition = getTransition(transitionKey);
	transition->setStackSymbol(stackSymbol);
}

std::string PDAState::getTransitionPushSymbol(const std::string &transitionKey) {
	PDATransition *transition = getTransition(transitionKey);
	return transition->getPushSymbol();
}

void PDAState::setTransitionPushSymbol(const std::string &transitionKey, const std::string &pushSymbol) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string newTransitionKey = PDATransition::generateTransitionKey(
	    key, getTransitionToState(transitionKey), getTransitionInput(transitionKey),
	    getTransitionStackSymbol(transitionKey), pushSymbol);

	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " +
		                                 getTransitionToState(transitionKey));
	}
	PDATransition *transition = getTransition(transitionKey);
	transition->setPushSymbol(pushSymbol);
}

std::vector<PDATransition> PDAState::getTransitions() {
	if (transitionsCacheInvalidated) {
		cachedTransitions.clear();
		for (const auto &pair : transitions) {
			cachedTransitions.push_back(pair.second);
		}
		transitionsCacheInvalidated = false;
	}
	return cachedTransitions;
}

void PDAState::removeTransition(const std::string &transitionKey) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	transitions.erase(transitionKey);
}

std::vector<PDATransition> PDAState::getTransitions() {
	if (transitionsCacheInvalidated) {
		cachedTransitions.clear();
		for (const auto &pair : transitions) {
			cachedTransitions.push_back(pair.second);
		}
		transitionsCacheInvalidated = false;
	}
	return cachedTransitions;
}

void PDAState::clearTransitionsTo(const std::string &toStateKey) {
	for (const auto &pair : transitions) {
		if (pair.second.getToStateKey() == toStateKey) {
			transitions.erase(pair.first);
		}
	}
}

void PDAState::clearTransitions() {
	transitions.clear();
}

std::string PDAState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}