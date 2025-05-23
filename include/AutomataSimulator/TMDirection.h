#pragma once
#include "config.h"
#include <string>

/**
 * @brief Represents the direction in which the tape head moves.
 */
enum class AUTOMATASIMULATOR_API TMDirection { LEFT, RIGHT, STAY };

class TMDirectionHelper {
  public:
	static std::string toString(TMDirection direction) {
		switch (direction) {
		case TMDirection::LEFT:
			return "LEFT";
		case TMDirection::RIGHT:
			return "RIGHT";
		case TMDirection::STAY:
			return "STAY";
		default:
			return "UNKNOWN";
		}
	}

	static TMDirection fromString(const std::string &direction) {
		if (direction == "LEFT") {
			return TMDirection::LEFT;
		} else if (direction == "RIGHT") {
			return TMDirection::RIGHT;
		} else if (direction == "STAY") {
			return TMDirection::STAY;
		} else {
			return TMDirection::STAY;
		}
	}
};