#include "pch.h"

class NFA_Test : public ::testing::Test {
  protected:
	void SetUp() override {
		automaton = new NonDeterministicFiniteAutomaton();

		automaton->addState("q0");
		automaton->addState("q1");
		automaton->setStartState("q0");
		automaton->setInputAlphabet({"0", "1"});
	}

	void TearDown() override {
		delete automaton;
	}

	NonDeterministicFiniteAutomaton *automaton;
};

TEST_F(NFA_Test, StateExists_ReturnsTrueWhenStateExists) {
	EXPECT_TRUE(automaton->stateExists("q0"));
	EXPECT_TRUE(automaton->stateExists("q1"));
}

TEST_F(NFA_Test, StateExists_ReturnsFalseWhenStateDoesNotExist) {
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_FALSE(automaton->stateExists(""));
}

TEST_F(NFA_Test, InputAlphabetSymbolExists_ReturnsTrueForValidSymbol) {
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("0"));
	EXPECT_TRUE(automaton->inputAlphabetSymbolExists("1"));
}

TEST_F(NFA_Test, InputAlphabetSymbolExists_ReturnsFalseForInvalidSymbol) {
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("2"));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists(""));
	EXPECT_FALSE(automaton->inputAlphabetSymbolExists("abc"));
}

TEST_F(NFA_Test, GetPossibleCurrentStates_GetsStartStateIfNoCurrentState) {
	EXPECT_TRUE(automaton->getPossibleCurrentStates()[0] == "q0");
}

TEST_F(NFA_Test, AddState_AddsNewStateSuccessfully) {
	automaton->addState("q2");
	EXPECT_TRUE(automaton->stateExists("q2"));
}

TEST_F(NFA_Test, AddState_ThrowsForDuplicateState) {
	EXPECT_THROW(automaton->addState("q0"), InvalidAutomatonDefinitionException);
	EXPECT_THROW(automaton->addState("q1"), InvalidAutomatonDefinitionException);
}

TEST_F(NFA_Test, AddState_AddsMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));
}

TEST_F(NFA_Test, UpdateStateLabel_UpdatesStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->stateExists("q3"));
	automaton->updateStateLabel("q3", "q4");
	EXPECT_TRUE(automaton->stateExists("q4"));
}

TEST_F(NFA_Test, UpdateStateLabel_UpdatesStartStateLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->getStartState() == "q3");
}

TEST_F(NFA_Test, UpdateStateLabel_UpdatesPossibleCurrentStatesLabel) {
	automaton->updateStateLabel("q0", "q3");
	EXPECT_TRUE(automaton->getPossibleCurrentStates()[0] == "q3");
}

TEST_F(NFA_Test, GetCurrentState_GetsStartStateIfNoCurrentStateSet) {
	EXPECT_TRUE(automaton->getCurrentState() == "q0");
}

TEST_F(NFA_Test, GetCurrentState_ThrowsIfNoStartStateOrCurrentStateSet) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();
	automaton->addState("q0");

	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);

	delete automaton;
}

TEST_F(NFA_Test, SetCurrentState_UpdatesCurrentState) {
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->getCurrentState() == "q1");
}

TEST_F(NFA_Test, SetCurrentState_UpdatesPossibleCurrentState) {
	automaton->setCurrentState("q1");
	EXPECT_TRUE(automaton->getPossibleCurrentStates().size() == 1);
	EXPECT_TRUE(automaton->getPossibleCurrentStates()[0] == "q1");
}

TEST_F(NFA_Test, SetCurrentState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setCurrentState("q3"), StateNotFoundException);
}

TEST_F(NFA_Test, GetState_GetsState) {
	FAState state = automaton->getState("q0");
	EXPECT_TRUE(state.getLabel() == "q0");
}

TEST_F(NFA_Test, GetState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->getState("q3"), StateNotFoundException);
}

TEST_F(NFA_Test, GetStates_GetsStates) {
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

TEST_F(NFA_Test, GetStates_GetsStatesAfterAddStateCall) {
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

TEST_F(NFA_Test, GetStates_GetsStatesAfterUpdateStateLabelCall) {
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

TEST_F(NFA_Test, RemoveState_RemovesState) {
	EXPECT_TRUE(automaton->getStates().size() == 2);

	automaton->removeState("q1");
	EXPECT_FALSE(automaton->stateExists("q1"));

	EXPECT_TRUE(automaton->getStates().size() == 1);
}
TEST_F(NFA_Test, RemoveState_RemovesStartState) {
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(NFA_Test, RemoveState_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeState("q0");
	EXPECT_FALSE(automaton->stateExists("q0"));
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(NFA_Test, RemoveState_RemovesFromCurrentPossibleStates) {
	automaton->setCurrentState("q1");
	automaton->removeState("q1");

	EXPECT_TRUE(automaton->getPossibleCurrentStates().empty());
}

TEST_F(NFA_Test, RemoveState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeState("q3"), StateNotFoundException);
}

TEST_F(NFA_Test, RemoveStates_RemovesMultipleStates) {
	automaton->addState("q2");
	automaton->addState("q3");
	EXPECT_TRUE(automaton->getStates().size() == 4);

	automaton->removeStates({"q1", "q2"});
	EXPECT_FALSE(automaton->stateExists("q1"));
	EXPECT_FALSE(automaton->stateExists("q2"));
	EXPECT_TRUE(automaton->stateExists("q3"));

	EXPECT_TRUE(automaton->getStates().size() == 2);
}

TEST_F(NFA_Test, RemoveStates_RemovesStartState) {
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(NFA_Test, RemoveStates_RemovesCurrentState) {
	automaton->setCurrentState("q0");
	automaton->removeStates({"q0", "q1"});
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(NFA_Test, RemoveStates_RemovesFromCurrentPossibleStates) {
	automaton->setCurrentState("q0");
	automaton->removeStates({"q0", "q1"});
	EXPECT_TRUE(automaton->getPossibleCurrentStates().empty());
}

TEST_F(NFA_Test, RemoveStates_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeStates({"q1", "q2"}), StateNotFoundException);
}

TEST_F(NFA_Test, ClearStates_RemovesAllStates) {
	EXPECT_TRUE(automaton->getStates().size() == 2);
	automaton->clearStates();
	EXPECT_TRUE(automaton->getStates().empty());
}

TEST_F(NFA_Test, ClearStates_RemovesCurrentState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getCurrentState(), InvalidAutomatonDefinitionException);
}

TEST_F(NFA_Test, ClearStates_RemovesFromCurrentPossibleStates) {
	automaton->setCurrentState("q0");
	automaton->clearStates();
	EXPECT_TRUE(automaton->getPossibleCurrentStates().empty());
}

TEST_F(NFA_Test, ClearStates_RemovesStartState) {
	automaton->clearStates();
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(NFA_Test, GetInputAlphabet_GetsAlphabet) {
	std::vector<std::string> alphabet = automaton->getInputAlphabet();
	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(NFA_Test, SetInputAlphabet_OverridesAlphabet) {
	automaton->setInputAlphabet({"a", "b", "c"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 3);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "a") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "b") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "c") != alphabet.end());
}

TEST_F(NFA_Test, SetInputAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->setInputAlphabet({"a", "", "c"}));
}

TEST_F(NFA_Test, AddInputAlphabet_AddsNewSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 4);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
}

TEST_F(NFA_Test, AddInputAlphabet_NoThrowForEpsilon) {
	EXPECT_NO_THROW(automaton->addInputAlphabet({"a", "", "c"}), InvalidAlphabetException);
}

TEST_F(NFA_Test, AddInputAlphabet_IgnoresDuplicates) {
	automaton->addInputAlphabet({"1", "0", "1"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(NFA_Test, RemoveInputAlphabetSymbol_RemovesSymbol) {
	automaton->removeInputAlphabetSymbol("0");
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 1);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
}

TEST_F(NFA_Test, RemoveInputAlphabetSymbol_ThrowsIfNotFound) {
	EXPECT_THROW(automaton->removeInputAlphabetSymbol("x"), InputAlphabetSymbolNotFoundException);
}

TEST_F(NFA_Test, RemoveInputAlphabetSymbols_RemovesMultipleSymbols) {
	automaton->addInputAlphabet({"2", "3"});
	automaton->removeInputAlphabetSymbols({"0", "3"});
	std::vector<std::string> alphabet = automaton->getInputAlphabet();

	EXPECT_EQ(alphabet.size(), 2);
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "0") != alphabet.end());
	EXPECT_FALSE(std::find(alphabet.begin(), alphabet.end(), "3") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "1") != alphabet.end());
	EXPECT_TRUE(std::find(alphabet.begin(), alphabet.end(), "2") != alphabet.end());
}

TEST_F(NFA_Test, RemoveInputAlphabetSymbols_ThrowsIfAnySymbolNotFound) {
	automaton->addInputAlphabet({"2"});
	EXPECT_THROW(automaton->removeInputAlphabetSymbols({"1", "x"}), InputAlphabetSymbolNotFoundException);
}

TEST_F(NFA_Test, ClearInputAlphabet_RemovesAllSymbols) {
	automaton->clearInputAlphabet();
	EXPECT_TRUE(automaton->getInputAlphabet().empty());
}

TEST_F(NFA_Test, GetStartState_ReturnsCorrectState) {
	EXPECT_EQ(automaton->getStartState(), "q0");
}

TEST_F(NFA_Test, GetStartState_ThrowsIfNotSet) {
	automaton->removeState("q0");
	EXPECT_THROW(automaton->getStartState(), InvalidStartStateException);
}

TEST_F(NFA_Test, SetStartState_SetsNewStartState) {
	automaton->addState("q2");
	automaton->setStartState("q2");
	EXPECT_EQ(automaton->getStartState(), "q2");
}

TEST_F(NFA_Test, SetStartState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->setStartState("q3"), StateNotFoundException);
}

TEST_F(NFA_Test, SetStartState_SetsCurrentStateIfEmpty) {
	automaton->clearStates();
	automaton->addState("q4");
	automaton->setStartState("q4");
	EXPECT_EQ(automaton->getStartState(), "q4");
	EXPECT_EQ(automaton->getCurrentState(), "q4");
}

TEST_F(NFA_Test, SetStartState_DoesNotOverrideCurrentStateIfSet) {
	automaton->addState("q2");
	automaton->setCurrentState("q1");
	automaton->setStartState("q2");

	EXPECT_EQ(automaton->getStartState(), "q2");
	EXPECT_EQ(automaton->getCurrentState(), "q1");
}

TEST_F(NFA_Test, AddTransition_AddsValidTransition) {
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

TEST_F(NFA_Test, AddTransition_ThrowsIfFromStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("qX", "q1", "0"), StateNotFoundException);
}

TEST_F(NFA_Test, AddTransition_ThrowsIfToStateDoesNotExist) {
	EXPECT_THROW(automaton->addTransition("q0", "qX", "0"), StateNotFoundException);
}

TEST_F(NFA_Test, AddTransition_ThrowsIfInputNotInAlphabet) {
	EXPECT_THROW(automaton->addTransition("q0", "q1", "2"), InvalidTransitionException);
}

TEST_F(NFA_Test, AddTransition_NoThrowIfEpsilonInNFA) {
	EXPECT_NO_THROW(automaton->addTransition("q0", "q1", ""));
}

TEST_F(NFA_Test, AddTransition_NoThrowIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	EXPECT_NO_THROW(automaton->addTransition("q0", "q2", "0"));
}

TEST_F(NFA_Test, AddTransition_AllowsDifferentInputs) {
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

TEST_F(NFA_Test, AddTransition_DoesNotInvalidateExistingTransitions) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q0", "1");

	const auto &q0Transitions = automaton->getState("q0").getTransitions();
	const auto &q1Transitions = automaton->getState("q1").getTransitions();

	EXPECT_EQ(q0Transitions.size(), 1);
	EXPECT_EQ(q1Transitions.size(), 1);

	EXPECT_EQ(q0Transitions[0].getInput(), "0");
	EXPECT_EQ(q1Transitions[0].getInput(), "1");
}

TEST_F(NFA_Test, UpdateTransitionInput_ValidUpdate) {
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

TEST_F(NFA_Test, UpdateTransitionInput_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");
	automaton->addInputAlphabet({"2"});

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");
	automaton->updateTransitionInput(transitionKey, "2");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q2", "1")));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q1", "2")));
}

TEST_F(NFA_Test, UpdateTransitionInput_ThrowsIfInputNotInAlphabet) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "2"), InvalidTransitionException);
}

TEST_F(NFA_Test, UpdateTransitionInput_ThrowsIfTransitionExists) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q1", "1");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionInput(transitionKey, "1"), InvalidTransitionException);
}

TEST_F(NFA_Test, UpdateTransitionInput_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionInput("invalidKey", "1"), TransitionNotFoundException);
}

TEST_F(NFA_Test, UpdateTransitionInput_NoThrowIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q2", "1");

	EXPECT_NO_THROW(automaton->updateTransitionInput(transitionKey, "0"));
}

TEST_F(NFA_Test, UpdateTransitionFromState_ValidUpdate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	automaton->updateTransitionFromState(transitionKey, "q2");

	EXPECT_TRUE(automaton->getState("q2").transitionExists(FATransition::generateTransitionKey("q2", "q1", "0")));
	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(NFA_Test, UpdateTransitionFromState_ThrowsIfDuplicate) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q2", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionFromState(transitionKey, "q2"), InvalidTransitionException);
}

TEST_F(NFA_Test, UpdateTransitionFromState_ThrowsIfTransitionNotFound) {
	automaton->addState("q2");
	EXPECT_THROW(automaton->updateTransitionFromState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(NFA_Test, UpdateTransitionFromState_NoThrowIfNonDeterministic) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q1", "q2", "0");
	EXPECT_NO_THROW(automaton->updateTransitionFromState(transitionKey, "q0"));
}

TEST_F(NFA_Test, UpdateTransitionToState_ValidUpdate) {
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

TEST_F(NFA_Test, UpdateTransitionToState_ThrowsIfStateNotFound) {
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->updateTransitionToState(transitionKey, "qX"), StateNotFoundException);
}

TEST_F(NFA_Test, UpdateTransitionToState_ThrowsIfTransitionNotFound) {
	EXPECT_THROW(automaton->updateTransitionToState("invalidKey", "q2"), TransitionNotFoundException);
}

TEST_F(NFA_Test, UpdateTransitionToState_DoesNotInvalidateOtherTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q1", "1");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");
	automaton->updateTransitionToState(transitionKey, "q2");

	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q1", "1")));
}

TEST_F(NFA_Test, RemoveTransition_RemovesValidTransition) {
	automaton->addTransition("q0", "q1", "0");

	std::string transitionKey = FATransition::generateTransitionKey("q0", "q1", "0");
	EXPECT_TRUE(automaton->getState("q0").transitionExists(transitionKey));

	automaton->removeTransition(transitionKey);

	EXPECT_FALSE(automaton->getState("q0").transitionExists(transitionKey));
}

TEST_F(NFA_Test, RemoveTransition_ThrowsIfTransitionNotFound) {
	std::string invalidTransitionKey = FATransition::generateTransitionKey("q0", "q1", "0");

	EXPECT_THROW(automaton->removeTransition(invalidTransitionKey), TransitionNotFoundException);
}

TEST_F(NFA_Test, ClearTransitionsBetween_RemovesTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");

	automaton->clearTransitionsBetween("q0", "q1");

	EXPECT_FALSE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q1", "0")));
	EXPECT_TRUE(automaton->getState("q0").transitionExists(FATransition::generateTransitionKey("q0", "q2", "1")));
}

TEST_F(NFA_Test, ClearTransitionsBetween_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearTransitionsBetween("qX", "qY"), StateNotFoundException);
}

TEST_F(NFA_Test, ClearStateTransitions_RemovesAllTransitionsFromState) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");

	automaton->clearStateTransitions("q0");

	EXPECT_TRUE(automaton->getState("q0").getTransitions().empty());
}

TEST_F(NFA_Test, ClearStateTransitions_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->clearStateTransitions("qX"), StateNotFoundException);
}

TEST_F(NFA_Test, ClearTransitions_RemovesAllTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "1");
	automaton->addTransition("q1", "q0", "0");

	automaton->clearTransitions();

	for (auto &state : automaton->getStates()) {
		EXPECT_TRUE(state.getTransitions().empty());
	}
}

TEST_F(NFA_Test, AddAcceptState_ValidState) {
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
	EXPECT_TRUE(acceptStates[0].getIsAccept());
}

TEST_F(NFA_Test, AddAcceptState_DuplicateAcceptState) {
	automaton->addAcceptState("q1");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(NFA_Test, AddAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->addAcceptState("qX"), StateNotFoundException);
}

TEST_F(NFA_Test, RemoveAcceptState_ValidState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(NFA_Test, RemoveAcceptState_NonAcceptState) {
	automaton->addAcceptState("q1");
	automaton->removeAcceptState("q0");

	auto acceptStates = automaton->getAcceptStates();
	ASSERT_EQ(acceptStates.size(), 1);
	EXPECT_EQ(acceptStates[0].getKey(), "q1");
}

TEST_F(NFA_Test, RemoveAcceptState_ThrowsIfStateNotFound) {
	EXPECT_THROW(automaton->removeAcceptState("qX"), StateNotFoundException);
}

TEST_F(NFA_Test, ClearAcceptStates_RemovesAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(NFA_Test, ClearAcceptStates_NoAcceptStates) {
	automaton->clearAcceptStates();

	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(NFA_Test, GetAcceptStates_ReturnsAllAcceptStates) {
	automaton->addAcceptState("q0");
	automaton->addAcceptState("q1");

	auto acceptStates = automaton->getAcceptStates();

	ASSERT_EQ(acceptStates.size(), 2);
	EXPECT_TRUE((acceptStates[0].getKey() == "q0" && acceptStates[1].getKey() == "q1") ||
	            (acceptStates[0].getKey() == "q1" && acceptStates[1].getKey() == "q0"));
}

TEST_F(NFA_Test, GetAcceptStates_EmptyIfNoAcceptStates) {
	auto acceptStates = automaton->getAcceptStates();
	EXPECT_TRUE(acceptStates.empty());
}

TEST_F(NFA_Test, Reset_SetsCurrentStateToStartState) {
	automaton->addTransition("q0", "q1", "0");
	automaton->processInput("0");

	EXPECT_EQ(automaton->getCurrentState(), "q1");

	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
}

TEST_F(NFA_Test, Reset_HandlesAcceptStartState) {
	automaton->addAcceptState("q0");
	automaton->reset();

	EXPECT_EQ(automaton->getCurrentState(), "q0");
	EXPECT_FALSE(automaton->getAcceptStates().empty());
}

TEST_F(NFA_Test, Simulate_ShouldThrowExceptionForSimulationWithoutStartState) {
	NonDeterministicFiniteAutomaton *automaton = new NonDeterministicFiniteAutomaton();
	automaton->setInputAlphabet({"0", "1"});

	EXPECT_THROW(automaton->simulate({""}), InvalidStartStateException);
}

TEST_F(NFA_Test, Simulate_ValidInputAccepts) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");

	EXPECT_TRUE(automaton->simulate({"0", "1"}));
}

TEST_F(NFA_Test, Simulate_InvalidInputReject) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");

	EXPECT_FALSE(automaton->simulate({"0", "0"}));
}

TEST_F(NFA_Test, Simulate_InvalidInputRejects) {
	automaton->addTransition("q0", "q1", "0");

	EXPECT_FALSE(automaton->simulate({"1"}));
}

TEST_F(NFA_Test, Simulate_ThrowsIfSimulationDepthExceeded) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q0", "1");

	automaton->setStartState("q0");

	std::vector<std::string> input = {"0", "1", "0", "1", "0", "1", "0", "1", "0", "1", "0", "1"};

	EXPECT_THROW(automaton->simulate(input, 3), SimulationDepthExceededException);
}

TEST_F(NFA_Test, Simulate_EmptyInputStaysAtStartState) {
	automaton->addAcceptState("q0");
	EXPECT_TRUE(automaton->simulate({}));
}

TEST_F(NFA_Test, Simulate_BasicAccept) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addAcceptState("q1");
	EXPECT_TRUE(automaton->simulate({"0"}));
}

TEST_F(NFA_Test, Simulate_MultipleInputsAccept) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");
	EXPECT_TRUE(automaton->simulate({"0", "1"}));
}

TEST_F(NFA_Test, Simulate_MultipleInputsReject) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");
	EXPECT_FALSE(automaton->simulate({"0", "0"}));
}

TEST_F(NFA_Test, Simulate_EmptyInputString) {
	automaton->addAcceptState("q0");
	EXPECT_TRUE(automaton->simulate({}));

	EXPECT_FALSE(automaton->simulate({}));
}

TEST_F(NFA_Test, Simulate_NonDeterministicPath) {
	automaton->addTransition("q0", "q0", "0");
	automaton->addTransition("q0", "q1", "0");
	automaton->addAcceptState("q1");
	EXPECT_TRUE(automaton->simulate({"0"}));
}

TEST_F(NFA_Test, Simulate_EpsilonTransition) {
	automaton->addTransition("q0", "q1", "");
	automaton->addAcceptState("q1");
	EXPECT_TRUE(automaton->simulate({}));
}

TEST_F(NFA_Test, Simulate_EpsilonWithInputTransition) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "");
	automaton->addTransition("q1", "q2", "0");
	automaton->addAcceptState("q2");
	EXPECT_TRUE(automaton->simulate({"0"}));
}

TEST_F(NFA_Test, Simulate_EpsilonClosure) {
	automaton->addState("q2");
	automaton->addState("q3");
	automaton->addTransition("q0", "q1", "");
	automaton->addTransition("q1", "q2", "");
	automaton->addTransition("q2", "q3", "0");
	automaton->addAcceptState("q3");
	EXPECT_TRUE(automaton->simulate({"0"}));
}

TEST_F(NFA_Test, Simulate_LongInputSequence) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addTransition("q2", "q0", "0");
	automaton->addAcceptState("q2");
	EXPECT_TRUE(automaton->simulate({"0", "1"}));
	EXPECT_TRUE(automaton->simulate({"0", "1", "0", "1"}));
	EXPECT_FALSE(automaton->simulate({"0", "1", "0"}));
}

TEST_F(NFA_Test, ProcessInput_BasicAccept) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addAcceptState("q1");
	automaton->reset();
	EXPECT_TRUE(automaton->processInput("0"));
}

TEST_F(NFA_Test, ProcessInput_BasicReject) {
	automaton->addTransition("q0", "q1", "0");
	automaton->reset();
	EXPECT_FALSE(automaton->processInput("0")); // q1 is not an accept state
}

TEST_F(NFA_Test, ProcessInput_InvalidInput) {
	automaton->addTransition("q0", "q1", "0");
	automaton->reset();
	EXPECT_FALSE(automaton->processInput("1")); // No transition on "1" from q0
}

TEST_F(NFA_Test, ProcessInput_SequentialInputs) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q1", "q2", "1");
	automaton->addAcceptState("q2");

	automaton->reset();
	EXPECT_FALSE(automaton->processInput("0")); // In q1, not accepting
	EXPECT_TRUE(automaton->processInput("1"));  // Now in q2, which is accepting
}

TEST_F(NFA_Test, ProcessInput_EpsilonTransition) {
	automaton->addTransition("q0", "q1", ""); // Epsilon transition
	automaton->addAcceptState("q1");

	automaton->reset();
	EXPECT_TRUE(automaton->processInput("")); // Should follow epsilon transition
}

TEST_F(NFA_Test, ProcessInput_NonDeterministicChoice) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", "0");
	automaton->addTransition("q0", "q2", "0");
	automaton->addAcceptState("q1");

	automaton->reset();

	// This is non-deterministic, but with our test setup and random selection,
	// we know it should eventually reach an accept state
	// Note: In real testing, this might need a different approach due to randomness
	bool foundAcceptingPath = false;
	for (int i = 0; i < 10; i++) {
		automaton->reset();
		if (automaton->processInput("0")) {
			foundAcceptingPath = true;
			break;
		}
	}
	EXPECT_TRUE(foundAcceptingPath);
}

TEST_F(NFA_Test, ProcessInput_MultipleEpsilonTransitions) {
	automaton->addState("q2");
	automaton->addTransition("q0", "q1", ""); // Epsilon transition
	automaton->addTransition("q1", "q2", ""); // Another epsilon transition
	automaton->addAcceptState("q2");

	automaton->reset();
	EXPECT_TRUE(automaton->processInput("")); // Should follow both epsilon transitions
}

TEST_F(NFA_Test, ProcessInput_ResetBehavior) {
	automaton->addTransition("q0", "q1", "0");
	automaton->addAcceptState("q1");

	automaton->reset();
	EXPECT_TRUE(automaton->processInput("0"));

	automaton->reset();                         // Should go back to start state q0
	EXPECT_FALSE(automaton->processInput("1")); // No valid transition
}