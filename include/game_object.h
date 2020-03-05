#ifndef ANT_WORLD_GAME_OBJECT_H
#define ANT_WORLD_GAME_OBJECT_H

#include "delta.h"

namespace aw {

    class World;

    class GameObject {
    public:
        GameObject();
        ~GameObject();

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        virtual void render();
        virtual void process();

        bool getDeletionFlag() const { return m_deletionFlag; }
        void setDeletionFlag() { m_deletionFlag = true; }

    protected:
        World *m_world;

        ysVector m_position;
        ysVector m_velocity;
        ysVector m_heading;

        bool m_deletionFlag;
    };

} /* namespace aw */

#endif /* ANT_WORLD_GAME_OBJECT_H */
