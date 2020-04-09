#include "../include/hole.h"

#include "../include/world.h"
#include "../include/interior_generation.h"

aw::Hole::Hole() {
    m_realm = nullptr;
    m_targetRealm = nullptr;
}

aw::Hole::~Hole() {
    /* void */
}

void aw::Hole::initialize() {
    GameObject::initialize();

    addTag(Tag::Hole);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
    RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->Radius = 2.5f;
}

void aw::Hole::render() {
    int color[] = { 0xDE, 0xB8, 0x87 };
    ysVector position = RigidBody.GetPosition();

    ysMatrix translation = ysMath::TranslationTransform(position);
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 5.0f, 5.0f, (int)Layer::Holes);
}

void aw::Hole::onEnter(GameObject *object) {
    object->RigidBody.SetPosition(ysMath::Constants::Zero);
}

void aw::Hole::onExit(GameObject *object) {
    object->RigidBody.SetPosition(RigidBody.GetPosition());
}

void aw::Hole::process() {
    /* void */
}

aw::Realm *aw::Hole::generateRealm() {
    m_targetRealm = m_world->newRealm<Realm>();
    m_targetRealm->setExitPortal(this);

    InteriorGenerator::WallsParam param;
    param.GridSize = 10.0f;
    param.MaxCells = 10;
    param.MinCells = 5;
    InteriorGenerator::generateWalls(m_targetRealm, param);
    m_targetRealm->setIndoor(true);

    return m_targetRealm;
}
