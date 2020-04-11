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

        void setCarryPoint(ysTransform *point) { m_carryPoint = point; }
        ysTransform *getCarryPoint() const { return m_carryPoint; }

        GameObject *getCarriedItem() const { return m_carryItem; }

        void setDropLocation(const ysVector &dropLocation) { m_dropLocation = dropLocation; }
        ysVector getDropLocation() const { return m_dropLocation; }

        virtual void initialize();

        virtual void changeRealm(Realm *newRealm);

        virtual void process();

    protected:
        GameObject *m_carryItem;
        ysTransform *m_carryPoint;
        ysVector m_dropLocation;
    };

} /* namespace aw */

#endif /* ANT_WORLD_INSECT_H */
