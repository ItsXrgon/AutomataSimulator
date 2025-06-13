#include "pch.h"

class DFA_Test : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new DeterministicFiniteAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1"});
	}

	void TearDown() override {
		delete automaton;
	}

	DeterministicFiniteAutomaton *automaton;
};

TEST_F(DFA_Test, GetInput_GetsInput) {
	std::vector<std::string> input = automaton->getInput();
	EXPECT_EQ(input.size(), 0);
}

TEST_F(DFA_Test, SetInput_SetsInput) {
	automaton->setInput({"0", "1", "0"});
	std::vector<std::string> input = automaton->getInput();

	EXPECT_EQ(input.size(), 3);
	EXPECT_TRUE(std::find(input.begin(), input.end(), "0") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "1") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "0") != input.end());
}

TEST_F(DFA_Test, SetInput_ThrowsForEpsilon) {
	EXPECT_THROW(automaton->setInput({"0", "", "1"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DFA_Test, SetInput_ThrowsForSymbolNotInAlphabet) {
	EXPECT_THROW(automaton->setInput({"a", "b"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DFA_Test, SetInput_AddsNewInputs) {
	automaton->addInputAlphabet({"2", "3"});
	automaton->addInput({"2", "3"});
	std::vector<std::string> input = automaton->getInput();

	EXPECT_EQ(input.size(), 2);
	EXPECT_TRUE(std::find(input.begin(), input.end(), "2") != input.end());
	EXPECT_TRUE(std::find(input.begin(), input.end(), "3") != input.end());
}

TEST_F(DFA_Test, AddInput_ThrowsForEpsilon) {
	EXPECT_THROW(automaton->addInput({"0", "", "1"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DFA_Test, AddInput_ThrowsForInputNotInAlphabet) {
	EXPECT_THROW(automaton->setInput({"a", "b"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DFA_Test, StateExists_ReturnsTrueWhenStateExists) {
	EXPECT_TRUE(automaton->stateExists("q0"));
	EXPECT_TRUE(automaton->stateExists("q1"));
}

TEST_F(DFA_Test, StateExists_ReturnsFalseWhenStateDoesNotExist) {
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_FALSE(automaton->stateExists(""));
}

TEST_F(DFA_Test, InputAlphabetSymbolExists_ReturnsTrueForValidSymbol) {
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("0"));
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("1"));
}

TEST_F(DFA_Test, InputAlphabetSymbolExists_ReturnsFalseForInvalidSymbol) {
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("2"));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists(""));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("abc"));
}

TEST_F(DFA_Test, AddState_AddsNewStateSuccessfully) {
	automaton->addState("q2");
	EXPECT_TRUE(automaton->stateExists("q2"));
}

TEST_F(DFA_Test, AddState_ThrowsForDuplicateState) {
	EXPECT_THROW(automaton->addState("q0"), InvalidAutomatonDefinitionException);
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, AddState_AddsMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));
}

TEST_F(DFA_Test, AddState_AddsStateAsAccept) {
	automaton->addState("q2", true);
	EXPECT_TRUE(automaton->getState("q2").getIsAccept());
}

TEST_F(DFA_Test, UpdateStateLabel_UpdatesStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->stateExists("q3"));
	automaton->updateStateLabel("q3", "q4");
	EXPECT_TRUE(automaton->stateExists("q4"));
}

TEST_F(DFA_Test, UpdateStateLabel_UpdatesStartStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->getStartState() == "q3");
}

TEST_F(DFA_Test, GetCurrentState_GetsStartStateIfNoCurrentStateSet) {
	EXPECT_TRUE(automaton->getCurrentState() == "q0");
}

TEST_F(DFA_Test, GetCurrentState_ThrowsIfNoStartStateOrCurrentStateSet) {
	DeterministicFiniteAutomaton *automaton = new DeterministicFiniteAutomaton();
	automaton->addState("q0");

	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);

	delete automaton;
}

TEST_F(DFA_Test, SetCurrentState_UpdatesCurrentState) {
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->getCurrentState() == "q1");
}

TEST_F(DFA_Test, SetCurrentState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setCurrentState("q3"), StateNotFoundException);
}

TEST_F(DFA_Test, GetState_GetsState) {
	FAState state = automaton->getState("q0");
	EXPECT_TRUE(state.getLabel() == "q0");
}

TEST_F(DFA_Test, GetState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->getState("q3"), StateNotFoundException);
}

TEST_F(DFA_Test, GetStates_GetsStates) {
	std::vector<FAState> states = automaton->getStates();
	EXPECT_TRUE(states.size() == 2);

	bool foundQ0 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q0") {
			foundQ0 = true;
		}
	}
	EXPECT_TRUE(foundQ0);
}

TEST_F(DFA_Test, GetStates_GetsStatesAfterAddStateCall) {
	automaton->addState("q3");
	std::vector<FAState> states = automaton->getStates();

	bool foundQ3 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q3") {
			foundQ3 = true;
		}
	}
	EXPECT_TRUE(foundQ3);
}

TEST_F(DFA_Test, GetStates_GetsStatesAfterUpdateStateLabelCall) {
	automaton->updateStateLabel("q0", "q3");
	std::vector<FAState> states = automaton->getStates();

	bool foundQ3 = false;
	for (auto &state : states) {
		if (state.getLabel() == "q3") {
			foundQ3 = true;
		}
	}
	EXPECT_TRUE(foundQ3);
}

TEST_F(DFA_Test, RemoveState_RemovesState) {
	EXPECT_TRUE(automaton->getStates().size() == 2);

	automaton->removeState("q1");
	EXPECT_FALSE(automaton->stateExists("q1"));

	EXPECT_TRUE(automaton->getStates().size() == 1);
}

TEST_F(DFA_Test, RemoveState_RemovesStartState) {
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DFA_Test, RemoveState_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, RemoveState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeState("q3"), StateNotFoundException);
}

TEST_F(DFA_Test, RemoveState_RemovesTransitionsWithStateAsToIfNotStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0");
	automaton->removeState("q0", false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(FATransition::generateTransitionKey("q1", "q0", "0")));
}

TEST_F(DFA_Test, RemoveState_ThrowsIfStateIsUsedInTransitionsIfStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0");
	EXPECT_THROW(automaton->removeState("q0", true), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, RemoveStates_RemovesMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->getStates().size() == 4);

	automaton->removeStates({"q1", "q2"});
	EXPECT_FALSE(automaton->stateExists("q1"));
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));

	EXPECT_TRUE(automaton->getStates().size() == 2);
}

TEST_F(DFA_Test, RemoveStates_RemovesStartState) {
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DFA_Test, RemoveStates_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, RemoveStates_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(DFA_Test, RemoveStates_RemovesTransitionsWithStateAsToIfNotStrict) {
	automaton->addTransition("q1", "q0", "0");
	automaton->removeStates({"q0"}, false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(FATransition::generateTransitionKey("q1", "q0", "0")));
}

TEST_F(DFA_Test, RemoveStates_ThrowsIfStateIsUsedInTransitionsIfStrict) {
	automaton->setCurrentState("q0");
	automaton->addTransition("q1", "q0", "0");
	EXPECT_THROW(automaton->removeStates({"q0"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, ClearStates_RemovesAllStates) {
	EXPECT_TRUE(automaton->getStates().size() == 2);
	automaton->clearStates();
	EXPECT_TRUE(automaton->getStates().empty());
}

TEST_F(DFA_Test, ClearStates_RemovesCurrentState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, ClearStates_RemovesStartState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DFA_Test, GetInputAlphabet_GetsAlphabet) {
	std::vector<std::string> alphabet = automaton->getInputAlphabet();
	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DFA_Test, SetInputAlphabet_OverridesAlphabet) {
	automaton->setInputAlphabet({"a", "b", "c"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 3);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "a") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "b") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "c") != alphabet.end());
}

TEST_F(DFA_Test, SetInputAlphabet_ThrowsForEpsilon) {
	EXPECT_THROW(automaton->setInputAlphabet({"a", "", "c"}), InvalidAlphabetException);
}

TEST_F(DFA_Test, SetInputAlphabet_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0");
	EXPECT_THROW(automaton->setInputAlphabet({"a", "b"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, SetInputAlphabet_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0");
	automaton->setInputAlphabet({"a", "b"}, false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(FATransition::generateTransitionKey("q1", "q0", "0")));
}

TEST_F(DFA_Test, AddInputAlphabet_AddsNewSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 4);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
}

TEST_F(DFA_Test, AddInputAlphabet_ThrowsForEpsilon) {
	EXPECT_THROW(automaton->addInputAlphabet({"a", "", "c"}), InvalidAlphabetException);
}

TEST_F(DFA_Test, AddInputAlphabet_IgnoresDuplicates) {
	automaton->addInputAlphabet({"1", "0", "1"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbol_RemovesSymbol) {
	automaton->removeInputAlphabetSymbol("0");
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 1);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbol_ThrowsIfNotFound) {
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("x"), InputAlphabetSymbolNotFoundException);
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbol_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0");
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("0", true), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbol_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0");
	automaton->removeInputAlphabetSymbol("0", false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(FATransition::generateTransitionKey("q1", "q0", "0")));
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbols_RemovesMultipleSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	automaton->removeInputAlphabetSymbols({"0", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbols_ThrowsIfAnySymbolNotFound) {
	automaton->addInputAlphabet({"2"});
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"1", "x"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbols_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0");
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"0"}, true), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, RemoveInputAlphabetSymbols_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0");
	automaton->removeInputAlphabetSymbols({"0"}, false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(FATransition::generateTransitionKey("q1", "q0", "0")));
}

TEST_F(DFA_Test, ClearInputAlphabet_RemovesAllSymbols) {
	automaton->clearInputAlphabet();
	EXPECT_TRUE(automaton->getInputAlphabet().empty());
}

TEST_F(DFA_Test, ClearInputAlphabet_ThrowsForUsedSymbolIfStrict) {
	automaton->addTransition("q1", "q0", "0");
	EXPECT_THROW(automaton->clearInputAlphabet(true), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, ClearInputAlphabet_RemovesTransitionsWithOldSymbolIfNotStrict) {
	automaton->addTransition("q1", "q0", "0");
	automaton->clearInputAlphabet(false);
	EXPECT_FALSE(automaton->getState("q1").transitionExists(FATransition::generateTransitionKey("q1", "q0", "0")));
}

TEST_F(DFA_Test, GetStartState_ReturnsCorrectState) {
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(DFA_Test, GetStartState_ThrowsIfNotSet) {
	automaton->removeState("q0");
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(DFA_Test, SetStartState_SetsNewStartState) {
	automaton->addState("q2");
	automaton->setStartState("q2");
	EXPECT_EQ(automaton->getStartState(), "q2");
}

TEST_F(DFA_Test, SetStartState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(DFA_Test, SetStartState_SetsCurrentStateIfEmpty) {
	automaton->clearStates();
	automaton->addState("q4");
	automaton->setStartState("q4");
	EXPECT_EQ(automaton->getStartState(), "q4");
	EXPECT_EQ(automaton->getCurrentState(), "q4");
}

TEST_F(DFA_Test, SetStartState_DoesNotOverrideCurrentStateIfSet) {
	automaton->addState("q2");
	automaton->setCurrentState("q1");
	automaton->setStartState("q2");

	EXPECT_EQ(automaton->getStartState(), "q2");
	EXPECT_EQ(automaton->getCurrentState(), "q1");
}

TEST_F(DFA_Test, AddTransition_AddsValidTransition) {
	automaton->addTransition("q0", "q1", "0");

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

TEST_F(DFA_Test, AddTransition_ThrowsIfFromStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("qX", "q1", "0"), StateNotFoundException);
}

TEST_F(DFA_Test, AddTransition_ThrowsIfToStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("q0", "qX", "0"), StateNotFoundException);
}

TEST_F(DFA_Test, AddTransition_ThrowsIfInputNotInAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "q1", "2"), InvalidTransitionException);
}

TEST_F(DFA_Test, AddTransition_ThrowsIfEpsilonInDFA) {
	EXPECT_THROW(automaton->addTransition("q0", "q1", ""), InvalidTransitionException);
}

TEST_F(DFA_Test, AddTransition_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	EXPECT_THROW(automaton->addTransition("q0", "q2", "0"), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, AddTransition_AllowsDifferentInputs) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q0", "1");

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

TEST_F(DFA_Test, AddTransition_DoesNotInvalidateExistingTransitions) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q0", "1");

	const auto &q0Transitions = automaton->getState("q0").getTransitions();
	const auto &q1Transitions = automaton->getState("q1").getTransitions();

	EXPECT_EQ(q0Transitions.size(), 1);
	EXPECT_EQ(q1Transitions.size(), 1);

	EXPECT_EQ(q0Transitions[0].getInput(), "0");
	EXPECT_EQ(q1Transitions[0].getInput(), "1");
}

TEST_F(DFA_Test, UpdateTransitionInput_ValidUpdate) {
	automaton->addTransition("q0", "q1", "0");
	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

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

TEST_F(DFA_Test, UpdateTransitionInput_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");
	automaton->updateTransitionInput(transitionKey, "2");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q2", "1")));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q1", "2")));
}

TEST_F(DFA_Test, UpdateTransitionInput_ThrowsIfInputNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(DFA_Test, UpdateTransitionInput_ThrowsIfTransitionExists) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q1", "1");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "1"), InvalidTransitionException);
}

TEST_F(DFA_Test, UpdateTransitionInput_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionInput("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(DFA_Test, UpdateTransitionInput_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q2", "1");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "0"), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, UpdateTransitionFromState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	automaton->updateTransitionFromState(transitionKey, "q2");

	EXPECT_TRUE(automaton->getState("q2").transitionExists(FATransition::generateTransitionKey("q2", "q1", "0")));
	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DFA_Test, UpdateTransitionFromState_ThrowsIfDuplicate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q2", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q2"), InvalidTransitionException);
}

TEST_F(DFA_Test, UpdateTransitionFromState_ThrowsIfTransitionNotFound) {
	automaton->addState("q2");
	EXPECT_THROW(automaton->updateTransitionFromState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(DFA_Test, UpdateTransitionFromState_ThrowsIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q1", "q2", "0");
	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q0"), InvalidAutomatonDefinitionException);
}

TEST_F(DFA_Test, UpdateTransitionToState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

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

TEST_F(DFA_Test, UpdateTransitionToState_ThrowsIfStateNotFound) {
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionToState(transitionKey, "qX"), StateNotFoundException);
}

TEST_F(DFA_Test, UpdateTransitionToState_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionToState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(DFA_Test, UpdateTransitionToState_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q1", "1");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");
	automaton->updateTransitionToState(transitionKey, "q2");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q1", "1")));
}

TEST_F(DFA_Test, RemoveTransition_RemovesValidTransition) {
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");
	EXPECT_TRUE(automaton->getState("q0").transitionExists(transitionKey));

	automaton->removeTransition(transitionKey);

	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(DFA_Test, RemoveTransition_ThrowsIfTransitionNotFound) {
	std::string invalidTransitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->removeTransition(invalidTransitionKey), TransitionNotFoundException);
}

TEST_F(DFA_Test, ClearTransitionsBetween_RemovesTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");

	automaton->clearTransitionsBetween("q0", "q1");

	EXPECT_FALSE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q1", "0")));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q2", "1")));
}

TEST_F(DFA_Test, ClearTransitionsBetween_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearTransitionsBetween("qX", "qY"), StateNotFoundException);
}

TEST_F(DFA_Test, ClearStateTransitions_RemovesAllTransitionsFromState) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");

	automaton->clearStateTransitions("q0");

	EXPECT_TRUE(automaton->getState("q0").getTransitions().empty());
}

TEST_F(DFA_Test, ClearStateTransitions_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearStateTransitions("qX"), StateNotFoundException);
}

TEST_F(DFA_Test, ClearTransitions_RemovesAllTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");
	automaton->addTransition("q1", "q0", "0");

	automaton->clearTransitions();

	for (auto state : automaton->getStates()) {
		EXPECT_TRUE(state.getTransitions().empty());
	}
}

TEST_F(DFA_Test, AddAcceptState_ValidState) {
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_TRUE(acceptStates[0].getIsAccept());
}

TEST_F(DFA_Test, AddAcceptState_DuplicateAcceptState) {
	automaton->addAcceptState("q1");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(DFA_Test, AddAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->addAcceptState("qX"), StateNotFoundException);
}

TEST_F(DFA_Test, AddAcceptStates_AddsMultipleAcceptStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_EQ(acceptStates[1].getKey(), "q2");
}

TEST_F(DFA_Test, AddAcceptStates_ThrowsForMissingStates) {
	EXPECT_THROW(automaton->addAcceptStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(DFA_Test, RemoveAcceptState_ValidState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DFA_Test, RemoveAcceptState_NonAcceptState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q0");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(DFA_Test, RemoveAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeAcceptState("qX"), StateNotFoundException);
}

TEST_F(DFA_Test, RemoveAcceptStates_RemovesMultipleAcceptStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 2);

	automaton->removeAcceptStates({"q1", "q2"});
	acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 0);
}

TEST_F(DFA_Test, RemoveAcceptStates_ThrowsForMissingStates) {
	automaton->addState("q2");
	automaton->addAcceptStates({"q1", "q2"});

	auto acceptStates = automaton->getAcceptStates();

	EXPECT_THROW(automaton->removeAcceptStates({"q2", "q3"}), StateNotFoundException);
}

TEST_F(DFA_Test, ClearAcceptStates_RemovesAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DFA_Test, ClearAcceptStates_NoAcceptStates) {
	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DFA_Test, GetAcceptStates_ReturnsAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();

	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_TRUE((acceptStates[0].getKey() == "q0" && acceptStates[1].getKey() == "q1") ||
	            (acceptStates[0].getKey() == "q1" && acceptStates[1].getKey() == "q0"));
}

TEST_F(DFA_Test, GetAcceptStates_EmptyIfNoAcceptStates) {
	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(DFA_Test, Reset_SetsCurrentStateToStartState) {
	automaton->addAcceptState("q0");
	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
}

TEST_F(DFA_Test, Reset_HandlesAcceptStartState) {
	automaton->addAcceptState("q0");
	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
	EXPECT_FALSE(automaton->getAcceptStates().empty());
}

TEST_F(DFA_Test, IsAccepting_ShouldAcceptIfStartStateIsAccept) {
	automaton->addAcceptState("q0");
	EXPECT_TRUE(automaton->isAccepting());
}

TEST_F(DFA_Test, IsAccepting_ShouldAcceptIfCurrentStateIsAccept) {
	automaton->addAcceptState("q1");
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->isAccepting());
}

TEST_F(DFA_Test, ProcessInput_ShouldAcceptValidEmptyInput) {
	automaton->addAcceptState("q0");
	automaton->addState("q2");
	automaton->addState("q3");

	automaton->addTransition("q0", "q1", "1");
	automaton->addTransition("q0", "q2", "0");

	automaton->addTransition("q1", "q0", "1");
	automaton->addTransition("q1", "q3", "0");

	automaton->addTransition("q2", "q0", "0");
	automaton->addTransition("q2", "q3", "1");

	automaton->addTransition("q3", "q1", "0");
	automaton->addTransition("q3", "q2", "1");

	EXPECT_TRUE(automaton->isAccepting());
	EXPECT_TRUE(automaton->processInput());
}

TEST_F(DFA_Test, ProcessInput_ShouldAcceptValidSequence) {
	automaton->addAcceptState("q0");
	automaton->addState("q2");
	automaton->addState("q3");

	automaton->addTransition("q0", "q1", "1");
	automaton->addTransition("q0", "q2", "0");

	automaton->addTransition("q1", "q0", "1");
	automaton->addTransition("q1", "q3", "0");

	automaton->addTransition("q2", "q0", "0");
	automaton->addTransition("q2", "q3", "1");

	automaton->addTransition("q3", "q1", "0");
	automaton->addTransition("q3", "q2", "1");

	automaton->setInput({"0", "1", "0", "1"});

	automaton->processInput();
	EXPECT_TRUE(automaton->getInputHead() == 1);
	EXPECT_TRUE(automaton->getCurrentState() == "q2");

	automaton->processInput();
	EXPECT_TRUE(automaton->getInputHead() == 2);
	EXPECT_TRUE(automaton->getCurrentState() == "q3");

	automaton->processInput();
	EXPECT_TRUE(automaton->getInputHead() == 3);
	EXPECT_TRUE(automaton->getCurrentState() == "q1");

	automaton->processInput();
	EXPECT_TRUE(automaton->getInputHead() == 4);
	EXPECT_TRUE(automaton->getCurrentState() == "q0");

	EXPECT_TRUE(automaton->isAccepting());
}

TEST_F(DFA_Test, ProcessInput_ShouldStayAcceptingAfterInputIsConsumed) {
	automaton->addAcceptState("q0");
	automaton->addState("q2");
	automaton->addState("q3");

	automaton->addTransition("q0", "q1", "1");
	automaton->addTransition("q0", "q2", "0");

	automaton->addTransition("q1", "q0", "1");
	automaton->addTransition("q1", "q3", "0");

	automaton->addTransition("q2", "q0", "0");
	automaton->addTransition("q2", "q3", "1");

	automaton->addTransition("q3", "q1", "0");
	automaton->addTransition("q3", "q2", "1");

	EXPECT_TRUE(automaton->isAccepting());

	automaton->processInput();
	EXPECT_TRUE(automaton->isAccepting());
}

TEST_F(DFA_Test, ProcessInputRejectIfNoTransitionFound) {
	automaton->addAcceptState("q0");
	automaton->setInput({"0"});

	EXPECT_FALSE(automaton->processInput());
}

TEST_F(DFA_Test, ProcessInput_ShouldProcessInputsForFerrymanProblem) {
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

	automaton->setInputAlphabet({"g", "m", "w", "c"});

	automaton->addTransition("MWGC-0", "WC-MG", "g");
	automaton->addTransition("WC-MG", "MWGC-0", "g");

	automaton->addTransition("WC-MG", "MWC-G", "m");
	automaton->addTransition("MWC-G", "WC-MG", "m");

	automaton->addTransition("MWC-G", "C-MWG", "w");
	automaton->addTransition("MWC-G", "W-MGC", "c");

	automaton->addTransition("C-MWG", "MWC-G", "w");
	automaton->addTransition("C-MWG", "MGC-W", "g");

	automaton->addTransition("W-MGC", "MWC-G", "c");
	automaton->addTransition("W-MGC", "MWG-C", "g");

	automaton->addTransition("MGC-W", "C-MWG", "g");
	automaton->addTransition("MGC-W", "G-MWC", "c");

	automaton->addTransition("MWG-C", "W-MGC", "g");
	automaton->addTransition("MWG-C", "G-MWC", "w");

	automaton->addTransition("G-MWC", "MGC-W", "c");
	automaton->addTransition("G-MWC", "MWG-C", "w");
	automaton->addTransition("G-MWC", "MG-WC", "m");

	automaton->addTransition("MG-WC", "G-MWC", "m");
	automaton->addTransition("MG-WC", "0-MWGC", "g");

	automaton->addTransition("0-MWGC", "MG-WC", "g");

	automaton->addAcceptState("0-MWGC");
	automaton->setInput({"g", "m", "w", "g", "c", "m", "g"});

	// Start at the initial state
	automaton->setCurrentState("MWGC-0");
	EXPECT_EQ(automaton->getCurrentState(), "MWGC-0");

	// Take the goat to the other side
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "WC-MG");

	// Return alone
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "MWC-G");

	// Take the wolf to the other side
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "C-MWG");

	// Return with the goat
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "MGC-W");

	// Take the cabbage to the other side
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "G-MWC");

	// Return alone
	automaton->processInput();
	EXPECT_EQ(automaton->getCurrentState(), "MG-WC");

	// Take the goat to the other side
	EXPECT_TRUE(automaton->processInput());
	EXPECT_TRUE(automaton->isAccepting());
	EXPECT_EQ(automaton->getCurrentState(), "0-MWGC");
}

TEST_F(DFA_Test, Simulate_ShouldThrowExceptionForSimulationWithoutStartState) {
	DeterministicFiniteAutomaton *automaton = new DeterministicFiniteAutomaton();
	automaton->setInputAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(DFA_Test, Simulate_ValidInputAccepts) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));
}

TEST_F(DFA_Test, Simulate_InvalidInputReject) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");

	EXPECT_FALSE(automaton->simulate({"0", "0"}));
}

TEST_F(DFA_Test, Simulate_RejectsIfSimulationDepthExceeded) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q0", "1");

	automaton->setStartState("q0");

	std::vector<std::string> input = {"0", "1", "0", "1", "0", "1"};

	EXPECT_FALSE(automaton->simulate(input, 3));
}

TEST_F(DFA_Test, Simulate_EmptyInputStaysAtStartState) {
	automaton->addAcceptState("q0");
	EXPECT_TRUE(automaton->simulate({}));
}

TEST_F(DFA_Test, Simulate_RejectIfNoTransitionFound) {
	automaton->addAcceptState("q0");
	EXPECT_FALSE(automaton->simulate({"0"}));
}

TEST_F(DFA_Test, Simulate_ShouldSimulateFerrymanProblem) {
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

	automaton->setInputAlphabet({"g", "m", "w", "c"});

	automaton->addTransition("MWGC-0", "WC-MG", "g");
	automaton->addTransition("WC-MG", "MWGC-0", "g");

	automaton->addTransition("WC-MG", "MWC-G", "m");
	automaton->addTransition("MWC-G", "WC-MG", "m");

	automaton->addTransition("MWC-G", "C-MWG", "w");
	automaton->addTransition("MWC-G", "W-MGC", "c");

	automaton->addTransition("C-MWG", "MWC-G", "w");
	automaton->addTransition("C-MWG", "MGC-W", "g");

	automaton->addTransition("W-MGC", "MWC-G", "c");
	automaton->addTransition("W-MGC", "MWG-C", "g");

	automaton->addTransition("MGC-W", "C-MWG", "g");
	automaton->addTransition("MGC-W", "G-MWC", "c");

	automaton->addTransition("MWG-C", "W-MGC", "g");
	automaton->addTransition("MWG-C", "G-MWC", "w");

	automaton->addTransition("G-MWC", "MGC-W", "c");
	automaton->addTransition("G-MWC", "MWG-C", "w");
	automaton->addTransition("G-MWC", "MG-WC", "m");

	automaton->addTransition("MG-WC", "G-MWC", "m");
	automaton->addTransition("MG-WC", "0-MWGC", "g");

	automaton->addTransition("0-MWGC", "MG-WC", "g");

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