#include "pch.h"  
#include "../AutomataSimulator/src/FiniteAutomata/DeterministicFiniteAutomaton/DeterministicFiniteAutomaton.h"  
#include "../AutomataSimulator/src/FiniteAutomata/FiniteAutomatonException.h"  

class DFATestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        automaton = new DeterministicFiniteAutomaton();

        automaton->addState("q0");
        automaton->addState("q1");
		automaton->setStartState("q0");
        automaton->setAlphabet({ "0", "1" });
    }

    void TearDown() override {
        delete automaton;
    }

    DeterministicFiniteAutomaton* automaton;
};

TEST_F(DFATestFixture, ShouldIncludeState) {
    EXPECT_EQ(automaton->getStates().size(), 2);
}

TEST_F(DFATestFixture, ShouldHaveStartState) {
    // Verify that the start state is set correctly
    ASSERT_NE(automaton->getStartState(), "");
    EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(DFATestFixture, ShouldUpdateStartState) {
    // Add a new state and update the start state
    automaton->setStartState("q1");
    EXPECT_EQ(automaton->getStartState(), "q1");
}

TEST_F(DFATestFixture, ShouldRejectInvalidStartState) {
    // Attempt to set an invalid start state (not in the automaton)
    EXPECT_THROW(automaton->setStartState("q2"), StateNotFoundException);
}

TEST_F(DFATestFixture, ShouldSimulateInput) {
    // Add transitions and simulate input
    automaton->addState("q2");
	automaton->addTransitionBetween("q0", "0", "q1");
	automaton->addTransitionBetween("q1", "1", "q2");
    automaton->addAcceptState("q2");


    EXPECT_TRUE(automaton->simulate({ "0", "1" })); // Accepts "01"
    EXPECT_FALSE(automaton->simulate({ "0", "0" })); // Rejects "00"
}   


TEST_F(DFATestFixture, ShouldSimulateFerrymanProblem) {
    // Wolf -> W, w
	// Goat -> G, g
	// Cabbage -> C, c
	// Ferryman -> M, m

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

	automaton->setAlphabet({ "g", "m", "w", "c" });

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

    // Valid sequence: take goat -> return alone -> take wolf -> return with goat -> take cabbage -> return alone -> take goat
    EXPECT_TRUE(automaton->simulate({ "g", "m", "w", "g", "c", "m", "g" }));

    // Invalid sequence: take wolf -> return alone -> take goat -> return with wolf -> take cabbage
    EXPECT_FALSE(automaton->simulate({ "w", "m", "g", "w", "c" }));

    // Invalid sequence: take cabbage -> return alone -> take goat -> return with cabbage -> take wolf
    EXPECT_FALSE(automaton->simulate({ "c", "m", "g", "c", "w" }));

    // Invalid sequence: take goat -> return alone -> take wolf -> return with goat -> take cabbage -> return alone -> take goat -> return with goat
    EXPECT_FALSE(automaton->simulate({ "g", "m", "w", "g", "c", "m", "g", "g"}));
}
