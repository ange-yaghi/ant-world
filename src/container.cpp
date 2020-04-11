#include "../include/container.h"

#include "../include/world.h"

aw::Container::Container() {
    addTag(Tag::Carryable);
    addTag(Tag::Container);
}

aw::Container::~Container() {
    /* void */
}

void aw::Container::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1 / 2.0f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(2.0f, 2.0f));

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 2.0f;
    bounds->GetAsBox()->HalfWidth = 2.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->Radius = 2.0f;
}

void aw::Container::process() {
    /* void */
}

void aw::Container::load(GameObject *object) {
    m_contents.push_back(object);

    // Remove the object from its realm
    object->changeRealm(nullptr);
}

aw::GameObject *aw::Container::unload(int objectIndex) {
    GameObject *object = m_contents[objectIndex];
    m_contents.erase(m_contents.begin() + objectIndex);

    m_realm->respawn(object);

    return object;
}

void aw::Container::render() {
    int color[] = { 0xc7, 0xea, 0x46 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());

    int layer = (int)Layer::Items;
    if (isBeingCarried()) layer = (int)Layer::PlayerCarriedItem;

    m_world->getEngine().DrawBox(color, 4.0f, 4.0f, layer);
}
