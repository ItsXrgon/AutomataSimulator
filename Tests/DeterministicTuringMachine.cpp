#include "pch.h"

class DTM_Test : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new DeterministicTuringMachine();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1"});
		automaton->addTapeAlphabet({"A", "Z"});
	}

	void TearDown() override {
		delete automaton;
	}

	DeterministicTuringMachine *automaton;
};

TEST_F(DTM_Test, GetInput_GetsInput) {
	std::vector<std::string> input = automaton->getInput();
	EXPECT_EQ(input.size(), 0);
}

TEST_F(DTM_Test, SetInput_SetsInput) {
	automaton->setInput({"0", "1", "0"});
	std::vector<std::string> input = automaton->getInput();

	EXPECT_EQ(input.size(), 3);
	EXPECT_TRUE(std::find(input.begin(), input.end(), "0") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "1") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "0") != input.end());
}

TEST_F(DTM_Test, SetInput_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->setInput({"0", "", "1"}));
}

TEST_F(DTM_Test, SetInput_ThrowsForInputNotInAlphabet) {
	EXPECT_THROW(automaton->setInput({"a", "b"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DTM_Test, SetInput_AddsNewInputs) {
	automaton->addInputAlphabet({"2", "3"});
	automaton->addInput({"2", "3"});
	std::vector<std::string> input = automaton->getInput();

	EXPECT_EQ(input.size(), 2);
	EXPECT_TRUE(std::find(input.begin(), input.end(), "2") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "3") != input.end());
}

TEST_F(DTM_Test, AddInput_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->addInput({"0", "", "1"}));
}

TEST_F(DTM_Test, AddInput_ThrowsForInputNotInAlphabet) {
	EXPECT_THROW(automaton->setInput({"a", "b"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DTM_Test, SetInputAlphabet_SetsAlphabet) {
	automaton->setInput({"0", "1", "0"});
	std::vector<std::string> input = automaton->getInput();

	EXPECT_EQ(input.size(), 3);
	EXPECT_TRUE(std::find(input.begin(), input.end(), "0") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "1") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "0") != input.end());
}

TEST_F(DTM_Test, StateExists_ReturnsTrueWhenStateExists) {
	EXPECT_TRUE(automaton->stateExists("q0"));
	EXPECT_TRUE(automaton->stateExists("q1"));
}

TEST_F(DTM_Test, StateExists_ReturnsFalseWhenStateDoesNotExist) {
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_FALSE(automaton->stateExists(""));
}

TEST_F(DTM_Test, InputAlphabetSymbolExists_ReturnsTrueForValidSymbol) {
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("0"));
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("1"));
}

TEST_F(DTM_Test, InputAlphabetSymbolExists_ReturnsFalseForInvalidSymbol) {
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("2"));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists(""));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("abc"));
}

TEST_F(DTM_Test, AddState_AddsNewStateSuccessfully) {
	automaton->addState("q2");
	EXPECT_TRUE(automaton->stateExists("q2"));
}

TEST_F(DTM_Test, AddState_ThrowsForDuplicateState) {
	EXPECT_THROW(automaton->addState("q0"), InvalidAutomatonDefinitionException);
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, AddState_AddsMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));
}

TEST_F(DTM_Test, AddState_AddsStateAsAccept) {
	automaton->addState("q2", true);
	EXPECT_TRUE(automaton->getState("q2").getIsAccept());
}

TEST_F(DTM_Test, UpdateStateLabel_UpdatesStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->stateExists("q3"));
	automaton->updateStateLabel("q3", "q4");
	EXPECT_TRUE(automaton->stateExists("q4"));
}

TEST_F(DTM_Test, UpdateStateLabel_UpdatesStartStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->getStartState() == "q3");
}

TEST_F(DTM_Test, GetCurrentState_GetsStartStateIfNoCurrentStateSet) {
	EXPECT_TRUE(automaton->getCurrentState() == "q0");
}

TEST_F(DTM_Test, GetCurrentState_ThrowsIfNoStartStateOrCurrentStateSet) {
	DeterministicTuringMachine *automaton = new DeterministicTuringMachine();
	automaton->addState("q0");

	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);

	delete automaton;
}

TEST_F(DTM_Test, SetCurrentState_UpdatesCurrentState) {
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->getCurrentState() == "q1");
}

TEST_F(DTM_Test, SetCurrentState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setCurrentState("q3"), StateNotFoundException);
}

TEST_F(DTM_Test, GetState_GetsState) {
	TMState state = automaton->getState("q0");
	EXPECT_TRUE(state.getLabel() == "q0");
}

TEST_F(DTM_Test, GetState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->getState("q3"), StateNotFoundException);
}

TEST_F(DTM_Test, GetStates_GetsStates) {
	std::vector<TMState> states = automaton->getStates();
	EXPECT_TRUE(states.size() == 2);

	bool foundQ0 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q0") {
			foundQ0 = true;
		}
	}
	EXPECT_TRUE(foundQ0);
}

TEST_F(DTM_Test, GetStates_GetsStatesAfterAddStateCall) {
	automaton->addState("q3");
	std::vector<TMState> states = automaton->getStates();

	bool foundQ3 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q3") {
			foundQ3 = true;
		}
	}
	EXPECT_TRUE(foundQ3);
}

TEST_F(DTM_Test, GetStates_GetsStatesAfterUpdateStateLabelCall) {
	automaton->updateStateLabel("q0", "q3");
	std::vector<TMState> states = automaton->getStates();

	bool foundQ3 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q3") {
			foundQ3 = true;
		}
	}
	EXPECT_TRUE(foundQ3);
}

TEST_F(DTM_Test, RemoveState_RemovesState) {
	EXPECT_TRUE(automaton->getStates().size() == 2);

	automaton->removeState("q1");
	EXPECT_FALSE(automaton->stateExists("q1"));

	EXPECT_TRUE(automaton->getStates().size() == 1);
}

TEST_F(DTM_Test, RemoveState_RemovesStartState) {
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DTM_Test, RemoveState_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, RemoveState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeState("q3"), StateNotFoundException);
}

TEST_F(DTM_Test, RemoveState_RemovesTransitionsWithStateAsToIfNotStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	automaton->removeState("q0", false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(
	    TMTransition::generateTransitionKey("q1", "q0", "0", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, RemoveState_ThrowsIfStateIsUsedInTransitionsIfStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	EXPECT_THROW(automaton->removeState("q0", true), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, RemoveStates_RemovesMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->getStates().size() == 4);

	automaton->removeStates({"q1", "q2"});
	EXPECT_FALSE(automaton->stateExists("q1"));
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));

	EXPECT_TRUE(automaton->getStates().size() == 2);
}

TEST_F(DTM_Test, RemoveStates_RemovesStartState) {
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DTM_Test, RemoveStates_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, RemoveStates_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(DTM_Test, RemoveStates_RemovesTransitionsWithStateAsToIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	automaton->removeStates({"q0"}, false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(
	    TMTransition::generateTransitionKey("q1", "q0", "0", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, RemoveStates_ThrowsIfStateIsUsedInTransitionsIfStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	EXPECT_THROW(automaton->removeStates({"q0"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, ClearStates_RemovesAllStates) {
	EXPECT_TRUE(automaton->getStates().size() == 2);
	automaton->clearStates();
	EXPECT_TRUE(automaton->getStates().empty());
}

TEST_F(DTM_Test, ClearStates_RemovesCurrentState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, ClearStates_RemovesStartState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DTM_Test, GetInputAlphabet_GetsAlphabet) {
	std::vector<std::string> alphabet = automaton->getInputAlphabet();
	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DTM_Test, SetInputAlphabet_OverridesAlphabet) {
	automaton->setInputAlphabet({"a", "b", "c"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 3);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "a") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "b") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "c") != alphabet.end());
}

TEST_F(DTM_Test, SetInputAlphabet_AddsToTapeAlphabet) {
	automaton->setInputAlphabet({"a", "b", "c"});
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "a") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "b") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "c") != alphabet.end());
}

TEST_F(DTM_Test, SetInputAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->setInputAlphabet({"a", "", "c"}));
}

TEST_F(DTM_Test, SetInputAlphabet_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	EXPECT_THROW(automaton->setInputAlphabet({"a", "b"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, SetInputAlphabet_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	automaton->setInputAlphabet({"a", "b"}, false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(
	    TMTransition::generateTransitionKey("q1", "q0", "0", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, AddInputAlphabet_AddsNewSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 4);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
}

TEST_F(DTM_Test, AddInputAlphabet_AddsSymbolsToTapeAlphabet) {
	automaton->addInputAlphabet({"2", "3"});
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
}

TEST_F(DTM_Test, AddInputAlphabet_ThrowsForEpsilon) {
	EXPECT_NO_THROW(automaton->addInputAlphabet({"a", "", "c"}));
}

TEST_F(DTM_Test, AddInputAlphabet_IgnoresDuplicates) {
	automaton->addInputAlphabet({"1", "0", "1"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbol_RemovesSymbol) {
	automaton->removeInputAlphabetSymbol("0");
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 1);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbol_ThrowsIfNotFound) {
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("x"), InputAlphabetSymbolNotFoundException);
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbol_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("0", true), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbol_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	automaton->removeInputAlphabetSymbol("0", false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(
	    TMTransition::generateTransitionKey("q1", "q0", "0", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbols_RemovesMultipleSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	automaton->removeInputAlphabetSymbols({"0", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbols_ThrowsIfAnySymbolNotFound) {
	automaton->addInputAlphabet({"2"});
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"1", "x"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbols_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"0"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, RemoveInputAlphabetSymbols_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	automaton->removeInputAlphabetSymbols({"0"}, false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(
	    TMTransition::generateTransitionKey("q1", "q0", "0", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, ClearInputAlphabet_RemovesAllSymbols) {
	automaton->clearInputAlphabet();
	EXPECT_TRUE(automaton->getInputAlphabet().empty());
}

TEST_F(DTM_Test, ClearInputAlphabet_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	EXPECT_THROW(automaton->clearInputAlphabet(true), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, ClearInputAlphabet_RemovesNonEpsilonTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::LEFT);
	automaton->addTransition("q0", "q1", "", "A", TMDirection::LEFT);

	automaton->clearInputAlphabet(false);

	EXPECT_FALSE(automaton->getState("q1").transitionExists(
	    TMTransition::generateTransitionKey("q1", "q0", "0", "A", TMDirection::LEFT)));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q1", "", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, GetTapeAlphabet_GetsAlphabet) {
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();
	EXPECT_EQ(alphabet.size(), 4);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
}

TEST_F(DTM_Test, SetTapeAlphabet_OverridesAlphabet) {
	automaton->setTapeAlphabet({"A", "_", "C"});
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_EQ(alphabet.size(), 3);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "_") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
}

TEST_F(DTM_Test, SetTapeAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->setTapeAlphabet({"A", "", "C"}));
}

TEST_F(DTM_Test, AddTapeAlphabet_AddsNewSymbols) {
	automaton->addTapeAlphabet({"_", "C"});
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_EQ(alphabet.size(), 6);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "_") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
}

TEST_F(DTM_Test, AddTapeAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->addTapeAlphabet({"A", "", "C"}));
}

TEST_F(DTM_Test, AddTapeAlphabet_IgnoresDuplicates) {
	automaton->addTapeAlphabet({"Z", "A", "Z"});
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
}

TEST_F(DTM_Test, RemoveTapeAlphabetSymbol_RemovesSymbol) {
	automaton->removeTapeAlphabetSymbol("A");
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
}

TEST_F(DTM_Test, RemoveTapeAlphabetSymbol_RemovesSymbolFromInputAlphabet) {
	automaton->addInputAlphabet({"A"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());

	automaton->removeTapeAlphabetSymbol("A");
	alphabet = automaton->getInputAlphabet();
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
}

TEST_F(DTM_Test, RemoveTapeAlphabetSymbol_ThrowsIfNotFound) {
	EXPECT_THROW(automaton->removeTapeAlphabetSymbol("X"), TapeAlphabetSymbolNotFoundException);
}

TEST_F(DTM_Test, RemoveTapeAlphabetSymbols_RemovesMultipleSymbols) {
	automaton->addTapeAlphabet({"_", "C"});
	automaton->removeTapeAlphabetSymbols({"Z", "C"});
	std::vector<std::string> alphabet = automaton->getTapeAlphabet();

	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "Z") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "A") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "_") != alphabet.end());
}

TEST_F(DTM_Test, RemoveTapeAlphabetSymbols_RemovesSymbolsFromInputAlphabet) {
	automaton->addInputAlphabet({"_", "C"});
	automaton->addTapeAlphabet({"_", "C"});

	std::vector<std::string> alphabet = automaton->getInputAlphabet();
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "_") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());

	automaton->removeTapeAlphabetSymbols({"_", "C"});
	alphabet = automaton->getInputAlphabet();

	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "_") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "C") != alphabet.end());
}

TEST_F(DTM_Test, RemoveTapeAlphabetSymbols_ThrowsIfAnySymbolNotFound) {
	automaton->addTapeAlphabet({"_"});
	EXPECT_THROW(automaton->removeTapeAlphabetSymbols({"_", "X"}), TapeAlphabetSymbolNotFoundException);
}

TEST_F(DTM_Test, ClearTapeAlphabet_RemovesAllSymbols) {
	automaton->clearTapeAlphabet();
	EXPECT_TRUE(automaton->getTapeAlphabet().empty());
}

TEST_F(DTM_Test, ClearTapeAlphabet_RemovesInputAlphabetSymbols) {
	automaton->clearTapeAlphabet();
	EXPECT_TRUE(automaton->getInputAlphabet().empty());
}

TEST_F(DTM_Test, GetStartState_ReturnsCorrectState) {
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(DTM_Test, GetStartState_ThrowsIfNotSet) {
	automaton->removeState("q0");
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DTM_Test, SetStartState_SetsNewStartState) {
	automaton->addState("q2");
	automaton->setStartState("q2");
	EXPECT_EQ(automaton->getStartState(), "q2");
}

TEST_F(DTM_Test, SetStartState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(DTM_Test, SetStartState_SetsCurrentStateIfEmpty) {
	automaton->clearStates();
	automaton->addState("q4");
	automaton->setStartState("q4");
	EXPECT_EQ(automaton->getStartState(), "q4");
	EXPECT_EQ(automaton->getCurrentState(), "q4");
}

TEST_F(DTM_Test, SetStartState_DoesNotOverrideCurrentStateIfSet) {
	automaton->addState("q2");
	automaton->setCurrentState("q1");
	automaton->setStartState("q2");

	EXPECT_EQ(automaton->getStartState(), "q2");
	EXPECT_EQ(automaton->getCurrentState(), "q1");
}

TEST_F(DTM_Test, AddTransition_AddsValidTransition) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::LEFT);

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool found = false;

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() == "0" && transition.getToStateKey() == "q1") {
			found = true;
			break;
		}
	}

	EXPECT_TRUE(found);
}

TEST_F(DTM_Test, AddTransition_ThrowsIfFromStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("qX", "q1", "0", "A", TMDirection::STAY), StateNotFoundException);
}

TEST_F(DTM_Test, AddTransition_ThrowsIfToStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("q0", "qX", "0", "A", TMDirection::STAY), StateNotFoundException);
}

TEST_F(DTM_Test, AddTransition_ThrowsIfInputNotInAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "q1", "2", "A", TMDirection::STAY), InvalidTransitionException);
}

TEST_F(DTM_Test, AddTransition_NoThrowIfEpsilonInDTM) {
	EXPECT_NO_THROW(automaton->addTransition("q0", "q1", "", "A", TMDirection::STAY));
}

TEST_F(DTM_Test, AddTransition_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	EXPECT_THROW(automaton->addTransition("q0", "q2", "0", "A", TMDirection::STAY),
	             InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, AddTransition_AllowsDifferentInputs) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q0", "1", "A", TMDirection::STAY);

	const auto &transitions = automaton->getState("q0").getTransitions();

	bool found0 = false, found1 = false;
	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() == "0" && transition.getToStateKey() == "q1") {
			found0 = true;
		}
		if (transition.getReadSymbol() == "1" && transition.getToStateKey() == "q0") {
			found1 = true;
		}
	}

	EXPECT_TRUE(found0);
	EXPECT_TRUE(found1);
}

TEST_F(DTM_Test, AddTransition_DoesNotInvalidateExistingTransitions) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q1", "q0", "1", "A", TMDirection::STAY);

	const auto &q0Transitions = automaton->getState("q0").getTransitions();
	const auto &q1Transitions = automaton->getState("q1").getTransitions();

	EXPECT_EQ(q0Transitions.size(), 1);
	EXPECT_EQ(q1Transitions.size(), 1);

	EXPECT_EQ(q0Transitions[0].getReadSymbol(), "0");
	EXPECT_EQ(q1Transitions[0].getReadSymbol(), "1");
}

TEST_F(DTM_Test, UpdateTransitionFromState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	automaton->updateTransitionFromState(transitionKey, "q2");

	EXPECT_TRUE(automaton->getState("q2").transitionExists(
	    TMTransition::generateTransitionKey("q2", "q1", "0", "A", TMDirection::STAY)));
	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DTM_Test, UpdateTransitionFromState_ThrowsIfDuplicate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q2", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q2"), InvalidTransitionException);
}

TEST_F(DTM_Test, UpdateTransitionFromState_ThrowsIfTransitionNotFound) {
	automaton->addState("q2");
	EXPECT_THROW(automaton->updateTransitionFromState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(DTM_Test, UpdateTransitionFromState_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q1", "q2", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q1", "q2", "0", "A", TMDirection::STAY);
	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q0"), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, UpdateTransitionToState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	automaton->updateTransitionToState(transitionKey, "q2");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() == "0" && transition.getToStateKey() == "q2") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DTM_Test, UpdateTransitionToState_ThrowsIfStateNotFound) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->updateTransitionToState(transitionKey, "qX"), StateNotFoundException);
}

TEST_F(DTM_Test, UpdateTransitionToState_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionToState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(DTM_Test, UpdateTransitionToState_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q1", "1", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->updateTransitionToState(transitionKey, "q2");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q1", "1", "A", TMDirection::STAY)));
}

TEST_F(DTM_Test, UpdateTransitionReadSymbol_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	automaton->updateTransitionReadSymbol(transitionKey, "Z");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getReadSymbol() == "Z" && transition.getToStateKey() == "q1") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
}

TEST_F(DTM_Test, UpdateTransitionReadSymbol_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->updateTransitionReadSymbol(transitionKey, "Z");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q2", "1", "A", TMDirection::STAY)));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q1", "Z", "A", TMDirection::STAY)));
}

TEST_F(DTM_Test, UpdateTransitionReadSymbol_ThrowsIfReadSymbolNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->updateTransitionReadSymbol(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(DTM_Test, UpdateTransitionReadSymbol_ThrowsIfTransitionExists) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q1", "1", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->updateTransitionReadSymbol(transitionKey, "1"), InvalidTransitionException);
}

TEST_F(DTM_Test, UpdateTransitionReadSymbol_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionReadSymbol("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(DTM_Test, UpdateTransitionReadSymbol_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q2", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->updateTransitionReadSymbol(transitionKey, "1"), InvalidAutomatonDefinitionException);
}

TEST_F(DTM_Test, UpdateTransitionWriteSymbol_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	automaton->updateTransitionWriteSymbol(transitionKey, "Z");

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getWriteSymbol() == "Z" && transition.getToStateKey() == "q1") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
}

TEST_F(DTM_Test, UpdateTransitionWriteSymbol_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->updateTransitionWriteSymbol(transitionKey, "Z");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q2", "1", "A", TMDirection::STAY)));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q1", "0", "Z", TMDirection::STAY)));
}

TEST_F(DTM_Test, UpdateTransitionWriteSymbol_ThrowsIfWriteSymbolNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->updateTransitionWriteSymbol(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(DTM_Test, UpdateTransitionWriteSymbol_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionWriteSymbol("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(DTM_Test, UpdateTransitionDirection_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	automaton->updateTransitionDirection(transitionKey, TMDirection::LEFT);

	const auto &transitions = automaton->getState("q0").getTransitions();
	bool foundUpdated = false;

	for (const auto &transition : transitions) {
		if (transition.getDirection() == TMDirection::LEFT && transition.getToStateKey() == "q1") {
			foundUpdated = true;
		}
	}

	EXPECT_TRUE(foundUpdated);
}

TEST_F(DTM_Test, UpdateTransitionDirection_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->updateTransitionDirection(transitionKey, TMDirection::LEFT);

	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q2", "1", "A", TMDirection::STAY)));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::LEFT)));
}

TEST_F(DTM_Test, UpdateTransitionDirection_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionDirection("invalidKey", TMDirection::LEFT), TransitionNotFoundException);
}

TEST_F(DTM_Test, RemoveTransition_RemovesValidTransition) {
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);

	std::string transitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);
	EXPECT_TRUE(automaton->getState("q0").transitionExists(transitionKey));

	automaton->removeTransition(transitionKey);

	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DTM_Test, RemoveTransition_ThrowsIfTransitionNotFound) {
	std::string invalidTransitionKey = TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY);

	EXPECT_THROW(automaton->removeTransition(invalidTransitionKey), TransitionNotFoundException);
}

TEST_F(DTM_Test, ClearTransitionsBetween_RemovesTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);

	automaton->clearTransitionsBetween("q0", "q1");

	EXPECT_FALSE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q1", "0", "A", TMDirection::STAY)));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(
	    TMTransition::generateTransitionKey("q0", "q2", "1", "A", TMDirection::STAY)));
}

TEST_F(DTM_Test, ClearTransitionsBetween_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearTransitionsBetween("qX", "qY"), StateNotFoundException);
}

TEST_F(DTM_Test, ClearStateTransitions_RemovesAllTransitionsFromState) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);

	automaton->clearStateTransitions("q0");

	EXPECT_TRUE(automaton->getState("q0").getTransitions().empty());
}

TEST_F(DTM_Test, ClearStateTransitions_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearStateTransitions("qX"), StateNotFoundException);
}

TEST_F(DTM_Test, ClearTransitions_RemovesAllTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0", "A", TMDirection::STAY);
	automaton->addTransition("q0", "q2", "1", "A", TMDirection::STAY);
	automaton->addTransition("q1", "q0", "0", "A", TMDirection::STAY);

	automaton->clearTransitions();

	for (auto &state : automaton->getStates()) {
		EXPECT_TRUE(state.getTransitions().empty());
	}
}

TEST_F(DTM_Test, AddAcceptState_ValidState) {
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_TRUE(acceptStates[0].getIsAccept());
}

TEST_F(DTM_Test, AddAcceptState_DuplicateAcceptState) {
	automaton->addAcceptState("q1");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(DTM_Test, AddAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->addAcceptState("qX"), StateNotFoundException);
}

TEST_F(DTM_Test, AddAcceptStates_AddsMultipleAcceptStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_EQ(acceptStates[1].getKey(), "q2");
}

TEST_F(DTM_Test, AddAcceptStates_ThrowsForMissingStates) {
	EXPECT_THROW(automaton->addAcceptStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(DTM_Test, RemoveAcceptState_ValidState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DTM_Test, RemoveAcceptState_NonAcceptState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q0");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(DTM_Test, RemoveAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeAcceptState("qX"), StateNotFoundException);
}

TEST_F(DTM_Test, RemoveAcceptStates_RemovesMultipleAcceptStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 2);

	automaton->removeAcceptStates({"q1", "q2"});
	acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 0);
}

TEST_F(DTM_Test, RemoveAcceptStates_ThrowsForMissingStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();

	EXPECT_THROW(automaton->removeAcceptStates({"q2", "q3"}), StateNotFoundException);
}

TEST_F(DTM_Test, ClearAcceptStates_RemovesAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DTM_Test, ClearAcceptStates_NoAcceptStates) {
	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DTM_Test, GetAcceptStates_ReturnsAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();

	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_TRUE((acceptStates[0].getKey() == "q0" && acceptStates[1].getKey() == "q1") ||
	            (acceptStates[0].getKey() == "q1" && acceptStates[1].getKey() == "q0"));
}

TEST_F(DTM_Test, GetAcceptStates_EmptyIfNoAcceptStates) {
	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DTM_Test, Reset_SetsCurrentStateToStartState) {
	automaton->addAcceptState("q0");
	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
}

TEST_F(DTM_Test, Reset_HandlesAcceptStartState) {
	automaton->addAcceptState("q0");
	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
}

TEST_F(DTM_Test, IsAccepting_ShouldAcceptIfStartStateIsAccept) {
	automaton->addAcceptState("q0");
	EXPECT_TRUE(automaton->isAccepting());
}

TEST_F(DTM_Test, IsAccepting_ShouldAcceptIfCurrentStateIsAccept) {
	automaton->addAcceptState("q1");
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->isAccepting());
}

TEST_F(DTM_Test, ProcessInput_ShouldAcceptValidSequence) {
	automaton->addState("q2", false);
	automaton->addState("q3", false);
	automaton->addState("q4", false);
	automaton->addState("q5", false);
	automaton->addState("q6", false);
	automaton->addState("q_accept", true);

	automaton->addInputAlphabet({"0", "1", "_"});

	automaton->addTransition("q0", "q1", "", "", TMDirection::STAY);

	automaton->addTransition("q1", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q1", "q2", "0", "_", TMDirection::RIGHT);
	automaton->addTransition("q1", "q5", "1", "_", TMDirection::RIGHT);

	automaton->addTransition("q2", "q2", "0", "0", TMDirection::RIGHT);
	automaton->addTransition("q2", "q2", "1", "1", TMDirection::RIGHT);
	automaton->addTransition("q2", "q3", "_", "_", TMDirection::LEFT);

	automaton->addTransition("q3", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q3", "q4", "0", "_", TMDirection::LEFT);

	automaton->addTransition("q4", "q4", "0", "0", TMDirection::LEFT);
	automaton->addTransition("q4", "q4", "1", "1", TMDirection::LEFT);
	automaton->addTransition("q4", "q1", "_", "_", TMDirection::RIGHT);

	automaton->addTransition("q5", "q5", "0", "0", TMDirection::RIGHT);
	automaton->addTransition("q5", "q5", "1", "1", TMDirection::RIGHT);
	automaton->addTransition("q5", "q6", "_", "_", TMDirection::LEFT);

	automaton->addTransition("q6", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q6", "q4", "1", "_", TMDirection::LEFT);

	automaton->setInput({"1", "0", "1", "1", "0", "1"});

	std::list<std::string> tape = automaton->getTape();
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q1");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q5");
	tape = automaton->getTape();

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q5");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q5");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q5");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q5");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q5");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q6");

	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "q4");
	tape = automaton->getTape();
}

TEST_F(DTM_Test, ProcessInputRejectIfNoTransitionFound) {
	automaton->addAcceptState("q0");
	automaton->setInput({"0"});

	EXPECT_FALSE(automaton->processInput());
}

TEST_F(DTM_Test, Simulate_ShouldThrowExceptionForSimulationWithoutStartState) {
	NonDeterministicTuringMachine *automaton = new NonDeterministicTuringMachine();
	automaton->setInputAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(DTM_Test, Simulate_ValidInputAccepts) {
	automaton->addState("q2", false);
	automaton->addState("q3", false);
	automaton->addState("q4", false);
	automaton->addState("q5", false);
	automaton->addState("q6", false);
	automaton->addState("q_accept", true);

	automaton->addInputAlphabet({"0", "1", "_"});

	automaton->addTransition("q0", "q1", "", "", TMDirection::STAY);

	automaton->addTransition("q1", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q1", "q2", "0", "_", TMDirection::RIGHT);
	automaton->addTransition("q1", "q5", "1", "_", TMDirection::RIGHT);

	automaton->addTransition("q2", "q2", "0", "0", TMDirection::RIGHT);
	automaton->addTransition("q2", "q2", "1", "1", TMDirection::RIGHT);
	automaton->addTransition("q2", "q3", "_", "_", TMDirection::LEFT);

	automaton->addTransition("q3", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q3", "q4", "0", "_", TMDirection::LEFT);

	automaton->addTransition("q4", "q4", "0", "0", TMDirection::LEFT);
	automaton->addTransition("q4", "q4", "1", "1", TMDirection::LEFT);
	automaton->addTransition("q4", "q1", "_", "_", TMDirection::RIGHT);

	automaton->addTransition("q5", "q5", "0", "0", TMDirection::RIGHT);
	automaton->addTransition("q5", "q5", "1", "1", TMDirection::RIGHT);
	automaton->addTransition("q5", "q6", "_", "_", TMDirection::LEFT);

	automaton->addTransition("q6", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q6", "q4", "1", "_", TMDirection::LEFT);

	EXPECT_TRUE(automaton->simulate({"1", "0", "1", "1", "0", "1"}));
}

TEST_F(DTM_Test, Simulate_InvalidInputReject) {
	automaton->addState("q2", false);
	automaton->addState("q3", false);
	automaton->addState("q4", false);
	automaton->addState("q5", false);
	automaton->addState("q6", false);
	automaton->addState("q_accept", true);

	automaton->addInputAlphabet({"0", "1", "_"});

	automaton->addTransition("q0", "q1", "", "", TMDirection::STAY);

	automaton->addTransition("q1", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q1", "q2", "0", "_", TMDirection::RIGHT);
	automaton->addTransition("q1", "q5", "1", "_", TMDirection::RIGHT);

	automaton->addTransition("q2", "q2", "0", "0", TMDirection::RIGHT);
	automaton->addTransition("q2", "q2", "1", "1", TMDirection::RIGHT);
	automaton->addTransition("q2", "q3", "_", "_", TMDirection::LEFT);

	automaton->addTransition("q3", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q3", "q4", "0", "_", TMDirection::LEFT);

	automaton->addTransition("q4", "q4", "0", "0", TMDirection::LEFT);
	automaton->addTransition("q4", "q4", "1", "1", TMDirection::LEFT);
	automaton->addTransition("q4", "q1", "_", "_", TMDirection::RIGHT);

	automaton->addTransition("q5", "q5", "0", "0", TMDirection::RIGHT);
	automaton->addTransition("q5", "q5", "1", "1", TMDirection::RIGHT);
	automaton->addTransition("q5", "q6", "_", "_", TMDirection::LEFT);

	automaton->addTransition("q6", "q_accept", "_", "_", TMDirection::STAY);
	automaton->addTransition("q6", "q4", "1", "_", TMDirection::LEFT);

	EXPECT_FALSE(automaton->simulate({"1", "0", "1", "1", "0", "0"}));
}