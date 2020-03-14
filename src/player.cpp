#include "../include/player.h"

#include "../include/world.h"

aw::Player::Player() {
    /* void */
}

aw::Player::~Player() {
    /* void */
}

void aw::Player::initialize() {
    Insect::initialize();
    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.SetPosition(ysMath::LoadVector(2.0f, 0.0f, 0.0f));
    RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 2.5f / 2;
    bounds->GetAsBox()->HalfWidth = 2.5f / 2;
    bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
    //RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    //bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    //bounds->GetAsCircle()->RadiusSquared = 1.25 * 1.25;
}

void aw::Player::process() {
    ysVector heading = ysMath::Constants::Zero;
    ysVector velocity = ysMath::LoadScalar(5.0f);
    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_LEFT)) {
        heading = ysMath::Add(heading, ysMath::Negate(ysMath::Constants::XAxis));
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_RIGHT)) {
        heading = ysMath::Add(heading, ysMath::Constants::XAxis);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_UP)) {
        heading = ysMath::Add(heading, ysMath::Constants::YAxis);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_DOWN)) {
        heading = ysMath::Add(heading, ysMath::Negate(ysMath::Constants::YAxis));
    }

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_SPACE)) {
        if (isCarryingItem()) drop();
        else grab();
    }

    RigidBody.SetVelocity(ysMath::Mul(heading, velocity));

    GameObject::process();
}

void aw::Player::grab() {
    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;

        carry((GameObject *)body->GetOwner());
        break;
    }
}

void aw::Player::drop() {
    Insect::drop();
}

void aw::Player::render() {
    int color[] = { 0xf1, 0xc4, 0x0f };
    ysVector position = RigidBody.GetPosition();

    ysMatrix translation = ysMath::TranslationTransform(position);
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 2.5f, 2.5f, (int)Layer::Player);
}
