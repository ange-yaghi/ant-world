#include "../include/vehicle.h"

#include "../include/world.h"

aw::Vehicle::Vehicle() {
    m_flWheel = nullptr;
    m_frWheel = nullptr;
    m_rlWheel = nullptr;
    m_rrWheel = nullptr;
}

aw::Vehicle::~Vehicle() {
    /* void */
}

void aw::Vehicle::initialize() {
    GameObject::initialize();

    RigidBody.SetInverseMass(1.0f / 10.0f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(1.0f, 1.0f));
    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.5f;
    bounds->GetAsBox()->HalfWidth = 1.5f;
    bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    RigidBody.SetOrientation(ysMath::LoadQuaternion(ysMath::Constants::PI / 2, ysMath::Constants::ZAxis));

    //m_flWheel = RigidBody.NewForceGenerator<Wheel>();
    //m_frWheel = RigidBody.NewForceGenerator<Wheel>();
    m_rlWheel = RigidBody.NewForceGenerator<Wheel>();
    m_rrWheel = RigidBody.NewForceGenerator<Wheel>();

    //m_flWheel->setRadius(0.5f);
    //m_frWheel->setRadius(0.5f);
    m_rlWheel->setRadius(0.5f);
    m_rrWheel->setRadius(0.5f);

    //m_flWheel->setEngine(&m_world->getEngine());
    //m_frWheel->setEngine(&m_world->getEngine());
    m_rlWheel->setEngine(&m_world->getEngine());
    m_rrWheel->setEngine(&m_world->getEngine());

    //m_flWheel->RigidBody.SetPosition(ysMath::LoadVector(-1.0f, 1.0f, 0.0f));
    //m_frWheel->RigidBody.SetPosition(ysMath::LoadVector(1.0f, 1.0f, 0.0f));
    m_rrWheel->RigidBody.SetPosition(ysMath::LoadVector(1.0f, -1.0f, 0.0f));
    m_rlWheel->RigidBody.SetPosition(ysMath::LoadVector(-1.0f, -1.0f, 0.0f, 1.0f));
}

void aw::Vehicle::process() {
    GameObject::process();

    m_rlWheel->RigidBody.SetAngularVelocity(ysMath::LoadVector(m_wheelSpeed, 0.0f, 0.0f));
    m_rrWheel->RigidBody.SetAngularVelocity(ysMath::LoadVector(m_wheelSpeed, 0.0f, 0.0f));

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_G)) {
        m_wheelSpeed += 0.1f;
        if (m_wheelSpeed > 50.0f) m_wheelSpeed = 50.0f;
    }
    else {
        m_wheelSpeed -= 0.1f;
        if (m_wheelSpeed < 0) m_wheelSpeed = 0.0f;
    }
}

void aw::Vehicle::render() {
    int red[] = { 255, 0, 0 };
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(red, 1.0f, 1.0f, (int)GameObject::Layer::Wall);

    m_rlWheel->debugRender();
    m_rrWheel->debugRender();
    //m_flWheel->debugRender();
    //m_frWheel->debugRender();
}
