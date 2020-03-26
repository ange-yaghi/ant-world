#ifndef ANT_WORLD_CRUM_H
#define ANT_WORLD_CRUM_H

#include "game_object.h"

namespace aw {

    class Crum : public GameObject {
    public:
        Crum();
        ~Crum();

        virtual void initialize();

        virtual void render();
        virtual void process();
    };

} /* namespace aw */

#endif /* ANT_WORLD_LEAF_H */
