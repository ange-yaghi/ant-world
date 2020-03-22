#include "../include/player.h"

#include "../include/world.h"
#include "../include/hole.h"

aw::Player::Player() {
    /* void */
}

aw::Player::~Player() {
    /* void */
}

void aw::Player::initialize() {
    Insect::initialize();
    addTag(Tag::Player);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.SetPosition(ysMath::LoadVector(2.0f, 0.0f, 0.0f));
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 2.5f / 2;
    bounds->GetAsBox()->HalfWidth = 2.5f / 2;
    bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->RadiusSquared = 2.5f * 2.5f;
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

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_I)) {
        enterHole();
    }

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_O)) {
        exitHole();
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

        GameObject *object = reinterpret_cast<GameObject *>(body->GetOwner());
        if (object->hasTag(Tag::Carryable) && !object->isBeingCarried()) {
            carry(object);
            break;
        }
    }
}

void aw::Player::drop() {
    Insect::drop();
}

void aw::Player::enterHole() {
    int collisions = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisions; ++i) {
        dphysics::Collision* col = RigidBody.GetCollision(i);
        dphysics::RigidBody* body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;

        GameObject *object = (GameObject*)body->GetOwner();
        if (object->hasTag(Tag::Hole)) {
            Hole* hole = reinterpret_cast<Hole *>(object);
            Realm *targetRealm = hole->getTargetRealm();

            if (targetRealm == nullptr) {
                targetRealm = hole->generateRealm();
            }

            changeRealm(targetRealm);
            setLastPortal(hole);
        }
    }
}

void aw::Player::exitHole() {
    Realm *currentRealm = getRealm();
    Hole *exitPortal = currentRealm->getExitPortal();
    
    if (exitPortal == nullptr) return;
    
    changeRealm(exitPortal->getRealm());
    setLastPortal(exitPortal);
}

void aw::Player::render() {
    int color[] = { 0xf1, 0xc4, 0x0f };
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 2.5f, 2.5f, (int)Layer::Player);
}
