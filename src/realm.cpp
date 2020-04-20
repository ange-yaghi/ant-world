#include "../include/realm.h"

#include "../include/game_object.h"
#include "../include/world.h"
#include "../include/hole.h"
#include "../include/player.h"

aw::Realm::Realm() {
    m_exitPortal = nullptr;
    m_world = nullptr;

    m_visibleObjectCount = 0;
}

aw::Realm::~Realm() {
    /* void */
}

void aw::Realm::registerGameObject(GameObject *object) {
    object->setRealm(this);
    object->setRealmRecordIndex((int)m_gameObjects.size());
    m_gameObjects.push_back(object);

    PhysicsSystem.RegisterRigidBody(&object->RigidBody);
}

void aw::Realm::unregisterGameObject(GameObject *object) {
    int index = object->getRealmRecordIndex();

    object->setRealmRecordIndex(-1);
    PhysicsSystem.RemoveRigidBody(&object->RigidBody);

    m_gameObjects[index] = m_gameObjects.back();
    m_gameObjects[index]->setRealmRecordIndex(index);
    m_gameObjects.pop_back();
}

void aw::Realm::process() {
    cleanObjectList();
    spawnObjects();
    respawnObjects();

    PhysicsSystem.Update(getEngine().GetFrameLength() / 2);
    PhysicsSystem.Update(getEngine().GetFrameLength() / 2);

    for (GameObject *g : m_gameObjects) {
        if (g->getDeletionFlag()) continue;

        g->updatePathfinderBounds();
        g->createVisualBounds();
        g->process();
    }
}

void aw::Realm::render() {
    if (isIndoor()) {
        m_world->getEngine().SetClearColor(0x0, 0x0, 0x0);

        constexpr float intensity = 50.0f;

        m_world->getEngine().SetAmbientLight(ysVector4(0.3f, 0.3f, 0.3f, 1.0f));
        dbasic::Light holeLight;
        holeLight.Position = ysVector4(0.0f, 0.0f, 10.0f, 0.0f);
        holeLight.Direction = ysMath::GetVector4(ysMath::Normalize(ysMath::LoadVector(0.2f, 0.2f, -1.0f)));
        holeLight.Color = ysVector4(0.5f * intensity, 0.5f * intensity, 1.0f * intensity, 0.0f);
        holeLight.FalloffEnabled = 1;
        holeLight.Attenuation0 = 0.99;
        holeLight.Attenuation1 = 0.8;
        m_world->getEngine().AddLight(holeLight);
    }
    else {
        m_world->getEngine().SetClearColor(0xf4, 0xa4, 0x60);
        m_world->getEngine().SetAmbientLight(ysVector4(0.1f, 0.1f, 0.1f, 1.0f));

        ysVector playerPosition = m_world->getPlayer()->RigidBody.Transform.GetLocalPosition();
        ysVector4 sunPosition = ysMath::GetVector4(playerPosition);
        sunPosition.z = 1000.0f;

        dbasic::Light sun;
        sun.Position = sunPosition;
        sun.Color = ysVector4(0.95f, 0.9f, 1.0f, 0.0f);
        sun.FalloffEnabled = 0;
        m_world->getEngine().AddLight(sun);
    }

    AABB cameraExtents = m_world->getCameraExtents();

    int visibleObjects = 0;

    for (GameObject *g : m_gameObjects) {
        if (g->getDeletionFlag()) continue;
        AABB extents = g->getVisualBounds();

        if (extents.intersects2d(cameraExtents)) {
            g->render();
            ++visibleObjects;
        }
    }

    m_visibleObjectCount = visibleObjects;
}

void aw::Realm::spawnObjects() {
    while (!m_spawnQueue.empty()) {
        GameObject *u = m_spawnQueue.front(); m_spawnQueue.pop();
        u->initialize();
        u->addPathfinderBounds();
        registerGameObject(u);
    }
}

void aw::Realm::respawnObjects() {
    while (!m_respawnQueue.empty()) {
        GameObject *u = m_respawnQueue.front(); m_respawnQueue.pop();
        registerGameObject(u);
    }
}

dbasic::DeltaEngine &aw::Realm::getEngine() {
    return m_world->getEngine();
}

void aw::Realm::updateRealms() {
    int N = (int)m_gameObjects.size();
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->isChangingRealm()) {
            GameObject *object = m_gameObjects[i];
            Realm *newRealm = object->getNewRealm();
            object->resetRealmChange();

            unregisterGameObject(object);

            if (newRealm != nullptr) {
                newRealm->registerGameObject(object);
            }

            GameObject *lastPortal = object->getLastPortal();
            object->setLastPortal(nullptr);

            if (lastPortal != nullptr) {
                if (newRealm == lastPortal->getTargetRealm()) {
                    lastPortal->onEnter(object);
                }
                else if (newRealm == lastPortal->getRealm()) {
                    lastPortal->onExit(object);
                }
            }

            --i;
            N = (int)m_gameObjects.size(); 
        }
    }
}

void aw::Realm::respawn(GameObject *object) {
    m_respawnQueue.push(object);
}

void aw::Realm::addToSpawnQueue(GameObject *object) {
    m_spawnQueue.push(object);
}

void aw::Realm::cleanObjectList() {
    int N = (int)m_gameObjects.size();
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->getDeletionFlag()) {
            m_deadObjects.push_back(m_gameObjects[i]);
            unregisterGameObject(m_gameObjects[i]);

            --i; --N;
        }
    }

    int N_dead = (int)m_deadObjects.size();
    for (int i = 0; i < N_dead; ++i) {
        if (m_deadObjects[i]->getReferenceCount() == 0) {
            destroyObject(m_deadObjects[i]);

            m_deadObjects[i] = m_deadObjects.back();
            m_deadObjects.pop_back();

            --i; --N_dead;
        }
        else {
            int a = 0;
        }
    }
}

void aw::Realm::destroyObject(GameObject *object) {
    _aligned_free((void *)object);
}
