#include "pch.h"

class DPDA_Test : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new DeterministicPushdownAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1"});
		automaton->setStackAlphabet({"A", "Z"});
	}

	void TearDown() override {
		delete automaton;
	}

	DeterministicPushdownAutomaton *automaton;
};

TEST_F(DPDA_Test, StateExists_ReturnsTrueWhenStateExists) {
	EXPECT_TRUE(automaton->stateExists("q0"));
	EXPECT_TRUE(automaton->stateExists("q1"));
}

TEST_F(DPDA_Test, StateExists_ReturnsFalseWhenStateDoesNotExist) {
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_FALSE(automaton->stateExists(""));
}

TEST_F(DPDA_Test, InputAlphabetSymbolExists_ReturnsTrueForValidSymbol) {
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("0"));
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("1"));
}

TEST_F(DPDA_Test, InputAlphabetSymbolExists_ReturnsFalseForInvalidSymbol) {
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("2"));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists(""));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("abc"));
}

TEST_F(DPDA_Test, AddState_AddsNewStateSuccessfully) {
	automaton->addState("q2");
	EXPECT_TRUE(automaton->stateExists("q2"));
}

TEST_F(DPDA_Test, AddState_ThrowsForDuplicateState) {
	EXPECT_THROW(automaton->addState("q0"), InvalidAutomatonDefinitionException);
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, AddState_AddsMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));
}

TEST_F(DPDA_Test, AddState_AddsStateAsAccept) {
	automaton->addState("q2", true);
	EXPECT_TRUE(automaton->getState("q2").getIsAccept());
}

TEST_F(DPDA_Test, UpdateStateLabel_UpdatesStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->stateExists("q3"));
	automaton->updateStateLabel("q3", "q4");
	EXPECT_TRUE(automaton->stateExists("q4"));
}

TEST_F(DPDA_Test, UpdateStateLabel_UpdatesStartStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->getStartState() == "q3");
}

TEST_F(DPDA_Test, GetCurrentState_GetsStartStateIfNoCurrentStateSet) {
	EXPECT_TRUE(automaton->getCurrentState() == "q0");
}

TEST_F(DPDA_Test, GetCurrentState_ThrowsIfNoStartStateOrCurrentStateSet) {
	DeterministicPushdownAutomaton *automaton = new DeterministicPushdownAutomaton();
	automaton->addState("q0");

	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);

	delete automaton;
}

TEST_F(DPDA_Test, SetCurrentState_UpdatesCurrentState) {
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->getCurrentState() == "q1");
}

TEST_F(DPDA_Test, SetCurrentState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setCurrentState("q3"), StateNotFoundException);
}

TEST_F(DPDA_Test, GetState_GetsState) {
	PDAState state = automaton->getState("q0");
	EXPECT_TRUE(state.getLabel() == "q0");
}

TEST_F(DPDA_Test, GetState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->getState("q3"), StateNotFoundException);
}

TEST_F(DPDA_Test, GetStates_GetsStates) {
	std::vector<PDAState> states = automaton->getStates();
	EXPECT_TRUE(states.size() == 2);

	bool foundQ0 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q0") {
			foundQ0 = true;
		}
	}
	EXPECT_TRUE(foundQ0);
}

TEST_F(DPDA_Test, GetStates_GetsStatesAfterAddStateCall) {
	automaton->addState("q3");
	std::vector<PDAState> states = automaton->getStates();

	bool foundQ3 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q3") {
			foundQ3 = true;
		}
	}
	EXPECT_TRUE(foundQ3);
}

TEST_F(DPDA_Test, GetStates_GetsStatesAfterUpdateStateLabelCall) {
	automaton->updateStateLabel("q0", "q3");
	std::vector<PDAState> states = automaton->getStates();

	bool foundQ3 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q3") {
			foundQ3 = true;
		}
	}
	EXPECT_TRUE(foundQ3);
}

TEST_F(DPDA_Test, RemoveState_RemovesState) {
	EXPECT_TRUE(automaton->getStates().size() == 2);

	automaton->removeState("q1");
	EXPECT_FALSE(automaton->stateExists("q1"));

	EXPECT_TRUE(automaton->getStates().size() == 1);
}

TEST_F(DPDA_Test, RemoveState_RemovesStartState) {
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DPDA_Test, RemoveState_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, RemoveState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeState("q3"), StateNotFoundException);
}

TEST_F(DPDA_Test, RemoveState_RemovesTransitionsWithStateAsToIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	automaton->removeState("q0", false);
	EXPECT_FALSE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "0", "A", "A")));
}

TEST_F(DPDA_Test, RemoveState_ThrowsIfStateIsUsedInTransitionsIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	EXPECT_THROW(automaton->removeState("q0", true), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, RemoveStates_RemovesMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->getStates().size() == 4);

	automaton->removeStates({"q1", "q2"});
	EXPECT_FALSE(automaton->stateExists("q1"));
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));

	EXPECT_TRUE(automaton->getStates().size() == 2);
}

TEST_F(DPDA_Test, RemoveStates_RemovesStartState) {
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DPDA_Test, RemoveStates_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, RemoveStates_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(DPDA_Test, RemoveStates_RemovesTransitionsWithStateAsToIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	automaton->removeStates({"q0"}, false);
	EXPECT_FALSE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "0", "A", "A")));
}

TEST_F(DPDA_Test, RemoveStates_ThrowsIfStateIsUsedInTransitionsIfStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0", "A", "A");
	EXPECT_THROW(automaton->removeStates({"q0"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, ClearStates_RemovesAllStates) {
	EXPECT_TRUE(automaton->getStates().size() == 2);
	automaton->clearStates();
	EXPECT_TRUE(automaton->getStates().empty());
}

TEST_F(DPDA_Test, ClearStates_RemovesCurrentState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, ClearStates_RemovesStartState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DPDA_Test, GetInputAlphabet_GetsAlphabet) {
	std::vector<std::string> alphabet = automaton->getInputAlphabet();
	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DPDA_Test, SetInputAlphabet_OverridesAlphabet) {
	automaton->setInputAlphabet({"a", "b", "c"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 3);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "a") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "b") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "c") != alphabet.end());
}

TEST_F(DPDA_Test, SetInputAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->setInputAlphabet({"a", "", "c"}));
}

TEST_F(DPDA_Test, SetInputAlphabet_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	EXPECT_THROW(automaton->setInputAlphabet({"a", "b"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, SetInputAlphabet_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	automaton->setInputAlphabet({"a", "b"}, false);
	EXPECT_FALSE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "0", "A", "A")));
}

TEST_F(DPDA_Test, AddInputAlphabet_AddsNewSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 4);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
}

TEST_F(DPDA_Test, AddInputAlphabet_ThrowsForEpsilon) {
	EXPECT_NO_THROW(automaton->addInputAlphabet({"a", "", "c"}));
}

TEST_F(DPDA_Test, AddInputAlphabet_IgnoresDuplicates) {
	automaton->addInputAlphabet({"1", "0", "1"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbol_RemovesSymbol) {
	automaton->removeInputAlphabetSymbol("0");
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 1);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbol_ThrowsIfNotFound) {
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("x"), InputAlphabetSymbolNotFoundException);
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbol_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("0", true), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbol_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	automaton->removeInputAlphabetSymbol("0", false);
	EXPECT_FALSE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "0", "A", "A")));
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbols_RemovesMultipleSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	automaton->removeInputAlphabetSymbols({"0", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbols_ThrowsIfAnySymbolNotFound) {
	automaton->addInputAlphabet({"2"});
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"1", "x"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbols_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"0"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, RemoveInputAlphabetSymbols_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	automaton->removeInputAlphabetSymbols({"0"}, false);
	EXPECT_FALSE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "0", "A", "A")));
}

TEST_F(DPDA_Test, ClearInputAlphabet_RemovesAllSymbols) {
	automaton->clearInputAlphabet();
	EXPECT_TRUE(automaton->getInputAlphabet().empty());
}

TEST_F(DPDA_Test, ClearInputAlphabet_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	EXPECT_THROW(automaton->clearInputAlphabet(true), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, ClearInputAlphabet_RemovesNonEpsilonTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", "A");
	automaton->addTransition("q1", "q0", "", "Z", "A");
	automaton->clearInputAlphabet(false);
	EXPECT_FALSE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "0", "A", "A")));
	EXPECT_TRUE(
	    automaton->getState("q1").transitionExists(PDATransition::generateTransitionKey("q1", "q0", "", "Z", "A")));
}

TEST_F(DPDA_Test, GetStackAlphabet_GetsAlphabet) {
	std::vector<std::string> alphabet = automaton->getStackAlphabet();
	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
}

TEST_F(DPDA_Test, SetStackAlphabet_OverridesAlphabet) {
	automaton->setStackAlphabet({"A", "B", "C"});
	std::vector<std::string> alphabet = automaton->getStackAlphabet();

	EXPECT_EQ(alphabet.size(), 3);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "B") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
}

TEST_F(DPDA_Test, SetStackAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->setStackAlphabet({"A", "", "C"}));
}

TEST_F(DPDA_Test, AddStackAlphabet_AddsNewSymbols) {
	automaton->addStackAlphabet({"B", "C"});
	std::vector<std::string> alphabet = automaton->getStackAlphabet();

	EXPECT_EQ(alphabet.size(), 4);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "B") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
}

TEST_F(DPDA_Test, AddStackAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->addStackAlphabet({"A", "", "C"}));
}

TEST_F(DPDA_Test, AddStackAlphabet_IgnoresDuplicates) {
	automaton->addStackAlphabet({"Z", "A", "Z"});
	std::vector<std::string> alphabet = automaton->getStackAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
}

TEST_F(DPDA_Test, RemoveStackAlphabetSymbol_RemovesSymbol) {
	automaton->removeStackAlphabetSymbol("A");
	std::vector<std::string> alphabet = automaton->getStackAlphabet();

	EXPECT_EQ(alphabet.size(), 1);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
}

TEST_F(DPDA_Test, RemoveStackAlphabetSymbol_ThrowsIfNotFound) {
	EXPECT_THROW(automaton->removeStackAlphabetSymbol("X"), StackAlphabetSymbolNotFoundException);
}

TEST_F(DPDA_Test, RemoveStackAlphabetSymbols_RemovesMultipleSymbols) {
	automaton->addStackAlphabet({"B", "C"});
	automaton->removeStackAlphabetSymbols({"Z", "C"});
	std::vector<std::string> alphabet = automaton->getStackAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "B") != alphabet.end());
}

TEST_F(DPDA_Test, RemoveStackAlphabetSymbols_ThrowsIfAnySymbolNotFound) {
	automaton->addStackAlphabet({"B"});
	EXPECT_THROW(automaton->removeStackAlphabetSymbols({"B", "X"}), StackAlphabetSymbolNotFoundException);
}

TEST_F(DPDA_Test, ClearStackAlphabet_RemovesAllSymbols) {
	automaton->clearStackAlphabet();
	EXPECT_TRUE(automaton->getStackAlphabet().empty());
}

TEST_F(DPDA_Test, GetStartState_ReturnsCorrectState) {
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(DPDA_Test, GetStartState_ThrowsIfNotSet) {
	automaton->removeState("q0");
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DPDA_Test, SetStartState_SetsNewStartState) {
	automaton->addState("q2");
	automaton->setStartState("q2");
	EXPECT_EQ(automaton->getStartState(), "q2");
}

TEST_F(DPDA_Test, SetStartState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(DPDA_Test, SetStartState_SetsCurrentStateIfEmpty) {
	automaton->clearStates();
	automaton->addState("q4");
	automaton->setStartState("q4");
	EXPECT_EQ(automaton->getStartState(), "q4");
	EXPECT_EQ(automaton->getCurrentState(), "q4");
}

TEST_F(DPDA_Test, SetStartState_DoesNotOverrideCurrentStateIfSet) {
	automaton->addState("q2");
	automaton->setCurrentState("q1");
	automaton->setStartState("q2");

	EXPECT_EQ(automaton->getStartState(), "q2");
	EXPECT_EQ(automaton->getCurrentState(), "q1");
}

TEST_F(DPDA_Test, AddTransition_AddsValidTransition) {
	automaton->addTransition("q0", "q1", "0", "A", "A");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool found = false;

	for (const auto &transition : transitions) {
		if (transition.getInput() == "0" && transition.getToStateKey() == "q1") {
			found = true;
			break;
		}
	}

	EXPECT_TRUE(found);
}

TEST_F(DPDA_Test, AddTransition_ThrowsIfFromStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("qX", "q1", "0", "A", "A"), StateNotFoundException);
}

TEST_F(DPDA_Test, AddTransition_ThrowsIfToStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("q0", "qX", "0", "A", "A"), StateNotFoundException);
}

TEST_F(DPDA_Test, AddTransition_ThrowsIfInputNotInAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "q1", "2", "A", "A"), InvalidTransitionException);
}

TEST_F(DPDA_Test, AddTransition_NoThrowIfEpsilonInDPDA) {
	EXPECT_NO_THROW(automaton->addTransition("q0", "q1", "", "A", "A"));
}

TEST_F(DPDA_Test, AddTransition_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	EXPECT_THROW(automaton->addTransition("q0", "q2", "0", "A", "A"), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, AddTransition_AllowsDifferentInputs) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q0", "1", "A", "A");

	const auto &transitions = automaton->getState("q0").getTransitions();

	bool found0 = false, found1 = false;
	for (const auto &transition : transitions) {
		if (transition.getInput() == "0" && transition.getToStateKey() == "q1") {
			found0 = true;
		}
		if (transition.getInput() == "1" && transition.getToStateKey() == "q0") {
			found1 = true;
		}
	}

	EXPECT_TRUE(found0);
	EXPECT_TRUE(found1);
}

TEST_F(DPDA_Test, AddTransition_DoesNotInvalidateExistingTransitions) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q1", "q0", "1", "A", "A");

	const auto &q0Transitions = automaton->getState("q0").getTransitions();
	const auto &q1Transitions = automaton->getState("q1").getTransitions();

	EXPECT_EQ(q0Transitions.size(), 1);
	EXPECT_EQ(q1Transitions.size(), 1);

	EXPECT_EQ(q0Transitions[0].getInput(), "0");
	EXPECT_EQ(q1Transitions[0].getInput(), "1");
}

TEST_F(DPDA_Test, UpdateTransitionInput_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	automaton->updateTransitionInput(transitionKey, "1");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getInput() == "1" && transition.getToStateKey() == "q1") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
}

TEST_F(DPDA_Test, UpdateTransitionInput_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");
	automaton->updateTransitionInput(transitionKey, "2");

	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q2", "1", "A", "A")));
	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q1", "2", "A", "A")));
}

TEST_F(DPDA_Test, UpdateTransitionInput_ThrowsIfInputNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionInput_ThrowsIfTransitionExists) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q1", "1", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "1"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionInput_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionInput("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(DPDA_Test, UpdateTransitionInput_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q2", "1", "A", "A");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "0"), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, UpdateTransitionFromState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	automaton->updateTransitionFromState(transitionKey, "q2");

	EXPECT_TRUE(
	    automaton->getState("q2").transitionExists(PDATransition::generateTransitionKey("q2", "q1", "0", "A", "A")));
	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DPDA_Test, UpdateTransitionFromState_ThrowsIfDuplicate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q2", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q2"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionFromState_ThrowsIfTransitionNotFound) {
	automaton->addState("q2");
	EXPECT_THROW(automaton->updateTransitionFromState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(DPDA_Test, UpdateTransitionFromState_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q1", "q2", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q1", "q2", "0", "A", "A");
	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q0"), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, UpdateTransitionToState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	automaton->updateTransitionToState(transitionKey, "q2");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getInput() == "0" && transition.getToStateKey() == "q2") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DPDA_Test, UpdateTransitionToState_ThrowsIfStateNotFound) {
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionToState(transitionKey, "qX"), StateNotFoundException);
}

TEST_F(DPDA_Test, UpdateTransitionToState_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionToState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(DPDA_Test, UpdateTransitionToState_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q1", "1", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");
	automaton->updateTransitionToState(transitionKey, "q2");

	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q1", "1", "A", "A")));
}

TEST_F(DPDA_Test, UpdateTransitionStackSymbol_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	automaton->updateTransitionStackSymbol(transitionKey, "Z");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getStackSymbol() == "Z" && transition.getToStateKey() == "q1") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
}

TEST_F(DPDA_Test, UpdateTransitionStackSymbol_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");
	automaton->updateTransitionStackSymbol(transitionKey, "Z");

	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q2", "1", "A", "A")));
	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q1", "0", "Z", "A")));
}

TEST_F(DPDA_Test, UpdateTransitionStackSymbol_ThrowsIfStackSymbolNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionStackSymbol(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionStackSymbol_ThrowsIfTransitionExists) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q1", "1", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionStackSymbol(transitionKey, "1"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionStackSymbol_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionStackSymbol("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(DPDA_Test, UpdateTransitionStackSymbol_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "0", "Z", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q2", "0", "Z", "A");

	EXPECT_THROW(automaton->updateTransitionStackSymbol(transitionKey, "A"), InvalidAutomatonDefinitionException);
}

TEST_F(DPDA_Test, UpdateTransitionPushSymbol_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	automaton->updateTransitionPushSymbol(transitionKey, "Z");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getPushSymbol() == "Z" && transition.getToStateKey() == "q1") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
}

TEST_F(DPDA_Test, UpdateTransitionPushSymbol_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");
	automaton->updateTransitionPushSymbol(transitionKey, "Z");

	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q2", "1", "A", "A")));
	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q1", "0", "A", "Z")));
}

TEST_F(DPDA_Test, UpdateTransitionPushSymbol_ThrowsIfPushSymbolNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionPushSymbol(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionPushSymbol_ThrowsIfTransitionExists) {
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q1", "1", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->updateTransitionPushSymbol(transitionKey, "1"), InvalidTransitionException);
}

TEST_F(DPDA_Test, UpdateTransitionPushSymbol_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionPushSymbol("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(DPDA_Test, RemoveTransition_RemovesValidTransition) {
	automaton->addTransition("q0", "q1", "0", "A", "A");

	std::string transitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");
	EXPECT_TRUE(automaton->getState("q0").transitionExists(transitionKey));

	automaton->removeTransition(transitionKey);

	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DPDA_Test, RemoveTransition_ThrowsIfTransitionNotFound) {
	std::string invalidTransitionKey = PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A");

	EXPECT_THROW(automaton->removeTransition(invalidTransitionKey), TransitionNotFoundException);
}

TEST_F(DPDA_Test, ClearTransitionsBetween_RemovesTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");

	automaton->clearTransitionsBetween("q0", "q1");

	EXPECT_FALSE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q1", "0", "A", "A")));
	EXPECT_TRUE(
	    automaton->getState("q0").transitionExists(PDATransition::generateTransitionKey("q0", "q2", "1", "A", "A")));
}

TEST_F(DPDA_Test, ClearTransitionsBetween_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearTransitionsBetween("qX", "qY"), StateNotFoundException);
}

TEST_F(DPDA_Test, ClearStateTransitions_RemovesAllTransitionsFromState) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");

	automaton->clearStateTransitions("q0");

	EXPECT_TRUE(automaton->getState("q0").getTransitions().empty());
}

TEST_F(DPDA_Test, ClearStateTransitions_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearStateTransitions("qX"), StateNotFoundException);
}

TEST_F(DPDA_Test, ClearTransitions_RemovesAllTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", "A");
	automaton->addTransition("q0", "q2", "1", "A", "A");
	automaton->addTransition("q1", "q0", "0", "A", "A");

	automaton->clearTransitions();

	for (auto &state : automaton->getStates()) {
		EXPECT_TRUE(state.getTransitions().empty());
	}
}

TEST_F(DPDA_Test, AddAcceptState_ValidState) {
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_TRUE(acceptStates[0].getIsAccept());
}

TEST_F(DPDA_Test, AddAcceptState_DuplicateAcceptState) {
	automaton->addAcceptState("q1");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(DPDA_Test, AddAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->addAcceptState("qX"), StateNotFoundException);
}

TEST_F(DPDA_Test, AddAcceptStates_AddsMultipleAcceptStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_EQ(acceptStates[1].getKey(), "q2");
}

TEST_F(DPDA_Test, AddAcceptStates_ThrowsForMissingStates) {
	EXPECT_THROW(automaton->addAcceptStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(DPDA_Test, RemoveAcceptState_ValidState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DPDA_Test, RemoveAcceptState_NonAcceptState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q0");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(DPDA_Test, RemoveAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeAcceptState("qX"), StateNotFoundException);
}

TEST_F(DPDA_Test, RemoveAcceptStates_AddsMultipleAcceptStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 2);

	automaton->removeAcceptStates({"q1", "q2"});
	acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 0);
}

TEST_F(DPDA_Test, RemoveAcceptStates_ThrowsForMissingStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();

	EXPECT_THROW(automaton->removeAcceptStates({"q2", "q3"}), StateNotFoundException);
}

TEST_F(DPDA_Test, ClearAcceptStates_RemovesAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DPDA_Test, ClearAcceptStates_NoAcceptStates) {
	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DPDA_Test, GetAcceptStates_ReturnsAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();

	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_TRUE((acceptStates[0].getKey() == "q0" && acceptStates[1].getKey() == "q1") ||
	            (acceptStates[0].getKey() == "q1" && acceptStates[1].getKey() == "q0"));
}

TEST_F(DPDA_Test, GetAcceptStates_EmptyIfNoAcceptStates) {
	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DPDA_Test, Reset_SetsCurrentStateToStartState) {
	automaton->addTransition("q0", "q1", "0", "Z", "A");
	automaton->processInput("0");

	EXPECT_EQ(automaton->getCurrentState(), "q1");

	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
}

TEST_F(DPDA_Test, Reset_HandlesAcceptStartState) {
	automaton->addAcceptState("q0");
	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
	EXPECT_FALSE(automaton->getAcceptStates().empty());
}

TEST_F(DPDA_Test, Simulate_ShouldThrowExceptionForSimulationWithoutStartState) {
	DeterministicPushdownAutomaton *automaton = new DeterministicPushdownAutomaton();
	automaton->setInputAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(DPDA_Test, Simulate_ShouldAcceptBasicInput) {
	automaton->addTransition("q0", "q1", "1", "Z", "Z");
	automaton->addAcceptState("q1");
	EXPECT_TRUE(automaton->simulate({"1"}));
}

TEST_F(DPDA_Test, Simulate_ShouldRejectInvalidInput) {
	automaton->addTransition("q0", "q1", "1", "Z", "Z");
	automaton->addAcceptState("q1");
	EXPECT_FALSE(automaton->simulate({"0"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleMultipleTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "1", "Z", "Z");
	automaton->addTransition("q1", "q2", "0", "Z", "Z");
	automaton->addAcceptState("q2");
	EXPECT_TRUE(automaton->simulate({"1", "0"}));
	EXPECT_FALSE(automaton->simulate({"1", "1"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleStackPushingAndPopping) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "1", "Z", "A");
	automaton->addTransition("q1", "q2", "0", "A", "");
	automaton->addAcceptState("q2");
	EXPECT_TRUE(automaton->simulate({"1", "0"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleEmptyInput) {
	automaton->addAcceptState("q0");
	EXPECT_TRUE(automaton->simulate({}));

	automaton->removeAcceptState("q0");
	EXPECT_FALSE(automaton->simulate({}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleComplexStackOperations) {
	automaton->addState("q2");
	automaton->addInputAlphabet({"#"});
	automaton->addTransition("q0", "q0", "1", "Z", "Z,A"); // Push A on Z for each "1"
	automaton->addTransition("q0", "q0", "1", "A", "A");   // Push A on A for each "1"
	automaton->addTransition("q0", "q1", "0", "A", "");    // Pop A for each "0"
	automaton->addTransition("q1", "q1", "0", "A", "");    // Continue popping A's
	automaton->addTransition("q1", "q2", "#", "Z", "Z");   // End marker
	automaton->addAcceptState("q2");

	// This should accept "111000#" (push 3 A's, pop 3 A's, then marker)
	EXPECT_TRUE(automaton->simulate({"1", "1", "1", "0", "0", "0", "#"}));

	// This should reject "110#" (unbalanced 1's and 0's)
	EXPECT_FALSE(automaton->simulate({"1", "1", "0", "#"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleMultipleEpsilonTransitions) {
	automaton->addState("q2");
	automaton->addState("q3");

	automaton->addTransition("q0", "q1", "", "Z", "Z");  // Epsilon from q0 to q1
	automaton->addTransition("q1", "q2", "", "Z", "Z");  // Epsilon from q1 to q2
	automaton->addTransition("q2", "q3", "1", "Z", "Z"); // Actual input
	automaton->addAcceptState("q3");
	EXPECT_TRUE(automaton->simulate({"1"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleStackGrowthAndShrinking) {
	automaton->addState("q2");
	// Build stack: 1=push, 0=pop
	automaton->addTransition("q0", "q0", "1", "Z", "Z,A"); // First 1 pushes A onto Z
	automaton->addTransition("q0", "q0", "1", "A", "A");   // Additional 1's push A
	automaton->addTransition("q0", "q1", "0", "A", "");    // 0 moves to q1 and pops A
	automaton->addTransition("q1", "q1", "0", "A", "");    // Additional 0's pop A's
	automaton->addTransition("q1", "q2", "", "Z", "Z");    // When Z is on top, epsilon to q2
	automaton->addAcceptState("q2");

	// Push 3 A's, then pop 3 A's
	EXPECT_TRUE(automaton->simulate({"1", "1", "1", "0", "0", "0"}));

	// Push 2 A's, but pop 3 (should fail - stack underflow)
	EXPECT_FALSE(automaton->simulate({"1", "1", "0", "0", "0"}));
}

TEST_F(DPDA_Test, Simulate_ShouldRecognizePalindrome) {
	automaton->setInputAlphabet({"a", "b", "c"});
	automaton->setStackAlphabet({"Z", "A", "B"});
	automaton->addState("q2");
	// Basic palindrome recognizer for even-length strings of a's and b's
	automaton->addTransition("q0", "q0", "a", "Z", "A"); // Push A for 'a'
	automaton->addTransition("q0", "q0", "a", "A", "A"); // Push A for 'a'
	automaton->addTransition("q0", "q0", "a", "B", "A"); // Push A for 'a' on B
	automaton->addTransition("q0", "q0", "b", "Z", "B"); // Push B for 'b'
	automaton->addTransition("q0", "q0", "b", "A", "B"); // Push B for 'b' on A
	automaton->addTransition("q0", "q0", "b", "B", "B"); // Push B for 'b' on B

	automaton->addTransition("q0", "q1", "c", "Z", "Z"); // Middle marker on Z
	automaton->addTransition("q0", "q1", "c", "A", "A"); // Middle marker on A
	automaton->addTransition("q0", "q1", "c", "B", "B"); // Middle marker on B

	automaton->addTransition("q1", "q1", "a", "A", ""); // Pop A for 'a'
	automaton->addTransition("q1", "q1", "b", "B", ""); // Pop B for 'b'

	automaton->addTransition("q1", "q2", "", "Z", "Z"); // When only Z remains, go to accept
	automaton->addAcceptState("q2");

	// Palindrome "abcba" (a, b, middle, b, a)
	EXPECT_TRUE(automaton->simulate({"a", "b", "c", "b", "a"}));

	// Not a palindrome "abcab"
	EXPECT_FALSE(automaton->simulate({"a", "b", "c", "a", "b"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleMultiCharacterStackSymbols) {
	automaton->setStackAlphabet({"Z", "A", "B"});
	automaton->addState("q2");
	automaton->addState("q3");

	automaton->addTransition("q0", "q1", "1", "Z", "B,A"); // Push "AB" onto Z
	automaton->addTransition("q1", "q2", "0", "A", "");    // Pop A
	automaton->addTransition("q2", "q3", "0", "B", "");    // Pop B
	automaton->addAcceptState("q3");
	EXPECT_TRUE(automaton->simulate({"1", "0", "0"}));
}

TEST_F(DPDA_Test, Simulate_ShouldHandleEmptyStackHandling) {
	automaton->addState("q2");

	// Test behavior when attempting to pop from an empty stack
	automaton->addTransition("q0", "q1", "1", "Z", "");  // Pop the initial Z
	automaton->addTransition("q1", "q2", "0", "Z", "Z"); // Should fail - no Z on stack
	automaton->addAcceptState("q2");
	EXPECT_FALSE(automaton->simulate({"1", "0"})); // Should fail on the second transition
}

TEST_F(DPDA_Test, Simulate_ShouldTestSimulationDepthLimit) {
	// Create a loop that could cause infinite recursion
	automaton->addTransition("q0", "q0", "", "Z", "Z"); // Epsilon transition to self

	// This should throw SimulationDepthExceededException
	EXPECT_THROW(automaton->simulate({"1"}, 5), SimulationDepthExceededException);
}

TEST_F(DPDA_Test, ShouldProcessInputsForLanguageEndingWith01) {
	DeterministicPushdownAutomaton *automaton = new DeterministicPushdownAutomaton();
	// Add states
	automaton->addState("q0"); // Start state
	automaton->addState("q1");
	automaton->addState("q2"); // Accept state for strings ending with "01"

	// Set the start state
	automaton->setStartState("q0");

	// Set the input and stack alphabets
	automaton->setInputAlphabet({"0", "1", ""});
	automaton->setStackAlphabet({"A", "Z"});

	// Add transitions
	automaton->addTransition("q0", "q1", "0", "Z", "A");
	automaton->addTransition("q1", "q2", "1", "A", "");

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

TEST_F(DPDA_Test, ShouldProcessInputsForBalancedParentheses) {
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
	automaton->addTransition("q0", "q0", "(", "Z", "A");
	automaton->addTransition("q0", "q0", "(", "A", "A");
	automaton->addTransition("q0", "q0", ")", "A", "");
	automaton->addTransition("q0", "q1", "", "Z", "");

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