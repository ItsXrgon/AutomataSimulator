#include "AutomataSimulator/c_api/TMTape_c.h"
#include "AutomataSimulator/TMDirection.h"
#include "AutomataSimulator/TMTape.h"
#include <cstring>

struct TMTape_t {
	TMTape instance;
};

TMTapeHandle TMTape_create() {
	return new TMTape_t{TMTape()};
}

TMTapeHandle TMTape_createWithBlankSymbol(const char *blankSymbol) {
	return new TMTape_t{TMTape(blankSymbol)};
}

void TMTape_destroy(TMTapeHandle tape) {
	delete tape;
}

void TMTape_loadInput(TMTapeHandle tape, const StringArray *input) {
	std::vector<std::string> symbols;
	symbols.reserve(input->length);
	for (size_t i = 0; i < input->length; ++i) {
		symbols.emplace_back(input->data[i]);
	}
	tape->instance.loadInput(symbols);
}

void TMTape_setBlankSymbol(TMTapeHandle tape, const char *blankSymbol) {
	tape->instance.setBlankSymbol(blankSymbol);
}

const char *TMTape_getBlankSymbol(TMTapeHandle tape) {
	return convertToCString(tape->instance.getBlankSymbol());
}

void TMTape_setHeadPosition(TMTapeHandle tape, int position) {
	tape->instance.setHeadPosition(position);
}

int TMTape_getHeadPosition(TMTapeHandle tape) {
	return tape->instance.getHeadPosition();
}

const char *TMTape_read(TMTapeHandle tape) {
	return convertToCString(tape->instance.read());
}

void TMTape_write(TMTapeHandle tape, const char *symbol) {
	tape->instance.write(symbol);
}

void TMTape_moveLeft(TMTapeHandle tape) {
	tape->instance.moveLeft();
}

void TMTape_moveRight(TMTapeHandle tape) {
	tape->instance.moveRight();
}

void TMTape_move(TMTapeHandle tape, const char *direction) {
	const TMDirection dir = TMDirectionHelper::fromString(direction);
	tape->instance.move(dir);
}

bool TMTape_isEmpty(TMTapeHandle tape) {
	return tape->instance.isEmpty();
}

void TMTape_reset(TMTapeHandle tape) {
	tape->instance.reset();
}

bool TMTape_isAtLeftEnd(TMTapeHandle tape) {
	return tape->instance.isAtLeftEnd();
}

const char *TMTape_toString(TMTapeHandle tape) {
	return convertToCString(tape->instance.toString());
}
