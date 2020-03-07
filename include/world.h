#ifndef ANT_WORLD_WORLD_H
#define ANT_WORLD_WORLD_H

#include "game_object.h"

#include "delta.h"

#include <vector>
#include <queue>
#include <malloc.h>

namespace aw {

    class World {
    public:
        World();
        ~World();

        void initialize(void *instance, ysContextObject::DEVICE_API api);
        void initialSpawn();
        void run();
        void frameTick();

        dbasic::DeltaEngine &getEngine() { return m_engine; }
        dbasic::AssetManager &getAssetManager() { return m_assetManager; }

        template <typename T> 
        T *spawn() {
            void *buffer = _aligned_malloc(sizeof(T), 16);
            T *newObject = new (buffer) T;
            newObject->setWorld(this);
            addToSpawnQueue(newObject);

            return newObject;
        }

    protected:
        void addToSpawnQueue(GameObject *object);
        void cleanObjectList();
        void destroyObject(GameObject *object);

        void render();
        void process();

        void spawnObjects();

    protected:
        std::queue<GameObject *> m_spawnQueue;
        std::vector<GameObject *> m_gameObjects;

    protected:
        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;
        dbasic::RigidBodySystem m_rigidBodySystem;
    };

} /* namespace aw */

#endif /* ANT_WORLD_WORLD_H */
