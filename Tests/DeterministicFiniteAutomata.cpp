#include "pch.h"

class DFATestFixture : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new DeterministicFiniteAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->setStartState("q0");
		automaton->setAlphabet({"0", "1"});
	}

	void TearDown() override {
		delete automaton;
	}

	DeterministicFiniteAutomaton *automaton;
};

TEST_F(DFATestFixture, ShouldHaveCorrectNumberOfStates) {
	EXPECT_EQ(automaton->getStates().size(), 2);
}

TEST_F(DFATestFixture, ShouldHaveValidStartState) {
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(DFATestFixture, ShouldUpdateStartStateCorrectly) {
	automaton->setStartState("q1");
	EXPECT_EQ(automaton->getStartState(), "q1");
}

TEST_F(DFATestFixture, ShouldThrowExceptionForInvalidStartState) {
	EXPECT_THROW(automaton->setStartState("q2"), StateNotFoundException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForDuplicateState) {
	EXPECT_THROW(automaton->addState("q1");, InvalidAutomatonDefinitionException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForMissingToStateInTransition) {
	EXPECT_THROW(automaton->addTransitionBetween("q0", "0", "q2"), StateNotFoundException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForMissingFromStateInTransition) {
	EXPECT_THROW(automaton->addTransitionBetween("q2", "0", "q0"), StateNotFoundException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForNonDeterministicTransition) {
	automaton->addState("q2");
	automaton->addTransitionBetween("q0", "0", "q1");
	EXPECT_THROW(automaton->addTransitionBetween("q0", "0", "q2"), InvalidTransitionException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForTransitionOutsideAlphabet) {
	EXPECT_THROW(automaton->addTransitionBetween("q0", "x", "q1"), InvalidTransitionException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForAddingEpsilonToAlphabet) {
	EXPECT_THROW(automaton->addAlphabet({""}), InvalidAlphabetException);
}

TEST_F(DFATestFixture, ShouldThrowExceptionForSettingEpsilonAsAlphabet) {
	EXPECT_THROW(automaton->setAlphabet({""}), InvalidAlphabetException);
}

TEST(DFATest, ShouldThrowExceptionForSimulationWithoutStartState) {
	DeterministicFiniteAutomaton *automaton = new DeterministicFiniteAutomaton();
	automaton->setAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST(DFATest, ShouldThrowExceptionForSimulationWithoutAlphabet) {
	DeterministicFiniteAutomaton *automaton = new DeterministicFiniteAutomaton();
	automaton->addState("q0");
	automaton->setStartState("q0");

	EXPECT_THROW(automaton->simulate({""}), InvalidAlphabetException);
}

TEST_F(DFATestFixture, ShouldCorrectlySimulateInputSequence) {
	automaton->addState("q2");
	automaton->addTransitionBetween("q0", "0", "q1");
	automaton->addTransitionBetween("q1", "1", "q2");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));  // Accepts "01"
	EXPECT_FALSE(automaton->simulate({"0", "0"})); // Rejects "00"
}

TEST_F(DFATestFixture, ShouldSimulateFerrymanProblem) {
	automaton->addState("MWGC-0");
	automaton->addState("WC-MG");
	automaton->addState("MWC-G");
	automaton->addState("C-MWG");
	automaton->addState("W-MGC");
	automaton->addState("MGC-W");
	automaton->addState("MWG-C");
	automaton->addState("G-MWC");
	automaton->addState("MG-WC");
	automaton->addState("0-MWGC");

	automaton->setStartState("MWGC-0");

	automaton->setAlphabet({"g", "m", "w", "c"});

	automaton->addTransitionBetween("MWGC-0", "g", "WC-MG");
	automaton->addTransitionBetween("WC-MG", "g", "MWGC-0");

	automaton->addTransitionBetween("WC-MG", "m", "MWC-G");
	automaton->addTransitionBetween("MWC-G", "m", "WC-MG");

	automaton->addTransitionBetween("MWC-G", "w", "C-MWG");
	automaton->addTransitionBetween("MWC-G", "c", "W-MGC");

	automaton->addTransitionBetween("C-MWG", "w", "MWC-G");
	automaton->addTransitionBetween("C-MWG", "g", "MGC-W");

	automaton->addTransitionBetween("W-MGC", "c", "MWC-G");
	automaton->addTransitionBetween("W-MGC", "g", "MWG-C");

	automaton->addTransitionBetween("MGC-W", "g", "C-MWG");
	automaton->addTransitionBetween("MGC-W", "c", "G-MWC");

	automaton->addTransitionBetween("MWG-C", "g", "W-MGC");
	automaton->addTransitionBetween("MWG-C", "w", "G-MWC");

	automaton->addTransitionBetween("G-MWC", "c", "MGC-W");
	automaton->addTransitionBetween("G-MWC", "w", "MWG-C");
	automaton->addTransitionBetween("G-MWC", "m", "MG-WC");

	automaton->addTransitionBetween("MG-WC", "m", "G-MWC");
	automaton->addTransitionBetween("MG-WC", "g", "0-MWGC");

	automaton->addTransitionBetween("0-MWGC", "g", "MG-WC");

	automaton->addAcceptState("0-MWGC");

	// Valid sequence: take goat -> return alone -> take wolf -> return with goat -> take cabbage -> return alone ->
	EXPECT_TRUE(automaton->simulate({"g", "m", "w", "g", "c", "m", "g"}));

	// Invalid sequence: take wolf -> return alone -> take goat -> return with wolf -> take cabbage
	EXPECT_FALSE(automaton->simulate({"w", "m", "g", "w", "c"}));

	// Invalid sequence: take cabbage -> return alone -> take goat -> return with cabbage -> take wolf
	EXPECT_FALSE(automaton->simulate({"c", "m", "g", "c", "w"}));

	// Invalid sequence: take goat -> return alone -> take wolf -> return with goat -> take cabbage -> return alone ->
	EXPECT_FALSE(automaton->simulate({"g", "m", "w", "g", "c", "m", "g", "g"}));
}

TEST_F(DFATestFixture, ShouldProcessInputsForFerrymanProblem) {
	automaton->addState("MWGC-0");
	automaton->addState("WC-MG");
	automaton->addState("MWC-G");
	automaton->addState("C-MWG");
	automaton->addState("W-MGC");
	automaton->addState("MGC-W");
	automaton->addState("MWG-C");
	automaton->addState("G-MWC");
	automaton->addState("MG-WC");
	automaton->addState("0-MWGC");

	automaton->setStartState("MWGC-0");

	automaton->setAlphabet({"g", "m", "w", "c"});

	automaton->addTransitionBetween("MWGC-0", "g", "WC-MG");
	automaton->addTransitionBetween("WC-MG", "g", "MWGC-0");

	automaton->addTransitionBetween("WC-MG", "m", "MWC-G");
	automaton->addTransitionBetween("MWC-G", "m", "WC-MG");

	automaton->addTransitionBetween("MWC-G", "w", "C-MWG");
	automaton->addTransitionBetween("MWC-G", "c", "W-MGC");

	automaton->addTransitionBetween("C-MWG", "w", "MWC-G");
	automaton->addTransitionBetween("C-MWG", "g", "MGC-W");

	automaton->addTransitionBetween("W-MGC", "c", "MWC-G");
	automaton->addTransitionBetween("W-MGC", "g", "MWG-C");

	automaton->addTransitionBetween("MGC-W", "g", "C-MWG");
	automaton->addTransitionBetween("MGC-W", "c", "G-MWC");

	automaton->addTransitionBetween("MWG-C", "g", "W-MGC");
	automaton->addTransitionBetween("MWG-C", "w", "G-MWC");

	automaton->addTransitionBetween("G-MWC", "c", "MGC-W");
	automaton->addTransitionBetween("G-MWC", "w", "MWG-C");
	automaton->addTransitionBetween("G-MWC", "m", "MG-WC");

	automaton->addTransitionBetween("MG-WC", "m", "G-MWC");
	automaton->addTransitionBetween("MG-WC", "g", "0-MWGC");

	automaton->addTransitionBetween("0-MWGC", "g", "MG-WC");

	automaton->addAcceptState("0-MWGC");

	// Start at the initial state
	automaton->setCurrentState("MWGC-0");
	EXPECT_EQ(automaton->getCurrentState(), "MWGC-0");

	// Take the goat to the other side
	automaton->processInput("g");
	EXPECT_EQ(automaton->getCurrentState(), "WC-MG");

	// Return alone
	automaton->processInput("m");
	EXPECT_EQ(automaton->getCurrentState(), "MWC-G");

	// Take the wolf to the other side
	automaton->processInput("w");
	EXPECT_EQ(automaton->getCurrentState(), "C-MWG");

	// Return with the goat
	automaton->processInput("g");
	EXPECT_EQ(automaton->getCurrentState(), "MGC-W");

	// Take the cabbage to the other side
	automaton->processInput("c");
	EXPECT_EQ(automaton->getCurrentState(), "G-MWC");

	// Return alone
	automaton->processInput("m");
	EXPECT_EQ(automaton->getCurrentState(), "MG-WC");

	// Take the goat to the other side
	EXPECT_TRUE(automaton->processInput("g"));
	EXPECT_EQ(automaton->getCurrentState(), "0-MWGC");
}