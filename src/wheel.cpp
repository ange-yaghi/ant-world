#include "../include/wheel.h"

#include "../include/math.h"
#include "../include/game_object.h"

dbasic::ModelAsset *aw::Wheel::m_wheelModel = nullptr;

aw::Wheel::Wheel() {
    m_coeffStaticFriction = 10.0f;
    m_coeffDynamicFriction = 2.0f;
    m_inputTorque = 1000.0f;
    m_resistance = 50.0f;
    m_slipping = false;
    m_velocity = ysMath::Constants::Zero;
    m_angularVelocity = 0.0f;
    m_free = false;
}

aw::Wheel::~Wheel() {
    /* void */
}

void aw::Wheel::debugRender() {
    m_engine->SetObjectTransform(RigidBody.Transform.GetWorldTransform());

    int wheel[3] = { 255, 0, 0 };
    int red[3] = { 255, 0, 0 };
    int green[3] = {0, 255, 0 };

    //m_engine->DrawBox(red, 0.2f, 1.0f, (int)GameObject::Layer::Wall);
    if (isSlipping()) {
        m_engine->SetMultiplyColor(ysVector4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    else {
        m_engine->SetMultiplyColor(ysVector4(0.0f, 0.0f, 1.0f, 1.0f));
    }

    m_engine->DrawModel(
        m_wheelModel, 
        ysMath::MatMult(
            ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 1.0f)), 
            RigidBody.Transform.GetWorldTransform()),
        1.0f, nullptr, (int)GameObject::Layer::Wall);

    int yellow[] = { 255, 255,  0 };
    m_engine->DrawAxis(red, 
        RigidBody.Transform.GetWorldPosition(), 
        getShearDirection(), 
        0.1f,
        2.0f,
        (int)GameObject::Layer::Wall);

    m_engine->DrawAxis(green,
        RigidBody.Transform.GetWorldPosition(),
        getForwardDirection(),
        0.1f,
        2.0f,
        (int)GameObject::Layer::Wall);

    m_engine->DrawAxis(yellow,
        RigidBody.Transform.GetWorldPosition(),
        m_velocity,
        0.1f,
        2.0f,
        (int)GameObject::Layer::Wall);
}

void aw::Wheel::Initialize(dphysics::RigidBody *rigidBody) {
    dphysics::ForceGenerator::Initialize(rigidBody);

    rigidBody->AddChild(&RigidBody);
    RigidBody.Transform.SetParent(&rigidBody->Transform);

    RigidBody.SetInverseMass(0.5f);
    RigidBody.SetInverseInertiaTensor(ysMath::LoadMatrix(
        ysMath::Constants::XAxis,
        ysMath::Constants::Zero,
        ysMath::Constants::Zero,
        ysMath::Constants::Zero));
    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
}

void aw::Wheel::GenerateForces(float dt) {
    RigidBody.SetAngularVelocity(
            ysMath::LoadVector(getAngularVelocity(), 0.0f, 0.0f));

    ysVector shear = getShearDirection();
    ysVector forward = ysMath::Cross(ysMath::Constants::ZAxis, shear);
    forward = ysMath::Mask(forward, ysMath::Constants::MaskOffZ);
    shear = ysMath::Mask(shear, ysMath::Constants::MaskOffZ);

    ysVector vehicleVelocity = m_rigidBody->GetVelocityAtLocalPoint(
        RigidBody.Transform.GetLocalPosition());
    m_velocity = vehicleVelocity;
    //vehicleVelocity = ysMath::Constants::Zero;

    // Wheel velocity at contact point (due to rotation)
    ysVector contactPoint = ysMath::LoadVector(0.0f, 0.0f, -m_radius, 1.0f);
    ysVector rotationVelocity = RigidBody.GetVelocityAtWorldPoint(
        ysMath::Add(RigidBody.Transform.GetWorldPosition(), contactPoint));
    //m_velocity = rotationVelocity;
    if (m_free) {
        ysVector proj = ysMath::Dot(vehicleVelocity, forward);
        proj = ysMath::Mul(proj, forward);
        rotationVelocity = ysMath::Negate(proj);
    }

    // Total wheel velocity at contact point
    ysVector wheelVelocity = ysMath::Add(vehicleVelocity, rotationVelocity);

    ysVector shearVelocity = ysMath::Dot(wheelVelocity, shear);
    ysVector forwardVelocity = ysMath::Dot(wheelVelocity, forward);

    ysVector wheelVelocityMag = ysMath::Magnitude(wheelVelocity);
    float wheelVelocity_s = ysMath::GetScalar(wheelVelocityMag);
    float correctionVelocity = 0.0f;
    if (wheelVelocity_s > getStaticVelocityCorrectionLimit(dt)) {
        correctionVelocity = getDynamicVelocityCorrectionLimit(dt);
        m_slipping = true;
    }
    else {
        m_slipping = false;
        correctionVelocity = min(
            ysMath::GetScalar(wheelVelocityMag),
            getStaticVelocityCorrectionLimit(dt));
    }

    if (wheelVelocity_s == 0) return;

    ysVector velocityDirection = ysMath::Div(wheelVelocity, wheelVelocityMag);
    ysVector correctionImpulse = ysMath::Mul(
        ysMath::LoadScalar(-correctionVelocity / m_rigidBody->GetInverseMass()),
        velocityDirection);

    ysVector correctionShear = ysMath::Mul(ysMath::Dot(correctionImpulse, shear), shear);

    m_rigidBody->AddImpulseWorldSpace(correctionShear, RigidBody.Transform.GetWorldPosition());

    // F = (axial_impulse) / dt
    // torque = (wheel_radius) * F = r * (axial_impulse) / dt
    // axial_impulse = torque * dt / r
    // max_torque = [maximum torque that can be supplied to the wheel]
    // excess_torque = max_torque - torque
    // if excess_torque > 0:
    //      apply excess_torque to slow the wheels
    //      apply max_torque to the vehicle
    // else:
    //      use all torque to slow the wheels

    float axialForce = 
        ysMath::GetScalar(ysMath::Dot(correctionImpulse, forward)) / dt;

    float torque = m_radius * axialForce;

    float wheelAngularImpulse = 0.0f;
    float vehicleLinearImpulse = 0.0f;

    float momentum = ysMath::GetX(RigidBody.GetAngularVelocity()) / RigidBody.GetInverseMass();

    if (std::abs(torque) < m_inputTorque) {
        //wheelAngularImpulse = -excessTorque * dt;
        vehicleLinearImpulse = torque * dt / m_radius;
    }
    else {
        RigidBody.SetAngularVelocity(ysMath::Constants::Zero);
        //wheelAngularImpulse = min(
        //    torque * dt,
        //    momentum
        //    );
    }

    if (!m_free) {
        RigidBody.AddAngularImpulseLocal(ysMath::LoadVector(-wheelAngularImpulse, 0.0f, 0.0f, 0.0f));

        m_rigidBody->AddImpulseWorldSpace(
            ysMath::Mul(ysMath::LoadScalar(vehicleLinearImpulse), forward),
            RigidBody.Transform.GetWorldPosition());
    }
}

ysVector aw::Wheel::getForwardDirection() {
    return ysMath::Cross(ysMath::Constants::ZAxis, getShearDirection());
}

ysVector aw::Wheel::getShearDirection() {
    return RigidBody.Transform.LocalToWorldDirection(ysMath::Constants::XAxis);
}

float aw::Wheel::getDynamicVelocityCorrectionLimit(float dt) const {
    return 9.81f * m_coeffDynamicFriction * dt;
}

void aw::Wheel::configureAssets(dbasic::AssetManager *am) {
    m_wheelModel = am->GetModelAsset("Debug::Wheel");
}

float aw::Wheel::getStaticVelocityCorrectionLimit(float dt) const {
    // Impulse = mass * dv
    // Impulse = F * dt
    // max-impulse = max-force * dt
    // max-impulse = mass * max-dv
    // max-dv = max-force * dt / mass
    // force_g = 9.81 * m
    // friction = force_g * u = 9.81 * m * u
    // max-dv = friction * dt / m = 9.81 * u * dt
    return 9.81f * m_coeffStaticFriction * dt;
}
