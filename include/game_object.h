#ifndef ANT_WORLD_GAME_OBJECT_H
#define ANT_WORLD_GAME_OBJECT_H

#include "delta.h"

namespace aw {

    class World;

    class GameObject {
    public:
        enum class Layer {
            Player,
            Mob,
            Items,
            Ground            
        };

    public:
        GameObject();
        ~GameObject();

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        virtual void render();
        virtual void process();

        bool getDeletionFlag() const { return m_deletionFlag; }
        void setDeletionFlag() { m_deletionFlag = true; }

        void setBeingCarried(bool carried) { m_beingCarried = carried; }
        bool canBeCarried() const { return m_canBeCarried; }

    protected:
        void setCanBeCarried(bool canBeCarried) { m_canBeCarried = canBeCarried; }

    protected:
        World *m_world;

        ysVector m_position;
        ysVector m_velocity;
        ysVector m_heading;

    private:
        bool m_canBeCarried;
        bool m_beingCarried;

    private:
        bool m_deletionFlag;
    };

} /* namespace aw */

#endif /* ANT_WORLD_GAME_OBJECT_H */
