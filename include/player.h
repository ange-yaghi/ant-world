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

    protected:
        virtual void grab();
        virtual void drop();
    };

} /* namespace aw */

#endif /* ANT_WORLD_PLAYER_H */
