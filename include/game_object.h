#ifndef ANT_WORLD_GAME_OBJECT_H
#define ANT_WORLD_GAME_OBJECT_H

#include "../include/aabb.h"

#include "delta.h"

#include <vector>

namespace aw {

    class World;
    class Realm;
    class Hole;

    class GameObject {
    public:
        enum class Layer {
            Ground,
            Holes,
            Items,
            Mob,
            Player          
        };

        enum class Tag {
            Hole,
            Insect,
            Player,
            Carryable,
            Edible,
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

        void clearPathfinderBounds() { m_pathfinderBounds.clear(); }
        void updatePathfinderBounds();
        void addPathfinderBound(const AABB &bounds);
        virtual void addPathfinderBounds();

        int getPathfinderBoundCount() { return (int)m_pathfinderBounds.size(); }
        AABB &getPathfinderBound(int index) { return m_pathfinderBounds[index]; }

        float getNutritionalValue() const { return m_nutritionalValue; }
        void setNutritionalValue(float value) { m_nutritionalValue = value; }

        void incrementReferenceCount() { ++m_referenceCount; }
        void decrementReferenceCount() { --m_referenceCount; }
        int getReferenceCount() const { return m_referenceCount; }

        virtual ysVector getPickupPointWorld() const { return RigidBody.GetWorldPosition(); }
        virtual float getPickupRadius() const { return 0.0f; }

    protected:
        std::vector<AABB> m_pathfinderBounds;

    protected:
        int m_referenceCount;

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

    private:
        float m_nutritionalValue;
    };

} /* namespace aw */

#endif /* ANT_WORLD_GAME_OBJECT_H */
