#pragma once
#include <string>
#include <vector>
#include "../Transition/Transition.h"

#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif

/**
 * @brief Represents a state in an automaton.
 * A state is defined by a "label" string, a vector of transitions that start from it, and a boolean indicating whether it is an accept state.
 */
class AUTOMATASIMULATOR_API State
{
private:
	/**
	 * @brief Unique identifier for the transition, formatted as "label".
	 */
	std::string key;

	/**
	 * @brief Label for the state.
	 */
	std::string label;

	/**
	 * @brief Transitions the start from the state.
	 */
	std::vector<Transition> transitions;

	/**
	 * @brief Boolean indicating whether it is an accept state.
	 */
	bool isAccept;
public:
	/**
	 * @brief Constructs a new State object.
	 * @param label The label for the state.
	 * @param isAccept The boolean indicating whether it is an accept state.
	 */
	State(const std::string& label, const bool& isAccept);

	/**
	 * @brief Copy constructor for the State object.
	 * @param other The State object to copy.
	 */
	State(const State& other);

	/**
	 * @brief Copy assignment operator for the State object.
	 * @param other The State object to copy.
	 * @return A reference to the copied State object.
	 */
	State& operator=(const State& other);

	/**
	 * @brief Move constructor for the State object.
	 * @param other The State object to move.
	 */
	State(State&& other) noexcept;

	/**
	 * @brief Move assignment operator for the State object.
	 * @param other The State object to move.
	 * @return A reference to the moved State object.
	 */
	State& operator=(State&& other) noexcept;

	/**
	 * @brief Destructor for the State object.
	 */
	~State();

	/**
	 * @brief Gets the unique key for this transition.
	 * @return The key string.
	 */
	std::string getKey() const;

	/**
	* @brief Sets the label for the state.
	* @param label The label for the state.
	*/
	void setLabel(const std::string& label);

	/**
	* @brief Gets the label for the state.
	* @return The label for the state.
	*/
	std::string getLabel() const;

	/**
	 * @brief Sets whether the state is an accept state.
	 * @param isAccept True if the state is an accept state, false otherwise.
	 */
	void setIsAccept(const bool& isAccept);

	/**
	 * @brief Returns whether the state is an accept state.
	 * @return True if the state is an accept state, false otherwise.
	 */
	bool getIsAccept() const;

	/**
	 * @brief Adds a transition to the state's transitions vector.
	 * @param input The input of the transition.
	 * @param toStateKey The to state key of the transition.
	 */
	void addTransitionTo(const std::string& input, const std::string& toStateKey);
	
	/**
	 * @brief Removes a transition to the state's transitions vector.
	 * @param input The input of the transition.
	 * @param toStateKey The to state key of the transition.
	 */
	void removeTransitionTo(const std::string& input, const std::string& toStateKey);

	/**
	 * @brief Clears transitions to the to state from the state's transitions vector.
	 * @param toStateKey The to state key of the transition.
	 */
	void clearTransitionsTo(const std::string& toStateKey);

	/**
	 * @brief Gets transitions from the state.
	 * @return A vector of transitions that start from the state.
	 */
	std::vector<Transition> getTransitions() const;

	/**
	 * @brief Clears all transitions from the state's transitions vector.
	 */
	void clearTransitions();
};
