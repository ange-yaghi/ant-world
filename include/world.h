#ifndef ANT_WORLD_WORLD_H
#define ANT_WORLD_WORLD_H

#include "game_object.h"
#include "realm.h"
#include "asset_loader.h"
#include "world_grid.h"

#include "delta.h"

#include <vector>
#include <queue>
#include <malloc.h>

namespace aw {

    class Player;

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

        AABB getCameraExtents() const;

        void render();
        void process();

        template <typename T>
        T *newRealm() {
            void *buffer = _aligned_malloc(sizeof(T), 16);
            T *newObject = new (buffer) T;
            newObject->setWorld(this);

            m_realms.push_back(newObject);

            return newObject;
        }

        Realm *getMainRealm() { return m_mainRealm; }

        Player *getPlayer() const { return m_player; }

    protected:
        void updateRealms();

        std::vector<Realm *> m_realms;

        Realm *m_mainRealm;

    protected:
        WorldGrid m_worldGrid;

        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;

        dbasic::Path m_assetPath;

        Player *m_player;
    };

} /* namespace aw */

#endif /* ANT_WORLD_WORLD_H */
