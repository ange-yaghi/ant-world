#include "../include/leaf.h"

#include "../include/world.h"

aw::Leaf::Leaf() {
    setCanBeCarried(true);
}

aw::Leaf::~Leaf() {
    /* void */
}

void aw::Leaf::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dbasic::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.1f);

    dbasic::CollisionObject *bounds;
    //RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    //bounds->SetMode(dbasic::CollisionObject::Mode::Fine);
    //bounds->GetAsBox()->HalfHeight = 0.5f;
    //bounds->GetAsBox()->HalfWidth = 0.5f;
    //bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    //bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->GetAsCircle()->RadiusSquared = 0.5f * 0.5f;
}

void aw::Leaf::process() {
    /* void */
}

void aw::Leaf::render() {
    int color[] = { 0xc7, 0xea, 0x46 };
    ysVector position = RigidBody.GetPosition();

    ysMatrix translation = ysMath::TranslationTransform(position);
    m_world->getEngine().SetObjectTransform(translation);
    m_world->getEngine().DrawBox(color, 1.0f, 1.0f, (int)Layer::Items);
}
