#ifndef ANT_WORLD_GAME_OBJECT_H
#define ANT_WORLD_GAME_OBJECT_H

#include "delta.h"

#include <vector>

namespace aw {

    class World;
    class Realm;
    class Hole;

    class GameObject {
    public:
        enum class Layer {
            Player,
            Mob,
            Items,
            Holes,
            Ground
        };

        enum class Tag {
            Hole,
            Insect,
            Player,
            Carryable,
            Count
        };

    public:
        GameObject();
        ~GameObject();

        dphysics::RigidBody RigidBody;

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        virtual void initialize();
        virtual void render();
        virtual void process();

        bool getDeletionFlag() const { return m_deletionFlag; }
        void setDeletionFlag() { m_deletionFlag = true; }

        void setBeingCarried(bool carried) { m_beingCarried = carried; }
        bool isBeingCarried() const { return m_beingCarried; }

        bool hasTag(Tag tag) const { return m_tags[(int)tag]; }
        void addTag(Tag tag) { m_tags[(int)tag] = true; }
        void removeTag(Tag tag) { m_tags[(int)tag] = false; }

        int getRealmRecordIndex() const { return m_realmRecordIndex; }
        void setRealmRecordIndex(int index) { m_realmRecordIndex = index; }

        void setRealm(Realm *realm) { m_realm = realm; }
        Realm *getRealm() const { return m_realm; }

        virtual void changeRealm(Realm *newRealm) { m_newRealm = newRealm; }
        Realm *getNewRealm() const { return m_newRealm; }

        Hole *getLastPortal() { return m_lastPortal; }
        void setLastPortal(Hole *hole) { m_lastPortal = hole; }

    protected:
        World *m_world;
        Realm *m_realm;
        Realm *m_newRealm;
        Hole *m_lastPortal;

    private:
        bool m_beingCarried;

    private:
        bool m_deletionFlag;

    private:
        std::vector<bool> m_tags;

    private:
        int m_realmRecordIndex;
    };

} /* namespace aw */

#endif /* ANT_WORLD_GAME_OBJECT_H */
