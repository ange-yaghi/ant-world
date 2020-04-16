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
    int brown[] = { 0xDE, 0xB8, 0x87 };
    int red[] = { 0xFF, 0x0, 0x0 };
    int *color = brown;

    if (m_highlight) color = red;

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(color, 5.0f, 5.0f, (int)Layer::Holes);
}

void aw::Hole::onEnter(GameObject *object) {
    object->RigidBody.Transform.SetPosition(ysMath::Constants::Zero);
}

void aw::Hole::onExit(GameObject *object) {
    object->RigidBody.Transform.SetPosition(RigidBody.Transform.GetWorldPosition());
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
