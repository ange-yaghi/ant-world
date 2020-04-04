#ifndef ANT_WORLD_FOOD_H
#define ANT_WORLD_FOOD_H

#include "game_object.h"

namespace aw {

    class Food : public GameObject {
    public:
        Food();
        ~Food();

        virtual void initialize();

        virtual void render();
        virtual void process();
    };

} /* namespace aw */

#endif /* ANT_WORLD_FOOD_H */
