#ifndef STATE_H
#define STATE_H

#include "../TMTransition/TMTransition.h"
#include <string>
#include <unordered_map>

// Represents a Turing Machine state
class State {
  public:
	State(const std::string &name, bool isAccept = false, bool isReject = false);

	// Accessors
	const std::string &getName() const;
	bool isAcceptState() const;
	bool isRejectState() const;

	// Transition handling
	void addTransition(char read, const TMTransition &transition);
	bool hasTransition(char read) const;
	const TMTransition &getTransition(char read) const;

  private:
	std::string name;                                 // State name
	bool acceptState;                                 // Is it an accepting state?
	bool rejectState;                                 // Is it a rejecting state?
	std::unordered_map<char, TMTransition> transitions; // Transitions on read symbols
};

#endif // STATE_H
