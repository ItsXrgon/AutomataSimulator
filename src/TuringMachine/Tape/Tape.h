#ifndef TAPE_H
#define TAPE_H

#include <map>
#include <string>

class Tape {
  public:
	explicit Tape(char blankSymbol);

	// Tape manipulation methods
	char read() const;       // Read the current symbol
	void write(char symbol); // Write a symbol
	void moveLeft();         // Move head to the left
	void moveRight();        // Move head to the right

	std::string toString() const; // Get tape contents as a string

  private:
	std::map<int, char> tape; // Simulate an infinite tape (sparse map)
	int headPosition;         // Current head position
	char blankSymbol;         // Default blank symbol
};

#endif // TAPE_H
