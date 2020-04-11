#include "../include/food.h"

#include "../include/world.h"

aw::Food::Food() {
    addTag(Tag::Carryable);
    addTag(Tag::Edible);
}

aw::Food::~Food() {
    /* void */
}

void aw::Food::initialize() {
    GameObject::initialize();

    setNutritionalValue(1.0f);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(10.0f);
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.0f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->Radius = 2.0f;
}

void aw::Food::process() {
    GameObject::process();
}

void aw::Food::render() {
    int color[] = { 0xc7, 0xea, 0x46 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(color, 2.0f, 2.0f, (int)Layer::Items);
}
