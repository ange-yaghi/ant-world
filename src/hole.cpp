#include "../include/hole.h"

#include "../include/world.h"

aw::Hole::Hole() {
    m_realm = nullptr;
}

aw::Hole::~Hole() {
    /* void */
}

void aw::Hole::initialize() {
    setCanBeCarried(false);
    addTag(Tag::Hole);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
    RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->RadiusSquared = 2.5f * 2.5f;
}

void aw::Hole::render() {
    int color[] = { 0xDE, 0xB8, 0x87 };
    ysVector position = RigidBody.GetPosition();

    ysMatrix translation = ysMath::TranslationTransform(position);
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 5.0f, 5.0f, (int)Layer::Holes);
}

void aw::Hole::process() {

}

void aw::Hole::generate() {
    m_targetRealm = m_world->newRealm<Realm>();
}
