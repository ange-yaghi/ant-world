#include "../include/flooring.h"

#include "../include/world.h"

aw::Flooring::Flooring() {
    m_width = 0.0f;
    m_height = 0.0f;
}

aw::Flooring::~Flooring() {
    /* void */
}

void aw::Flooring::initialize() {
    GameObject::initialize();

    addTag(Tag::Floor);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
}

void aw::Flooring::process() {
    /* void */
}

void aw::Flooring::render() {
    int color[] = { 0xDE, 0xB8, 0x87 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(color, m_width, m_height, (int)Layer::Holes);
}
