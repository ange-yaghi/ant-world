#ifndef ANT_WORLD_PLAYER_H
#define ANT_WORLD_PLAYER_H

#include "game_object.h"

namespace aw {

    class Player : public GameObject {
    public:
        Player();
        ~Player();

        virtual void process();
        virtual void render();
    };

} /* namespace aw */

#endif /* ANT_WORLD_PLAYER_H */
