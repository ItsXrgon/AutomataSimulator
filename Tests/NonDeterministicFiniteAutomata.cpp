#include "pch.h"

class NFATestFixture : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new NonDeterministicFiniteAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->addState("q2");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1"});
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
	EXPECT_THROW(automaton->addTransition("q0", "0", "q3"), StateNotFoundException);
}

TEST_F(NFATestFixture, ShouldThrowExceptionForMissingFromStateInTransition) {
	EXPECT_THROW(automaton->addTransition("q3", "0", "q0"), StateNotFoundException);
}

TEST_F(NFATestFixture, ShouldThrowExceptionForTransitionOutsideAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "x", "q1"), InvalidTransitionException);
}

TEST_F(NFATestFixture, ShouldNotThrowExceptionForSettingEpsilonAsAlphabet) {
	EXPECT_NO_THROW(automaton->setInputAlphabet({""}), InvalidAlphabetException);
}

TEST(NFATest, ShouldThrowExceptionForSimulationWithoutStartState) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();
	automaton->setInputAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(NFATestFixture, ShouldCorrectlySimulateInputSequence) {
	automaton->addTransition("q0", "0", "q1");
	automaton->addTransition("q1", "1", "q2");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));  // Accepts "01"
	EXPECT_FALSE(automaton->simulate({"0", "0"})); // Rejects "00"
}

TEST_F(NFATestFixture, ShouldHandleMultipleTransitions) {
	automaton->addTransition("q0", "0", "q1");
	automaton->addTransition("q0", "0", "q2");
	automaton->addAcceptState("q1");

	EXPECT_TRUE(automaton->simulate({"0"})); // Accepts "0" (can be in q1 or q2)
}

TEST_F(NFATestFixture, ShouldHandleEpsilonTransitions) {
	automaton->addTransition("q0", "", "q1"); // Epsilon transition
	automaton->addTransition("q1", "1", "q2");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"1"})); // Accepts "1" (via epsilon transition)
}

TEST_F(NFATestFixture, ShouldProcessInputsForLanguageEndingWith01Or10) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();

	automaton->addState("q0");
	automaton->addState("q1");
	automaton->addState("q2");
	automaton->addState("q3"); 

	automaton->setStartState("q0");

	automaton->setInputAlphabet({"0", "1"});

	automaton->addTransition("q0", "0", "q0");
	automaton->addTransition("q0", "1", "q0");
	automaton->addTransition("q0", "1", "q1");

	automaton->addTransition("q1", "0", "q2");
	automaton->addTransition("q1", "", "q2");

	automaton->addTransition("q1", "1", "q3");

	automaton->addTransition("q3", "0", "q3");
	automaton->addTransition("q3", "1", "q3");

	automaton->addAcceptState("q3");

	automaton->setCurrentState("q0");
	EXPECT_EQ(automaton->getCurrentState(), "q0");

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

TEST_F(NFATestFixture, ShouldUpdatePossibleCurrentStatesForLanguageEndingWith01Or10) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();
	std::unordered_set<std::string> possibleCurrentStates;

	automaton->addState("q0");
	automaton->addState("q1");
	automaton->addState("q2");
	automaton->addState("q3");

	automaton->setStartState("q0");

	automaton->setInputAlphabet({"0", "1"});

	automaton->addTransition("q0", "0", "q0");
	automaton->addTransition("q0", "1", "q0");
	automaton->addTransition("q0", "1", "q1");

	automaton->addTransition("q1", "0", "q2");
	automaton->addTransition("q1", "", "q2");

	automaton->addTransition("q1", "1", "q3");

	automaton->addTransition("q3", "0", "q3");
	automaton->addTransition("q3", "1", "q3");

	automaton->addAcceptState("q3");
	automaton->setCurrentState("q0");

	automaton->processInput("1");
	possibleCurrentStates = automaton->getPossibleCurrentStates();
	EXPECT_TRUE(possibleCurrentStates.find("q0") != possibleCurrentStates.end());
	EXPECT_TRUE(possibleCurrentStates.find("q1") != possibleCurrentStates.end());

	automaton->setCurrentState("q1");
	automaton->processInput("1");
	possibleCurrentStates = automaton->getPossibleCurrentStates();
	EXPECT_FALSE(possibleCurrentStates.find("q0") != possibleCurrentStates.end());
	EXPECT_FALSE(possibleCurrentStates.find("q1") != possibleCurrentStates.end());
	EXPECT_FALSE(possibleCurrentStates.find("q2") != possibleCurrentStates.end());
	EXPECT_TRUE(possibleCurrentStates.find("q3") != possibleCurrentStates.end());
}