#include "../include/realm.h"

#include "../include/game_object.h"
#include "../include/world.h"

aw::Realm::Realm() {
    /* void */
}

aw::Realm::~Realm() {
    /* void */
}

void aw::Realm::registerGameObject(GameObject *object) {
    object->setRealm(this);
    object->setRealmRecordIndex(m_gameObjects.size());
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
        registerGameObject(u);
        m_gameObjects.push_back(u);
    }
}

dbasic::DeltaEngine &aw::Realm::getEngine() {
    return m_world->getEngine();
}

void aw::Realm::updateRealms() {
    int N = m_gameObjects.size();
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->getNewRealm() != nullptr) {
            Realm *newRealm = m_gameObjects[i]->getNewRealm();
            m_gameObjects[i]->changeRealm(nullptr);

            unregisterGameObject(m_gameObjects[i]);
            newRealm->registerGameObject(m_gameObjects[i]);

            --i;
            N = m_gameObjects.size();
        }
    }
}

void aw::Realm::addToSpawnQueue(GameObject *object) {
    m_spawnQueue.push(object);
}

void aw::Realm::cleanObjectList() {
    int N = m_gameObjects.size();
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
