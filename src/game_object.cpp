#include "../include/game_object.h"

#include "../include/world.h"

aw::GameObject::GameObject() {
    m_world = nullptr;
    m_deletionFlag = false;

    m_beingCarried = false;
    m_canBeCarried = false;
}

aw::GameObject::~GameObject() {
    /* void */
}

void aw::GameObject::process() {
    /* void */
}

void aw::GameObject::initialize() {
    m_world->getEngine().RBSystem.RegisterRigidBody(&RigidBody);
    RigidBody.SetOwner((void *)this);
}

void aw::GameObject::render() {
    /* void */
}
