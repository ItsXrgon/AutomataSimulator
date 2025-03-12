#include "pch.h"

class DPDATestFixture : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new DeterministicPushdownAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->addState("q2");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1", ""}); // Epsilon allowed
		automaton->setStackAlphabet({"A", "Z"});     // Epsilon not allowed
	}

	void TearDown() override {
		delete automaton;
	}

	DeterministicPushdownAutomaton *automaton;
};

TEST_F(DPDATestFixture, ShouldHaveCorrectNumberOfStates) {
	EXPECT_EQ(automaton->getStates().size(), 3);
}

TEST_F(DPDATestFixture, ShouldHaveValidStartState) {
	ASSERT_NE(automaton->getStartState(), "");
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(DPDATestFixture, ShouldUpdateStartStateCorrectly) {
	automaton->setStartState("q1");
	EXPECT_EQ(automaton->getStartState(), "q1");
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForInvalidStartState) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForDuplicateState) {
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForMissingToStateInTransition) {
	EXPECT_THROW(automaton->addTransition("q0", "0", "q3", "Z", "AZ"), StateNotFoundException);
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForMissingFromStateInTransition) {
	EXPECT_THROW(automaton->addTransition("q3", "0", "q0", "Z", "AZ"), StateNotFoundException);
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForTransitionOutsideAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "x", "q1", "Z", "AZ"), InvalidTransitionException);
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForTransitionOutsideStackAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "0", "q1", "B", "AZ"), InvalidTransitionException);
}

TEST_F(DPDATestFixture, ShouldThrowExceptionForPushSymbolOutsideStackAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "0", "q1", "Z", "AB"), InvalidTransitionException);
}

TEST(DPDATest, ShouldThrowExceptionForSimulationWithoutStartState) {
	DeterministicPushdownAutomaton *automaton = new DeterministicPushdownAutomaton();
	automaton->setInputAlphabet({"0", "1"});
	automaton->setStackAlphabet({"A", "Z"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(DPDATestFixture, ShouldCorrectlySimulateInputSequence) {
	automaton->addTransition("q0", "0", "q1", "Z", "A");
	automaton->addTransition("q1", "1", "q2", "A", "");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));  // Accepts "01"
	EXPECT_FALSE(automaton->simulate({"0", "0"})); // Rejects "00"
}

TEST_F(DPDATestFixture, ShouldHandleEpsilonTransitions) {
	automaton->addTransition("q0", "", "q1", "Z", "A"); // Epsilon transition
	automaton->addTransition("q1", "1", "q2", "A", "");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"1"})); // Accepts "1" (via epsilon transition)
}

TEST_F(DPDATestFixture, ShouldHandleMultipleEpsilonTransitions) {
	automaton->addState("q3");
	automaton->addTransition("q0", "", "q1", "Z", "A"); // Epsilon transition
	automaton->addTransition("q1", "", "q2", "A", "");
	automaton->addTransition("q2", "1", "q3", "A", "");
	automaton->addAcceptState("q3");
	EXPECT_TRUE(automaton->simulate({"1"})); // Accepts "1" (via multiple epsilon transitions)
}

TEST_F(DPDATestFixture, ShouldProcessInputsForLanguageEndingWith01) {
	DeterministicPushdownAutomaton *automaton = new DeterministicPushdownAutomaton();
	// Add states
	automaton->addState("q0"); // Start state
	automaton->addState("q1");
	automaton->addState("q2"); // Accept state for strings ending with "01"

	// Set the start state
	automaton->setStartState("q0");

	// Set the input and stack alphabets
	automaton->setInputAlphabet({"0", "1", ""}); // Epsilon allowed
	automaton->setStackAlphabet({"A", "Z"});     // Epsilon not allowed

	// Add transitions
	automaton->addTransition("q0", "0", "q1", "Z", "AZ");
	automaton->addTransition("q1", "1", "q2", "A", "");

	// Mark accept states
	automaton->addAcceptState("q2");

	// Start at the initial state
	automaton->setCurrentState("q0");
	EXPECT_EQ(automaton->getCurrentState(), "q0");

	// Test input "01" (should end in q2, accepting "01")
	automaton->processInput("0");
	automaton->processInput("1");
	EXPECT_TRUE(automaton->getCurrentState() == "q2");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "00" (should not be accepted)
	automaton->processInput("0");
	automaton->processInput("0");
	EXPECT_FALSE(automaton->getCurrentState() == "q2");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "11" (should not be accepted)
	automaton->processInput("1");
	automaton->processInput("1");
	EXPECT_FALSE(automaton->getCurrentState() == "q2");

	delete automaton;
}

TEST_F(DPDATestFixture, ShouldProcessInputsForBalancedParentheses) {
	DeterministicPushdownAutomaton *automaton = new DeterministicPushdownAutomaton();
	// Add states
	automaton->addState("q0"); // Start state
	automaton->addState("q1"); // Accept state

	// Set the start state
	automaton->setStartState("q0");

	// Set the input and stack alphabets
	automaton->setInputAlphabet({"(", ")", ""}); // Epsilon allowed
	automaton->setStackAlphabet({"A", "Z"});     // Epsilon not allowed

	// Add transitions
	automaton->addTransition("q0", "(", "q0", "Z", "AZ"); // Push 'A' on '('
	automaton->addTransition("q0", "(", "q0", "A", "AA"); // Push 'A' on '('
	automaton->addTransition("q0", ")", "q0", "A", "");   // Pop 'A' on ')'
	automaton->addTransition("q0", "", "q1", "Z", "");    // Accept on empty stack

	// Mark accept states
	automaton->addAcceptState("q1");

	// Start at the initial state
	automaton->setCurrentState("q0");
	EXPECT_EQ(automaton->getCurrentState(), "q0");

	// Test input "()" (should be accepted)
	automaton->processInput("(");
	automaton->processInput(")");
	EXPECT_TRUE(automaton->getCurrentState() == "q1");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "(())" (should be accepted)
	automaton->processInput("(");
	automaton->processInput("(");
	automaton->processInput(")");
	automaton->processInput(")");
	EXPECT_TRUE(automaton->getCurrentState() == "q1");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "(()" (should not be accepted)
	automaton->processInput("(");
	automaton->processInput("(");
	automaton->processInput(")");
	EXPECT_FALSE(automaton->getCurrentState() == "q1");

	// Reset to initial state
	automaton->setCurrentState("q0");

	// Test input "())" (should not be accepted)
	automaton->processInput("(");
	automaton->processInput(")");
	automaton->processInput(")");
	EXPECT_FALSE(automaton->getCurrentState() == "q1");

	delete automaton;
}