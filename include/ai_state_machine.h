#ifndef ANT_WORLD_AI_STATE_MACHINE_H
#define ANT_WORLD_AI_STATE_MACHINE_H

namespace aw {

    class AiState;

    class AiStateMachine {
    public:
        AiStateMachine();
        ~AiStateMachine();

        void setStartState(AiState *startState) { m_startState = startState; }

        virtual void createStates() = 0;
        void addState(AiState *state);
        void update();

    protected:
        AiState *m_currentState;
        AiState *m_startState;
    };

} /* namespace aw */

#endif /* ANT_WORLD_AI_STATE_MACHINE_H */
