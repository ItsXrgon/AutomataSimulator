#include "pch.h"

class NPDATestFixture : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new NonDeterministicPushdownAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->addState("q2");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1", ""});  // Epsilon allowed
		automaton->setStackAlphabet({"A", "Z"}); // Epsilon not allowed
	}

	void TearDown() override {
		delete automaton;
	}

	NonDeterministicPushdownAutomaton *automaton;
};

TEST_F(NPDATestFixture, ShouldHaveCorrectNumberOfStates) {
	EXPECT_EQ(automaton->getStates().size(), 3);
}

TEST_F(NPDATestFixture, ShouldHaveValidStartState) {
	ASSERT_NE(automaton->getStartState(), "");
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(NPDATestFixture, ShouldUpdateStartStateCorrectly) {
	automaton->setStartState("q1");
	EXPECT_EQ(automaton->getStartState(), "q1");
}

TEST_F(NPDATestFixture, ShouldThrowExceptionForInvalidStartState) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(NPDATestFixture, ShouldThrowExceptionForDuplicateState) {
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(NPDATestFixture, ShouldThrowExceptionForMissingToStateInTransition) {
	EXPECT_THROW(automaton->addTransition("q0", "0", "q3", "Z", "AZ"), StateNotFoundException);
}

TEST_F(NPDATestFixture, ShouldThrowExceptionForMissingFromStateInTransition) {
	EXPECT_THROW(automaton->addTransition("q3", "0", "q0", "Z", "AZ"), StateNotFoundException);
}

TEST_F(NPDATestFixture, ShouldThrowExceptionForTransitionOutsideAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "x", "q1", "Z", "AZ"), InvalidTransitionException);
}

TEST_F(NPDATestFixture, ShouldThrowExceptionForPushSymbolOutsideStackAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "0", "q1", "Z", "AB"), InvalidTransitionException);
}

TEST(NPDATest, ShouldThrowExceptionForSimulationWithoutStartState) {
	NonDeterministicPushdownAutomaton *automaton = new NonDeterministicPushdownAutomaton();
	automaton->setInputAlphabet({"0", "1"});
	automaton->setStackAlphabet({"A", "Z"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(NPDATestFixture, ShouldCorrectlySimulateInputSequence) {
	automaton->addTransition("q0", "0", "q1", "Z", "AZ");
	automaton->addTransition("q1", "1", "q2", "A", "");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));  // Accepts "01"
	EXPECT_FALSE(automaton->simulate({"0", "0"})); // Rejects "00"
}

TEST_F(NPDATestFixture, ShouldHandleMultipleTransitions) {
	automaton->addTransition("q0", "0", "q1", "Z", "AZ");
	automaton->addTransition("q0", "0", "q2", "Z", "BZ");
	automaton->addAcceptState("q1");

	EXPECT_TRUE(automaton->simulate({"0"})); // Accepts "0" (can be in q1 or q2)
}

TEST_F(NPDATestFixture, ShouldHandleEpsilonTransitions) {
	automaton->addTransition("q0", "", "q1", "Z", "AZ"); // Epsilon transition
	automaton->addTransition("q1", "1", "q2", "A", "");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"1"})); // Accepts "1" (via epsilon transition)
}

TEST_F(NPDATestFixture, ShouldProcessInputsForLanguageEndingWith01Or10) {
	// Add states
	automaton->addState("q0"); // Start state
	automaton->addState("q1");
	automaton->addState("q2"); // Accept state for strings ending with "01"
	automaton->addState("q3"); // Accept state for strings ending with "10"

	// Set the start state
	automaton->setStartState("q0");

	// Set the input and stack alphabets
	automaton->setInputAlphabet({"0", "1", ""}); // Epsilon allowed
	automaton->setStackAlphabet({"A", "Z"});     // Epsilon not allowed

	// Add transitions
	// From q0, on "0", can go to q0 or q1 (non-deterministic)
	automaton->addTransition("q0", "0", "q0", "Z", "AZ");
	automaton->addTransition("q0", "0", "q1", "Z", "AZ");

	// From q0, on "1", can go to q0 or q3 (non-deterministic)
	automaton->addTransition("q0", "1", "q0", "Z", "AZ");
	automaton->addTransition("q0", "1", "q3", "Z", "AZ");

	// From q1, on "1", go to q2 (accept state for "01")
	automaton->addTransition("q1", "1", "q2", "A", "");

	// From q3, on "0", go to q4 (accept state for "10")
	automaton->addTransition("q3", "0", "q4", "A", "");

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