#ifndef ANT_WORLD_AI_STATE_H
#define ANT_WORLD_AI_STATE_H

namespace aw {

    class AiStateMachine;

    class AiState {
    public:
        AiState();
        virtual ~AiState();

        virtual void onEnter() = 0;
        virtual void onExit() = 0;
        virtual AiState *update() = 0;

        AiStateMachine *getMachine() { return m_machine; }
        void setMachine(AiStateMachine *machine) { m_machine = machine; }

    protected:
        AiStateMachine *m_machine;
    };

    template <typename T_Machine>
    class AiState_Real : public AiState {
    public:
        AiState_Real() { /* void */ }
        virtual ~AiState_Real() { /* void */ }

        T_Machine *machine() const { return static_cast<T_Machine>(m_machine); }
    };

} /* namespace aw */

#endif /* ANT_WORLD_AI_STATE_H */
