#include "../include/wall.h"

#include "../include/world.h"

aw::Wall::Wall() {
    m_width = 0.0f;
    m_height = 0.0f;
}

aw::Wall::~Wall() {
    /* void */
}

void aw::Wall::initialize() {
    GameObject::initialize();

    addTag(Tag::Wall);
    addTag(Tag::Obstacle);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = m_height / 2;
    bounds->GetAsBox()->HalfWidth = m_width / 2;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void aw::Wall::process() {
    /* void */
}

void aw::Wall::render() {
    int color[] = { 0x0, 0x0, 0x0 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(color, m_width, m_height, (int)Layer::Wall);
}
