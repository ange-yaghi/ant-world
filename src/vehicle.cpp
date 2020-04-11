#include "../include/vehicle.h"

#include "../include/world.h"

aw::Vehicle::Vehicle() {
    m_flWheel = nullptr;
    m_frWheel = nullptr;
    m_rlWheel = nullptr;
    m_rrWheel = nullptr;
    m_fWheel = nullptr;
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
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    RigidBody.Transform.SetOrientation(ysMath::LoadQuaternion(ysMath::Constants::PI / 2, ysMath::Constants::ZAxis));

    //m_flWheel = RigidBody.NewForceGenerator<Wheel>();
    //m_frWheel = RigidBody.NewForceGenerator<Wheel>();
    m_rlWheel = RigidBody.NewForceGenerator<Wheel>();
    m_rrWheel = RigidBody.NewForceGenerator<Wheel>();
    m_fWheel = RigidBody.NewForceGenerator<Wheel>();

    //m_flWheel->setRadius(0.5f);
    //m_frWheel->setRadius(0.5f);
    m_rlWheel->setRadius(0.5f);
    m_rrWheel->setRadius(0.5f);
    m_fWheel->setRadius(0.5f);

    //m_flWheel->setEngine(&m_world->getEngine());
    //m_frWheel->setEngine(&m_world->getEngine());
    m_rlWheel->setEngine(&m_world->getEngine());
    m_rrWheel->setEngine(&m_world->getEngine());
    m_fWheel->setEngine(&m_world->getEngine());

    //m_flWheel->RigidBody.SetPosition(ysMath::LoadVector(-1.0f, 1.0f, 0.0f));
    //m_frWheel->RigidBody.SetPosition(ysMath::LoadVector(1.0f, 1.0f, 0.0f));
    m_rrWheel->RigidBody.Transform.SetPosition(ysMath::LoadVector(1.0f, -1.0f, 0.0f));
    m_rlWheel->RigidBody.Transform.SetPosition(ysMath::LoadVector(-1.0f, -1.0f, 0.0f, 1.0f));
    m_fWheel->RigidBody.Transform.SetPosition(ysMath::LoadVector(0.0f, 1.0f, 0.0f, 1.0f));

    m_fWheel->m_free = true;
}

void aw::Vehicle::process() {
    GameObject::process();

    m_rlWheel->setAngularVelocity(-m_wheelSpeed);
    m_rrWheel->setAngularVelocity(-m_wheelSpeed);

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_G)) {
        m_wheelSpeed += 0.1f;
        if (m_wheelSpeed > 50.0f) m_wheelSpeed = 50.0f;
    }
    else {
        m_wheelSpeed -= 0.1f;
        if (m_wheelSpeed < 0) m_wheelSpeed = 0.0f;
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_LEFT)) {
        m_fWheel->RigidBody.Transform.SetOrientation(ysMath::LoadQuaternion(0.5f, ysMath::Constants::ZAxis));
    }
    else if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_RIGHT)) {
        m_fWheel->RigidBody.Transform.SetOrientation(ysMath::LoadQuaternion(2 * 3.14159 - 0.5f, ysMath::Constants::ZAxis));
    }
    else {
        m_fWheel->RigidBody.Transform.SetOrientation(ysMath::Constants::QuatIdentity);
    }
}

void aw::Vehicle::render() {
    int red[] = { 255, 0, 0 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(red, 1.0f, 1.0f, (int)GameObject::Layer::Wall);

    m_rlWheel->debugRender();
    m_rrWheel->debugRender();
    m_fWheel->debugRender();
    //m_flWheel->debugRender();
    //m_frWheel->debugRender();
}
