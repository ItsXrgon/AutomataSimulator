#include "pch.h"

class NFATestFixture : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new NonDeterministicFiniteAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->addState("q2");
		automaton->setStartState("q0");
		automaton->setAlphabet({"0", "1"});
	}

	void TearDown() override {
		delete automaton;
	}

	NonDeterministicFiniteAutomaton *automaton;
};

TEST_F(NFATestFixture, ShouldHaveCorrectNumberOfStates) {
	EXPECT_EQ(automaton->getStates().size(), 3);
}

TEST_F(NFATestFixture, ShouldHaveValidStartState) {
	ASSERT_NE(automaton->getStartState(), "");
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(NFATestFixture, ShouldUpdateStartStateCorrectly) {
	automaton->setStartState("q1");
	EXPECT_EQ(automaton->getStartState(), "q1");
}

TEST_F(NFATestFixture, ShouldThrowExceptionForInvalidStartState) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(NFATestFixture, ShouldThrowExceptionForDuplicateState) {
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(NFATestFixture, ShouldThrowExceptionForMissingToStateInTransition) {
	EXPECT_THROW(automaton->addTransitionBetween("q0", "0", "q3"), StateNotFoundException);
}

TEST_F(NFATestFixture, ShouldThrowExceptionForMissingFromStateInTransition) {
	EXPECT_THROW(automaton->addTransitionBetween("q3", "0", "q0"), StateNotFoundException);
}

TEST_F(NFATestFixture, ShouldThrowExceptionForTransitionOutsideAlphabet) {
	EXPECT_THROW(automaton->addTransitionBetween("q0", "x", "q1"), InvalidTransitionException);
}

TEST_F(NFATestFixture, ShouldNotThrowExceptionForSettingEpsilonAsAlphabet) {
	EXPECT_NO_THROW(automaton->setAlphabet({""}), InvalidAlphabetException);
}

TEST(NFATest, ShouldThrowExceptionForSimulationWithoutStartState) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();
	automaton->setAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST(NFATest, ShouldThrowExceptionForSimulationWithoutAlphabet) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();
	automaton->addState("q0");
	automaton->setStartState("q0");

	EXPECT_THROW(automaton->simulate({""}), InvalidAlphabetException);
}

TEST_F(NFATestFixture, ShouldCorrectlySimulateInputSequence) {
	automaton->addTransitionBetween("q0", "0", "q1");
	automaton->addTransitionBetween("q1", "1", "q2");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));  // Accepts "01"
	EXPECT_FALSE(automaton->simulate({"0", "0"})); // Rejects "00"
}

TEST_F(NFATestFixture, ShouldHandleMultipleTransitions) {
	automaton->addTransitionBetween("q0", "0", "q1");
	automaton->addTransitionBetween("q0", "0", "q2");
	automaton->addAcceptState("q1");

	EXPECT_TRUE(automaton->simulate({"0"})); // Accepts "0" (can be in q1 or q2)
}

TEST_F(NFATestFixture, ShouldHandleEpsilonTransitions) {
	automaton->addTransitionBetween("q0", "", "q1"); // Epsilon transition
	automaton->addTransitionBetween("q1", "1", "q2");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"1"})); // Accepts "1" (via epsilon transition)
}

TEST_F(NFATestFixture, ShouldProcessInputsForLanguageEndingWith01Or10) {
	// Add states
	automaton->addState("q0"); // Start state
	automaton->addState("q1");
	automaton->addState("q2"); // Accept state for strings ending with "01"
	automaton->addState("q3"); // Accept state for strings ending with "10"

	// Set the start state
	automaton->setStartState("q0");

	// Set the alphabet
	automaton->setAlphabet({"0", "1"});

	// Add transitions
	// From q0, on "0", can go to q0 or q1 (non-deterministic)
	automaton->addTransitionBetween("q0", "0", "q0");
	automaton->addTransitionBetween("q0", "0", "q1");

	// From q0, on "1", can go to q0 or q3 (non-deterministic)
	automaton->addTransitionBetween("q0", "1", "q0");
	automaton->addTransitionBetween("q0", "1", "q3");

	// From q1, on "1", go to q2 (accept state for "01")
	automaton->addTransitionBetween("q1", "1", "q2");

	// From q3, on "0", go to q4 (accept state for "10")
	automaton->addTransitionBetween("q3", "0", "q4");

	// Mark accept states
	automaton->addAcceptState("q2");
	automaton->addAcceptState("q4");

	// Start at the initial state
	automaton->setCurrentState("q0");
	EXPECT_EQ(automaton->getCurrentState(), "q0");

	// Test input "010" (should end in q2, accepting "01")
	automaton->processInput("0");
	automaton->processInput("1");
	automaton->processInput("0");
	EXPECT_TRUE(automaton->getCurrentState() == "q2" || automaton->getCurrentState() == "q4");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "101" (should end in q4, accepting "10")
	automaton->processInput("1");
	automaton->processInput("0");
	automaton->processInput("1");
	EXPECT_TRUE(automaton->getCurrentState() == "q2" || automaton->getCurrentState() == "q4");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "000" (should not be accepted)
	automaton->processInput("0");
	automaton->processInput("0");
	automaton->processInput("0");
	EXPECT_FALSE(automaton->getCurrentState() == "q2" || automaton->getCurrentState() == "q4");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "111" (should not be accepted)
	automaton->processInput("1");
	automaton->processInput("1");
	automaton->processInput("1");
	EXPECT_FALSE(automaton->getCurrentState() == "q2" || automaton->getCurrentState() == "q4");
}