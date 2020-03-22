#include "../include/realm.h"

#include "../include/game_object.h"
#include "../include/world.h"
#include "../include/hole.h"

aw::Realm::Realm() {
    m_exitPortal = nullptr;
    m_world = nullptr;
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

    PhysicsSystem.Update(getEngine().GetFrameLength() / 2);
    PhysicsSystem.Update(getEngine().GetFrameLength() / 2);

    for (GameObject *g : m_gameObjects) {
        g->updatePathfinderBounds();
        g->process();
    }
}

void aw::Realm::render() {
    for (GameObject *g : m_gameObjects) {
        g->render();
    }
}

void aw::Realm::spawnObjects() {
    while (!m_spawnQueue.empty()) {
        GameObject *u = m_spawnQueue.front(); m_spawnQueue.pop();
        u->initialize();
        u->addPathfinderBounds();
        registerGameObject(u);
    }
}

dbasic::DeltaEngine &aw::Realm::getEngine() {
    return m_world->getEngine();
}

void aw::Realm::updateRealms() {
    int N = (int)m_gameObjects.size();
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->getNewRealm() != nullptr) {
            GameObject *object = m_gameObjects[i];
            Realm *newRealm = object->getNewRealm();
            object->changeRealm(nullptr);

            unregisterGameObject(object);
            newRealm->registerGameObject(object);

            Hole *lastPortal = object->getLastPortal();
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

void aw::Realm::addToSpawnQueue(GameObject *object) {
    m_spawnQueue.push(object);
}

void aw::Realm::cleanObjectList() {
    int N = (int)m_gameObjects.size();
    int writeIndex = 0;
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->getDeletionFlag()) {
            destroyObject(m_gameObjects[i]);
        }
        else {
            m_gameObjects[writeIndex++] = m_gameObjects[i];
        }
    }
}

void aw::Realm::destroyObject(GameObject *object) {
    _aligned_free((void *)object);
}
