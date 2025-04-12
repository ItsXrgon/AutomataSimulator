#include "AutomataSimulator/PDAState.h"

PDAState::PDAState(const std::string &label, const bool &isAccept)
    : key(label), label(label), isAccept(isAccept), transitionsCacheInvalidated(false) {}

PDAState::PDAState(const PDAState &other)
    : key(other.key), label(other.label), isAccept(other.isAccept), transitions(other.transitions),
      transitionsCacheInvalidated(other.transitionsCacheInvalidated) {}

PDAState &PDAState::operator=(const PDAState &other) {
	if (this != &other) {
		key = other.key;
		label = other.label;
		isAccept = other.isAccept;
		transitions = other.transitions;
		transitionsCacheInvalidated = other.transitionsCacheInvalidated;
	}
	return *this;
}

PDAState::PDAState(PDAState &&other) noexcept
    : key(std::move(other.key)), label(std::move(other.label)), isAccept(other.isAccept),
      transitions(std::move(other.transitions)), transitionsCacheInvalidated(other.transitionsCacheInvalidated) {}

PDAState &PDAState::operator=(PDAState &&other) noexcept {
	if (this != &other) {
		key = std::move(other.key);
		label = std::move(other.label);
		isAccept = other.isAccept;
		transitions = std::move(other.transitions);
		transitionsCacheInvalidated = other.transitionsCacheInvalidated;
	}
	return *this;
}

bool PDAState::operator==(const PDAState &other) const {
	return key == other.key && label == other.label && isAccept == other.isAccept && transitions == other.transitions;
}

PDAState::~PDAState() {}

PDATransition *PDAState::getTransitionInternal(const std::string &key) {
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

	// Loop over all transitions to update their from state key.
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

void PDAState::addTransition(const std::string &toStateKey, const std::string &input, const std::string &stackSymbol,
                             const std::string &pushSymbol) {
	std::string transitionKey = PDATransition::generateTransitionKey(key, toStateKey, input, stackSymbol, pushSymbol);

	// Check if transition already exists
	if (transitionExists(transitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " + toStateKey +
		                                 " -> " + stackSymbol + " -> " + pushSymbol);
	}

	PDATransition transition = PDATransition(key, toStateKey, input, stackSymbol, pushSymbol);
	transitions[transition.getKey()] = transition;
	transitionsCacheInvalidated = true;
}

PDATransition PDAState::getTransition(const std::string &key) {
	auto it = transitions.find(key);
	// Check if state exists
	if (it == transitions.end()) {
		throw TransitionNotFoundException(key);
	}
	return it->second;
}

void PDAState::setTransitionInput(const std::string &transitionKey, const std::string &input) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}
	std::string newTransitionKey = PDATransition::generateTransitionKey(key, getTransitionToState(transitionKey), input,
	                                                                    getTransitionStackSymbol(transitionKey),
	                                                                    getTransitionPushSymbol(transitionKey));

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " + input + " -> " +
		                                 getTransitionToState(transitionKey));
	}

	PDATransition *transition = getTransitionInternal(transitionKey);
	transition->setInput(input);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string PDAState::getTransitionInput(const std::string &transitionKey) {
	PDATransition *transition = getTransitionInternal(transitionKey);
	return transition->getInput();
}

std::string PDAState::getTransitionToState(const std::string &transitionKey) {
	PDATransition *transition = getTransitionInternal(transitionKey);
	return transition->getToStateKey();
}

void PDAState::setTransitionToState(const std::string &transitionKey, const std::string &toState) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = PDATransition::generateTransitionKey(key, toState, getTransitionInput(transitionKey),
	                                                                    getTransitionStackSymbol(transitionKey),
	                                                                    getTransitionPushSymbol(transitionKey));

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " + toState);
	}

	PDATransition *transition = getTransitionInternal(transitionKey);
	transition->setToStateKey(toState);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string PDAState::getTransitionStackSymbol(const std::string &transitionKey) {
	PDATransition *transition = getTransitionInternal(transitionKey);
	return transition->getStackSymbol();
}

void PDAState::setTransitionStackSymbol(const std::string &transitionKey, const std::string &stackSymbol) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = PDATransition::generateTransitionKey(key, getTransitionToState(transitionKey),
	                                                                    getTransitionInput(transitionKey), stackSymbol,
	                                                                    getTransitionPushSymbol(transitionKey));

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " +
		                                 getTransitionToState(transitionKey));
	}
	PDATransition *transition = getTransitionInternal(transitionKey);
	transition->setStackSymbol(stackSymbol);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
}

std::string PDAState::getTransitionPushSymbol(const std::string &transitionKey) {
	PDATransition *transition = getTransitionInternal(transitionKey);
	return transition->getPushSymbol();
}

void PDAState::setTransitionPushSymbol(const std::string &transitionKey, const std::string &pushSymbol) {
	if (!transitionExists(transitionKey)) {
		throw TransitionNotFoundException(transitionKey);
	}

	std::string newTransitionKey = PDATransition::generateTransitionKey(
	    key, getTransitionToState(transitionKey), getTransitionInput(transitionKey),
	    getTransitionStackSymbol(transitionKey), pushSymbol);

	// Check if transition already exists
	if (transitionExists(newTransitionKey)) {
		throw InvalidTransitionException("Transition already exists: " + key + " -> " +
		                                 getTransitionInput(transitionKey) + " -> " +
		                                 getTransitionToState(transitionKey));
	}
	PDATransition *transition = getTransitionInternal(transitionKey);
	transition->setPushSymbol(pushSymbol);

	transitions[newTransitionKey] = *transition;
	transitions.erase(transitionKey);

	transitionsCacheInvalidated = true;
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
	transitionsCacheInvalidated = true;
}

void PDAState::clearTransitionsTo(const std::string &toStateKey) {
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

void PDAState::clearTransitions() {
	transitions.clear();
	transitionsCacheInvalidated = true;
}

std::string PDAState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}