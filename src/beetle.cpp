#include "../include/beetle.h"

#include "../include/world.h"
#include "../include/interior_generation.h"

aw::Beetle::Beetle() {
    m_realm = nullptr;
    m_targetRealm = nullptr;
}

aw::Beetle::~Beetle() {
    /* void */
}

void aw::Beetle::initialize() {
    GameObject::initialize();

    addTag(Tag::Beetle);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1 / 100.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->Radius = 7.5f;

    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 7.5f;
    bounds->GetAsBox()->HalfWidth = 5.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void aw::Beetle::render() {
    int color[] = { 0xDE, 0xB8, 0x87 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(color, 10.0f, 15.0f, (int)Layer::Holes);
}

void aw::Beetle::onEnter(GameObject *object) {
    object->RigidBody.Transform.SetPosition(ysMath::Constants::Zero);
}

void aw::Beetle::onExit(GameObject *object) {
    object->RigidBody.Transform.SetPosition(RigidBody.Transform.GetWorldPosition());
}

void aw::Beetle::process() {
    /* void */
}

aw::Realm *aw::Beetle::generateRealm() {
    m_targetRealm = m_world->newRealm<Realm>();
    m_targetRealm->setExitPortal(this);

    InteriorGenerator::generateRectangle(m_targetRealm, 10.0f, 15.0f);
    m_targetRealm->setIndoor(true);

    return m_targetRealm;
}
