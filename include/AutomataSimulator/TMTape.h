#pragma once
#include "AutomatonException.h"
#include "TMDirection.h"
#include <list>
#include <string>
#include <vector>

class TMTape {
  private:
	/**
	 * @brief The tape of the Turing Machine.
	 */
	std::list<std::string> tape;

	/**
	 *  The current tape head
	 */
	std::list<std::string>::iterator head;

	/**
	 * @brief The position of the head on the tape.
	 */
	int headIndex;

	/**
	 * @brief The blank symbol of the tape.
	 */
	std::string blankSymbol;

  public:
	/**
	 * @brief Constructs a new Tape object.
	 */
	TMTape();

	/**
	 * @brief Constructs a new Tape object.
	 */
	TMTape(std::string blankSymbol);

	/**
	 * @brief Copy constructor for the Tape object.
	 * @param other The Tape object to copy.
	 */
	TMTape(const TMTape &other);

	/**
	 * @brief Copy assignment operator for the Tape object.
	 * @param other The Tape object to copy.
	 * @return A reference to the copied Tape object.
	 */
	TMTape &operator=(const TMTape &other);

	/**
	 * @brief Move constructor for the Tape object.
	 * @param other The Tape object to move.
	 */
	TMTape(TMTape &&other) noexcept;

	/**
	 * @brief Move assignment operator for the Tape object.
	 * @param other The Tape object to move.
	 * @return A reference to the moved Tape object.
	 */
	TMTape &operator=(TMTape &&other) noexcept;

	/**
	 * @brief Deconstructor for the Tape object
	 */
	~TMTape();

	/**
	 * @brief Gets the tape of the Turing Machine.
	 * @return The tape of the Turing Machine.
	 */
	std::list<std::string> getTape() const;

	/**
	 * @brief Sets the tape to the new tape.
	 * @param tape The new tape to set.
	 */
	void setTape(const std::list<std::string> &tape);

	/**
	 * @brief Loads the input onto the tape.
	 * @param input The input to load onto the tape.
	 */
	void loadInput(const std::vector<std::string> &input);

	/**
	 * @brief Sets the blank symbol of the tape.
	 * @brief This update all blank symbols on the tape to the new symbol.
	 * @param blankSymbol The blank symbol to set.
	 */
	void setBlankSymbol(const std::string &blankSymbol);

	/**
	 * @brief Gets the blank symbol of the tape.
	 * @return The blank symbol of the tape
	 */
	std::string getBlankSymbol() const;

	/**
	 * @brief Sets the current head position.
	 */
	void setHeadPosition(const int &position);

	/**
	 * @brief Gets the current head position.
	 * @return The current head position.
	 */
	int getHeadPosition() const;

	/**
	 * @brief Read the current symbol
	 * @return The current symbol
	 */
	std::string read() const;

	/**
	 * @brief Writes a symbol to the tape
	 * @param symbol The symbol to write
	 */
	void write(const std::string &symbol);

	/**
	 * @brief Moves the the tape according to direction given.
	 */
	void move(const TMDirection &direction);

	/**
	 * @brief Moves the head of the tape to the left
	 */
	void moveLeft();

	/**
	 * @brief Moves the head of the tape to the right
	 */
	void moveRight();

	/**
	 * @brief Checks if the tape is empty
	 */
	bool isEmpty() const;

	/**
	 * @brief Clears the tape and reset the head position
	 */
	void reset();

	/**
	 * @brief Checks if the tape is at the left end
	 */
	bool isAtLeftEnd() const;

	/**
	 * @brief Converts the tape to a string
	 * @return The tape as a string
	 */
	std::string toString() const;
};