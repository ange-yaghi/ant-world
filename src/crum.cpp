#include "../include/crum.h"

#include "../include/world.h"

aw::Crum::Crum() {
    addTag(Tag::Carryable);
    addTag(Tag::Edible);
}

aw::Crum::~Crum() {
    /* void */
}

void aw::Crum::initialize() {
    GameObject::initialize();

    setNutritionalValue(1.0f);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(10.0f);
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.5f;
    bounds->GetAsBox()->HalfWidth = 0.5f;
    bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    //RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    //bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    //bounds->GetAsCircle()->RadiusSquared = 0.5f * 0.5f;
}

void aw::Crum::process() {
    /* void */
}

void aw::Crum::render() {
    int color[] = { 0xc7, 0xea, 0x46 };
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 1.0f, 1.0f, (int)Layer::Items);
}
