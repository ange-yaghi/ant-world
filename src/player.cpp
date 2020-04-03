#include "../include/player.h"

#include "../include/world.h"
#include "../include/hole.h"

#include <sstream>

ysAnimationAction
    *aw::Player::AnimBlink = nullptr,
    *aw::Player::AnimIdle = nullptr,
    *aw::Player::AnimWalk = nullptr,
    *aw::Player::PoseFaceDown = nullptr,
    *aw::Player::PoseFaceUp = nullptr, 
    *aw::Player::PoseFaceLeft = nullptr, 
    *aw::Player::PoseFaceRight = nullptr;

dbasic::SceneObjectAsset *aw::Player::CharacterRoot = nullptr;

aw::Player::Player() {
    m_energy = 0.0f;
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
    bounds->GetAsBox()->HalfHeight = 2.0f;
    bounds->GetAsBox()->HalfWidth = 2.0f;
    bounds->GetAsBox()->Orientation = ysMath::LoadIdentity();
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->RadiusSquared = 2.5f * 2.5f;
    //RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    //bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    //bounds->GetAsCircle()->RadiusSquared = 1.25 * 1.25;

    m_renderSkeleton = m_world->getAssetManager().BuildRenderSkeleton(&RigidBody, CharacterRoot);

    m_renderSkeleton->BindAction(AnimBlink, &m_animBlink);
    m_renderSkeleton->BindAction(AnimWalk, &m_animWalk);
    m_renderSkeleton->BindAction(AnimIdle, &m_animIdle);
    m_renderSkeleton->BindAction(PoseFaceDown, &m_faceDown);
    m_renderSkeleton->BindAction(PoseFaceUp, &m_faceUp);
    m_renderSkeleton->BindAction(PoseFaceLeft, &m_faceLeft);
    m_renderSkeleton->BindAction(PoseFaceRight, &m_faceRight);

    m_legsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_antennaChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_eyelidChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_bodyRotationChannel = m_renderSkeleton->AnimationMixer.NewChannel();

    ysAnimationChannel::ActionSettings settings;
    settings.FadeIn = 0.0f;
    m_bodyRotationChannel->AddSegment(&m_faceUp, settings);

    dphysics::RigidBody *mouth = &m_renderSkeleton->GetNode("Mouth")->RigidBody;
    setCarryPoint(mouth);

    m_energy = 10.0f;
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

void aw::Player::eat() {
    if (!isCarryingItem()) return;

    GameObject *carriedObject = m_carryItem;
    if (!carriedObject->hasTag(Tag::Edible)) return;

    m_energy += carriedObject->getNutritionalValue();

    carriedObject->setDeletionFlag();
    drop();
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

void aw::Player::updateMotion() {
    ysVector heading = ysMath::Constants::Zero;
    ysVector velocity = ysMath::LoadScalar(5.0f);
    dbasic::RenderNode *probe = m_renderSkeleton->GetNode("Root");
     
    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_LEFT)) {
        heading = ysMath::Add(heading, ysMath::Negate(ysMath::Constants::XAxis));
        //RigidBody.SetOrientation(ysMath::LoadQuaternion(3 * ysMath::Constants::PI / 2.0f, ysMath::Constants::ZAxis));
        ysQuaternion a = ysMath::LoadQuaternion(ysMath::Constants::PI / 2.0f, ysMath::Constants::ZAxis);
        //RigidBody.SetAngularVelocity(ysMath::LoadVector(0.0f, 0.0f, -2.0f));
        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = 20.0f;
        if (m_bodyRotationChannel->GetCurrentAction() != &m_faceLeft) {
            m_bodyRotationChannel->AddSegment(&m_faceLeft, settings);
        }
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_RIGHT)) {
        heading = ysMath::Add(heading, ysMath::Constants::XAxis);
        //RigidBody.SetOrientation(ysMath::LoadQuaternion(ysMath::Constants::PI / 2.0f, ysMath::Constants::ZAxis));
        //RigidBody.SetAngularVelocity(ysMath::LoadVector(0.0f, 0.0f, 2.0f));
        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = 20.0f;
        m_bodyRotationChannel->AddSegment(&m_faceRight, settings);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_UP)) {
        heading = ysMath::Add(heading, ysMath::Constants::YAxis);
        //RigidBody.SetOrientation(ysMath::Constants::QuatIdentity);
        //velocity = ysMath::LoadScalar(5.0f);
        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = 20.0f;
        m_bodyRotationChannel->AddSegment(&m_faceUp, settings);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_DOWN)) {
        heading = ysMath::Add(heading, ysMath::Negate(ysMath::Constants::YAxis));
        //RigidBody.SetOrientation(ysMath::LoadQuaternion(ysMath::Constants::PI, ysMath::Constants::ZAxis));
        //velocity = ysMath::LoadScalar(5.0f);
        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = 20.0f;
        m_bodyRotationChannel->AddSegment(&m_faceDown, settings);
    }

    //ysVector vel = RigidBody.GetGlobalSpace(ysMath::Mul(ysMath::Constants::YAxis, velocity));
    RigidBody.SetVelocity(ysMath::Mul(heading, velocity));
}

void aw::Player::updateAnimation() {
    float velocity = ysMath::GetScalar(ysMath::Magnitude(RigidBody.GetVelocity()));
    float unitVelocity = 5.0f;
    float relVelocity = velocity / unitVelocity;
    
    if (relVelocity < 0.001f) {
        ysAnimationChannel::ActionSettings s;
        s.FadeIn = 20.0f;
        s.Speed = 1.0f;

        if (m_legsChannel->GetCurrentAction() != &m_animIdle) {
            m_legsChannel->AddSegment(&m_animIdle, s);
            m_legsChannel->ClearQueue();
        }
        else if (!m_legsChannel->HasQueuedSegments()) {
            m_legsChannel->QueueSegment(&m_animIdle, s);
        }
    }
    else {
        if (m_legsChannel->GetSpeed() != relVelocity) {
            m_legsChannel->ChangeSpeed(relVelocity);
            m_legsChannel->ClearQueue();
        }

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = 10.0f / relVelocity;
        settings.Speed = relVelocity;

        if (m_legsChannel->GetCurrentAction() != &m_animWalk) {
            m_legsChannel->AddSegment(&m_animWalk, settings);
            m_legsChannel->ClearQueue();
        }
        else if (!m_legsChannel->HasQueuedSegments()) {
            settings.FadeIn = 0.0f;
            m_legsChannel->QueueSegment(&m_animWalk, settings);
        }
    }

    RigidBody.UpdateDerivedData(true); // TODO: could remove this call?
    m_renderSkeleton->UpdateAnimation(m_world->getEngine().GetFrameLength() * 60.0f);
}

void aw::Player::configureAssets(dbasic::AssetManager *am) {
    AnimBlink = am->GetAction("Blink");
    AnimIdle = am->GetAction("Idle");
    AnimWalk = am->GetAction("Walk");
    PoseFaceUp = am->GetAction("FaceUp");
    PoseFaceDown = am->GetAction("FaceDown");
    PoseFaceLeft = am->GetAction("FaceLeft");
    PoseFaceRight = am->GetAction("FaceRight");

    AnimBlink->SetLength(40.0f);
    AnimIdle->SetLength(100.0f);
    AnimWalk->SetLength(30.0f);
    PoseFaceUp->SetLength(0.0f);
    PoseFaceDown->SetLength(0.0f);
    PoseFaceLeft->SetLength(0.0f);
    PoseFaceRight->SetLength(0.0f);

    CharacterRoot = am->GetSceneObject("Armature");
}

void aw::Player::process() {
    Insect::process();

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

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_E)) {
        eat();
    }

    updateEnergy(m_world->getEngine().GetFrameLength());
    updateMotion();
    updateAnimation();
}

void aw::Player::render() {
    int color[] = { 0xf1, 0xc4, 0x0f };
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    //m_world->getEngine().DrawBox(color, 2.5f, 2.5f, (int)Layer::Player);

    m_world->getEngine().DrawRenderSkeleton(m_renderSkeleton, 1.0f, (int)Layer::Player);

    dbasic::Console *console = m_world->getEngine().GetConsole();
    console->Clear();
    console->MoveToLocation(dbasic::GuiPoint(1, 2));
    console->SetFontForeColor(0, 0, 0, 1.0f);
    console->SetFontBackColor(0, 0, 0, 0.0f);

    std::stringstream msg;
    ysVector position = RigidBody.GetWorldPosition();
    msg << "Pos " << ysMath::GetX(position) << "/" << ysMath::GetY(position) << "\n";
    msg << "FPS " << m_world->getEngine().GetAverageFramerate() << "\n";
    msg << "Energy: " << m_energy << "\n";
    msg << "AO/DO: " << m_realm->getAliveObjectCount() << "/" << m_realm->getDeadObjectCount() << "\n";
    console->DrawGeneralText(msg.str().c_str());
}

void aw::Player::updateEnergy(float dt) {
    m_energy -= 0.01f * dt;
}
