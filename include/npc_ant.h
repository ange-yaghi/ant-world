#ifndef ANT_WORLD_NPC_ANT_H
#define ANT_WORLD_NPC_ANT_H

#include "insect.h"

#include "pathfinder.h"

namespace aw {

    class NpcAnt : public Insect {
    public:
        NpcAnt();
        ~NpcAnt();

        virtual void initialize();
        virtual void process();
        virtual void render();

    protected:
        Pathfinder m_pathfinder;

        dphysics::CollisionObject *m_sightLine;
    };

} /* namespace aw */

#endif /* ANT_WORLD_NPC_ANT_H */
