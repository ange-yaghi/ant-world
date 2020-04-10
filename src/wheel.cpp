#include "../include/wheel.h"

#include "../include/math.h"
#include "../include/game_object.h"

dbasic::ModelAsset *aw::Wheel::m_wheelModel = nullptr;

aw::Wheel::Wheel() {
    m_coeffStaticFriction = 5.0f;
    m_coeffDynamicFriction = 4.0f;
    m_inputTorque = 1000.0f;
    m_resistance = 50.0f;
    m_slipping = false;
}

aw::Wheel::~Wheel() {
    /* void */
}

void aw::Wheel::debugRender() {
    m_engine->SetObjectTransform(RigidBody.GetTransform());

    int red[3] = { 255, 0, 0 };

    //m_engine->DrawBox(red, 0.2f, 1.0f, (int)GameObject::Layer::Wall);
    if (isSlipping()) {
        m_engine->SetMultiplyColor(ysVector4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    else {
        m_engine->SetMultiplyColor(ysVector4(0.0f, 0.0f, 1.0f, 1.0f));
    }

    m_engine->DrawModel(m_wheelModel, RigidBody.GetTransform(), 1.0f, nullptr, (int)GameObject::Layer::Wall);
}

void aw::Wheel::Initialize(dphysics::RigidBody *rigidBody) {
    dphysics::ForceGenerator::Initialize(rigidBody);

    rigidBody->AddChild(&RigidBody);

    RigidBody.SetInverseMass(0.5f);
    RigidBody.SetInverseInertiaTensor(ysMath::LoadMatrix(
        ysMath::Constants::XAxis,
        ysMath::Constants::Zero,
        ysMath::Constants::Zero,
        ysMath::Constants::Zero));
    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
}

void aw::Wheel::GenerateForces(float dt) {
    ysVector shear = getShearDirection();
    ysVector forward = ysMath::Cross(ysMath::Constants::ZAxis, shear);

    ysVector vehicleVelocity = m_rigidBody->GetVelocityLocal(RigidBody.GetPosition());

    // Wheel velocity at contact point (due to rotation)
    ysVector contactPoint = ysMath::LoadVector(0.0f, 0.0f, -m_radius, 1.0f);
    ysVector rotationVelocity = RigidBody.GetGlobalSpace(RigidBody.GetVelocityLocal(contactPoint));

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

    m_rigidBody->AddImpulseWorldSpace(correctionShear, RigidBody.GetWorldPosition());

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

    if (torque < m_inputTorque) {
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

    RigidBody.AddAngularImpulseLocal(ysMath::LoadVector(-wheelAngularImpulse, 0.0f, 0.0f, 0.0f));
    m_rigidBody->AddImpulseWorldSpace(
        ysMath::Mul(ysMath::LoadScalar(vehicleLinearImpulse), forward), 
        RigidBody.GetWorldPosition());
}

ysVector aw::Wheel::getForwardDirection() const {
    return ysMath::MatMult(RigidBody.GetOrientationMatrix(), ysMath::Constants::YAxis);
}

ysVector aw::Wheel::getShearDirection() const {
    return ysMath::MatMult(RigidBody.GetOrientationMatrix(), ysMath::Constants::XAxis);
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
