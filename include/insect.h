#ifndef ANT_WORLD_INSECT_H
#define ANT_WORLD_INSECT_H

#include "game_object.h"

namespace aw {

    class Insect : public GameObject {
    public:
        Insect();
        ~Insect();

        bool isCarryingItem() const { return m_carryItem != nullptr; }
        void carry(GameObject *object);
        void drop();

        void setCarryLocation(const ysVector &carryLocation) { m_carryLocation = carryLocation; }
        ysVector getCarryLocation() const { return m_carryLocation; }

        void setDropLocation(const ysVector &dropLocation) { m_dropLocation = dropLocation; }
        ysVector getDropLocation() const { return m_dropLocation; }

    protected:
        GameObject *m_carryItem;
        ysVector m_carryLocation;
        ysVector m_dropLocation;
    };

} /* namespace aw */

#endif /* ANT_WORLD_INSECT_H */
