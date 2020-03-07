#ifndef ANT_WORLD_LEAF_H
#define ANT_WORLD_LEAF_H

#include "game_object.h"

namespace aw {

    class Leaf : public GameObject {
    public:
        Leaf();
        ~Leaf();

        virtual void initialize();

        virtual void render();
        virtual void process();
    };

} /* namespace aw */

#endif /* ANT_WORLD_LEAF_H */
