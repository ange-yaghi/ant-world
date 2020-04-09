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
    RigidBody.SetInverseMass(1.0f);
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsCircle()->Radius = 0.9f;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->Radius = 2.0f;
}

void aw::Cookie::render() {
    constexpr bool ShowAwake = false;

    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());

    if (RigidBody.IsAwake() || !ShowAwake) {
        m_world->getEngine().SetMultiplyColor(ysVector4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else {
        m_world->getEngine().SetMultiplyColor(ysVector4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    m_world->getEngine().DrawImage(CookieTexture->GetTexture(), (int)Layer::Items);
}

void aw::Cookie::process() {
    Food::process();
}

void aw::Cookie::configureAssets(dbasic::AssetManager *am) {
    CookieTexture = am->GetTexture("Cookie");
}
