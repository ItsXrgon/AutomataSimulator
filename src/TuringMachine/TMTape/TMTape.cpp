#include "AutomataSimulator/TMTape.h"
#include <sstream>

TMTape::TMTape() : blankSymbol("_"), headIndex(0) {
	tape.push_back(blankSymbol);
	head = tape.begin();
}

TMTape::TMTape(std::string blankSymbol) : blankSymbol(blankSymbol), headIndex(0) {
	tape.push_back(blankSymbol);
	head = tape.begin();
}

TMTape::TMTape(const TMTape &other) : tape(other.tape), blankSymbol(other.blankSymbol), headIndex(other.headIndex) {
	head = tape.begin();
	std::advance(head, headIndex);
}

TMTape &TMTape::operator=(const TMTape &other) {
	if (this != &other) {
		tape = other.tape;
		blankSymbol = other.blankSymbol;
		headIndex = other.headIndex;
		head = tape.begin();
		std::advance(head, headIndex);
	}
	return *this;
}

TMTape::TMTape(TMTape &&other) noexcept
    : tape(std::move(other.tape)), blankSymbol(std::move(other.blankSymbol)), headIndex(other.headIndex) {
	head = tape.begin();
	std::advance(head, headIndex);
}

TMTape &TMTape::operator=(TMTape &&other) noexcept {
	if (this != &other) {
		tape = std::move(other.tape);
		blankSymbol = std::move(other.blankSymbol);
		headIndex = other.headIndex;
		head = tape.begin();
		std::advance(head, headIndex);
	}
	return *this;
}

TMTape::~TMTape() {}

void TMTape::loadInput(const std::vector<std::string> &input) {
	tape.clear();
	for (const auto &s : input) {
		tape.push_back(s);
	}
	if (tape.empty()) {
		tape.push_back(blankSymbol);
	}
	head = tape.begin();
	headIndex = 0;
}

std::list<std::string> TMTape::getTape() const {
	return tape;
}

void TMTape::setTape(const std::list<std::string> &newTape) {
	tape = newTape;
	head = tape.begin();
	headIndex = 0;
}

void TMTape::setBlankSymbol(const std::string &blankSymbol) {
	for (auto &s : tape) {
		if (s == this->blankSymbol) {
			s = blankSymbol;
		}
	}
	this->blankSymbol = blankSymbol;
}

std::string TMTape::getBlankSymbol() const {
	return blankSymbol;
}

void TMTape::setHeadPosition(const int &position) {
	if (position < 0)
		return;
	while (position >= static_cast<int>(tape.size())) {
		tape.push_back(blankSymbol);
	}
	head = tape.begin();
	std::advance(head, position);
	headIndex = position;
}

int TMTape::getHeadPosition() const {
	return headIndex;
}

std::string TMTape::read() const {
	return *head;
}

void TMTape::write(const std::string &symbol) {
	*head = symbol;
}

void TMTape::moveLeft() {
	if (head == tape.begin()) {
		return;
	}

	--head;
	--headIndex;
}

void TMTape::moveRight() {
	++head;
	++headIndex;

	if (head == tape.end()) {
		// Append blank and reposition head
		tape.push_back(blankSymbol);
		head = std::prev(tape.end());
	}
}

void TMTape::move(const TMDirection &direction) {
	switch (direction) {
	case TMDirection::LEFT:
		moveLeft();
		break;
	case TMDirection::RIGHT:
		moveRight();
		break;
	case TMDirection::STAY:
	default:
		break;
	}
}

bool TMTape::isEmpty() const {
	return tape.empty();
}

void TMTape::reset() {
	tape.clear();
	tape.push_back(blankSymbol);
	head = tape.begin();
	headIndex = 0;
}

bool TMTape::isAtLeftEnd() const {
	return head == tape.begin();
}

std::string TMTape::toString() const {
	std::ostringstream oss;
	oss << "[";
	int i = 0;
	for (auto it = tape.begin(); it != tape.end(); ++it, ++i) {
		if (i == headIndex) {
			oss << "(" << *it << ")";
		} else {
			oss << *it;
		}
		if (std::next(it) != tape.end()) {
			oss << ", ";
		}
	}
	oss << "]";
	return oss.str();
}
