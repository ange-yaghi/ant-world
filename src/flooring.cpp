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
    ysVector position = RigidBody.GetPosition();

    ysMatrix translation = ysMath::TranslationTransform(position);
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 10.0f, 10.0f, (int)Layer::Holes);
}
