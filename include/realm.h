#ifndef ANT_WORLD_REALM_H
#define ANT_WORLD_REALM_H

#include "delta.h"

#include <vector>
#include <queue>

namespace aw {

    class GameObject;
    class World;
    class Hole;

    class Realm {
    public:
        Realm();
        ~Realm();

        dphysics::RigidBodySystem PhysicsSystem;

        void registerGameObject(GameObject *object);
        void unregisterGameObject(GameObject *object);

        void updateRealms();

        void process();
        void render();

        void spawnObjects();

        dbasic::DeltaEngine &getEngine();

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        template <typename T>
        T *spawn() {
            void *buffer = _aligned_malloc(sizeof(T), 16);
            T *newObject = new (buffer) T;
            newObject->setWorld(m_world);
            newObject->setRealm(this);
            addToSpawnQueue(newObject);

            return newObject;
        }

        void setExitPortal(Hole *portal) { m_exitPortal = portal; }
        Hole *getExitPortal() const { return m_exitPortal; }

    protected:
        void addToSpawnQueue(GameObject *object);
        void cleanObjectList();
        void destroyObject(GameObject *object);

    protected:
        std::queue<GameObject *> m_spawnQueue;
        std::vector<GameObject *> m_gameObjects;

    protected:
        World *m_world;
        Hole *m_exitPortal;
    };

} /* namespace aw */

#endif /* ANT_WORLD_REALM_H */
