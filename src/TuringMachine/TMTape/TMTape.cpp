#include "TMTape.h"

TMTape::TMTape() : blankSymbol("_"), headPosition(0) {}

TMTape::TMTape(std::string blankSymbol) : blankSymbol(blankSymbol), headPosition(0) {}

TMTape::TMTape(const TMTape &other)
    : tape(other.tape), headPosition(other.headPosition), blankSymbol(other.blankSymbol) {}

TMTape &TMTape::operator=(const TMTape &other) {
	if (this != &other) {
		tape = other.tape;
		headPosition = other.headPosition;
		blankSymbol = other.blankSymbol;
	}
	return *this;
}

TMTape::TMTape(TMTape &&other) noexcept
    : tape(std::move(other.tape)), headPosition(other.headPosition), blankSymbol(other.blankSymbol) {}

TMTape &TMTape::operator=(TMTape &&other) noexcept {
	if (this != &other) {
		tape = std::move(other.tape);
		headPosition = other.headPosition;
		blankSymbol = other.blankSymbol;
	}
	return *this;
}

TMTape::~TMTape() {}

void TMTape::setTape(const std::map<int, std::string> &tape) {
	this->tape = tape;
}

std::map<int, std::string> TMTape::getTape() const {
	return tape;
}

void TMTape::setBlankSymbol(std::string blankSymbol) {
	for (auto &pair : tape) {
		if (pair.second == this->blankSymbol) {
			pair.second = blankSymbol;
		}
	}
	this->blankSymbol = blankSymbol;
}

std::string TMTape::getBlankSymbol() const {
	return blankSymbol;
}

void TMTape::setHeadPosition(const int &position) {
	headPosition = position;
}

int TMTape::getHeadPosition() const {
	return headPosition;
}

std::string TMTape::read() const {
	if (tape.find(headPosition) == tape.end()) {
		return blankSymbol;
	}
	return tape.at(headPosition);
}

void TMTape::write(std::string symbol) {
	tape[headPosition] = symbol;
}

void TMTape::moveLeft() {
	if (isAtLeftEnd()) {
		return;
	}
	headPosition--;
}

void TMTape::move(TMDirection direction) {
	switch (direction) {
	case TMDirection::LEFT:
		moveLeft();
		break;
	case TMDirection::RIGHT:
		moveRight();
		break;
	default:
		break;
	}
}

void TMTape::moveRight() {
	headPosition++;
}

bool TMTape::isEmpty() const {
	return tape.empty();
}

void TMTape::reset() {
	tape.clear();
	headPosition = 0;
}

bool TMTape::isAtLeftEnd() const {
	return headPosition == 0;
}

std::string TMTape::toString() const {
	std::string tapeStr = "[";
	for (auto it = tape.begin(); it != tape.end(); ++it) {
		tapeStr += it->second + ", ";
	}
	tapeStr.erase(tapeStr.size() - 2, 2);
	tapeStr += "]";
	return tapeStr;
}