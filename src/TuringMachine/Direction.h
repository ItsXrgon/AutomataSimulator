#ifndef DIRECTION_H
#define DIRECTION_H
#include <string>

// Enum representing head movement direction
enum class Direction { LEFT, RIGHT, STAY };

class DirectionHelper {
  public:
	static std::string toString(Direction dir) {
		switch (dir) {
		case Direction::LEFT:
			return "LEFT";
		case Direction::RIGHT:
			return "RIGHT";
		case Direction::STAY:
			return "STAY";
		default:
			return "UNKNOWN";
		}
	}
};

#endif // DIRECTION_H
