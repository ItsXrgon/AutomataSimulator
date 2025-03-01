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