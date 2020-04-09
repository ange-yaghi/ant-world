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
    bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void aw::Wall::process() {
    /* void */
}

void aw::Wall::render() {
    int color[] = { 0x0, 0x0, 0x0 };
    ysVector position = RigidBody.GetPosition();

    ysMatrix translation = ysMath::TranslationTransform(position);
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 10.0f, 10.0f, (int)Layer::Wall);
}
