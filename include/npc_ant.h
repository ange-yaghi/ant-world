#ifndef ANT_WORLD_NPC_ANT_H
#define ANT_WORLD_NPC_ANT_H

#include "insect.h"

#include "pathfinder.h"

#include <vector>

namespace aw {

    class NpcAnt : public Insect {
    public:
        NpcAnt();
        ~NpcAnt();

        virtual void initialize();
        virtual void process();
        virtual void render();

        void setDestination(const ysVector &destination);
        ysVector getDestination() const;

    protected:
        Pathfinder m_pathfinder;

        ysVector m_destination;

        dphysics::CollisionObject *m_sightLine;
    };

} /* namespace aw */

#endif /* ANT_WORLD_NPC_ANT_H */
