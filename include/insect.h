#ifndef ANT_WORLD_INSECT_H
#define ANT_WORLD_INSECT_H

#include "game_object.h"

namespace aw {

    class Insect : public GameObject {
    public:
        Insect();
        ~Insect();

        bool isCarryingItem() const { return m_carryItem == nullptr; }
        void carry(GameObject *object);
        void drop();

    protected:
        GameObject *m_carryItem;
    };

} /* namespace aw */

#endif /* ANT_WORLD_INSECT_H */
