#include "../include/cookie.h"

#include "../include/world.h"

dbasic::TextureAsset *aw::Cookie::CookieTexture = nullptr;

aw::Cookie::Cookie() {
    /* void */
}

aw::Cookie::~Cookie() {
    /* void */
}

void aw::Cookie::initialize() {
    GameObject::initialize();

    setNutritionalValue(1.0f);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(10.0f);
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsCircle()->RadiusSquared = 0.75f * 0.75f;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->RadiusSquared = 2.0f * 2.0f;
}

void aw::Cookie::render() {
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawImage(CookieTexture->GetTexture(), (int)Layer::Items);
}

void aw::Cookie::process() {
    /* void */
}

void aw::Cookie::configureAssets(dbasic::AssetManager *am) {
    CookieTexture = am->GetTexture("Cookie");
}
