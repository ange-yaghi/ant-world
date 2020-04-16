#include "../include/probe.h"

aw::Probe::Probe() {
    m_radius = 0.0f;
}

aw::Probe::~Probe() {
    /* void */
}

void aw::Probe::initialize() {
    GameObject::initialize();

    addTag(Tag::Probe);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
    RigidBody.SetAlwaysAwake(true);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsCircle()->Radius = m_radius;

    RigidBody.SetGhost(true);
}

void aw::Probe::process() {
    /* void */
}

void aw::Probe::render() {
    /* void */
}

bool aw::Probe::isSafe() {
    return RigidBody.GetCollisionCount() == 0;
}
