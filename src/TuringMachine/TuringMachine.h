#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include "State.h"
#include "Tape.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

// Main Turing Machine class
class TuringMachine {
  public:
	TuringMachine(const std::string &startState, char blankSymbol);

	// Add states and transitions
	void addState(const std::string &stateName, bool isAccept = false, bool isReject = false);
	void addTransition(const std::string &fromState, char read, char write, Direction move, const std::string &toState);

	// Simulation
	bool simulate(const std::string &input);

  private:
	std::unordered_map<std::string, State> states; // Collection of states
	std::string currentState;                      // Current active state
	Tape tape;                                     // Turing machine tape
};

#endif // TURING_MACHINE_H
