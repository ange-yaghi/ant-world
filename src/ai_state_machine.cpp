#include "../include/ai_state_machine.h"

#include "../include/ai_state.h"

aw::AiStateMachine::AiStateMachine() {
    m_currentState = nullptr;
    m_startState = nullptr;
}

aw::AiStateMachine::~AiStateMachine() {
    /* void */
}

void aw::AiStateMachine::addState(AiState *state) {
    state->setMachine(this);
}

void aw::AiStateMachine::update() {
    AiState *nextState = (m_currentState == nullptr)
        ? m_startState
        : m_currentState->update();

    if (nextState != m_currentState) {
        m_currentState->onExit();
        m_currentState = nextState;
        m_currentState->onEnter();
    }
}
