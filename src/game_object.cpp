#include "../include/game_object.h"

#include "../include/world.h"

aw::GameObject::GameObject() {
    m_world = nullptr;
    m_deletionFlag = false;

    m_beingCarried = false;

    m_tags = std::vector<bool>((int)Tag::Count, false);
    m_realmRecordIndex = -1;

    m_realm = nullptr;
    m_newRealm = nullptr;
    m_lastPortal = nullptr;

    m_referenceCount = 0;
}

aw::GameObject::~GameObject() {
    /* void */
}

void aw::GameObject::process() {
    /* void */
}

void aw::GameObject::updatePathfinderBounds() {
    clearPathfinderBounds();
    addPathfinderBounds();
}

void aw::GameObject::addPathfinderBound(const AABB &bounds) {
    m_pathfinderBounds.push_back(bounds);
}

void aw::GameObject::addPathfinderBounds() {
    int objects = RigidBody.CollisionGeometry.GetNumObjects();
    for (int i = 0; i < objects; ++i) {
        dphysics::CollisionObject *object = RigidBody.CollisionGeometry.GetCollisionObject(i);
        if (object->GetMode() == dphysics::CollisionObject::Mode::Fine) {
            ysVector minPoint, maxPoint;
            object->GetBounds(minPoint, maxPoint);
            addPathfinderBound({ minPoint, maxPoint });
        }
    }
}

void aw::GameObject::initialize() {
    RigidBody.SetOwner((void *)this);
}

void aw::GameObject::render() {
    /* void */
}
