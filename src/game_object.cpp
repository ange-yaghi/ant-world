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
}

aw::GameObject::~GameObject() {
    /* void */
}

void aw::GameObject::process() {
    /* void */
}

void aw::GameObject::initialize() {
    RigidBody.SetOwner((void *)this);
}

void aw::GameObject::render() {
    /* void */
}
