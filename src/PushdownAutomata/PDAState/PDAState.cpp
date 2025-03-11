#define AUTOMATASIMULATOR_EXPORTS
#include "PDAState.h"
#include "../PushdownAutomatonException.h"

PDAState::PDAState(const std::string &label, const bool &isAccept) {
	this->label = label;
	this->isAccept = isAccept;
	key = label;
	transitions = std::vector<PDATransition>();
}

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

		other.isAccept = false;
	}
	return *this;
}

PDAState::~PDAState() {}

std::string PDAState::getKey() const {
	return key;
}

void PDAState::setLabel(const std::string &label) {
	this->label = label;
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
	PDATransition transition = PDATransition(key, toStateKey, input, stackSymbol, pushSymbol);
	transitions.push_back(transition);
}

std::vector<PDATransition> PDAState::getTransitions() const {
	return this->transitions;
}

void PDAState::removeTransitionTo(const std::string &input, const std::string &toStateKey, const std::string &stackSymbol,
                               const std::string &pushSymbol) {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		if (it->getInput() == input && it->getToStateKey() == toStateKey && it->getStackSymbol() == stackSymbol &&
		    it->getPushSymbol() == pushSymbol) {
			transitions.erase(it);
			return;
		}
	}
	throw TransitionNotFoundException("Transition not found: " + key + " -> " + input + " -> " + toStateKey);
}

void PDAState::clearTransitionsTo(const std::string &toStateKey) {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		if (it->getToStateKey() == toStateKey) {
			transitions.erase(it);
			return;
		}
	}
}

void PDAState::clearTransitions() {
	transitions.clear();
}

std::string PDAState::toString() const {
	return "State: [" + label + " | Accept State: " + (isAccept ? "Yes" : "No") + "]";
}