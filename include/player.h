#ifndef ANT_WORLD_PLAYER_H
#define ANT_WORLD_PLAYER_H

#include "insect.h"

namespace aw {

    class Player : public Insect {
    public:
        Player();
        ~Player();

        virtual void initialize();
        virtual void process();
        virtual void render();

        void updateEnergy(float dt);

    protected:
        virtual void grab();
        virtual void drop();
        virtual void eat();

        virtual void enterHole();
        virtual void exitHole();

    protected:
        float m_energy;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PLAYER_H */
