#include "../include/player.h"

#include "../include/world.h"
#include "../include/hole.h"
#include "../include/math.h"
#include "../include/container.h"

#include <sstream>

ysAnimationAction
    *aw::Player::AnimBlink = nullptr,
    *aw::Player::AnimIdle = nullptr,
    *aw::Player::AnimWalk = nullptr,
    *aw::Player::PoseFaceDown = nullptr,
    *aw::Player::PoseFaceUp = nullptr, 
    *aw::Player::PoseFaceLeft = nullptr, 
    *aw::Player::PoseFaceRight = nullptr,
    *aw::Player::AnimSidestep = nullptr;

dbasic::SceneObjectAsset *aw::Player::CharacterRoot = nullptr;

aw::Player::Player() {
    m_energy = 0.0f;

    m_antennaChannel = nullptr;
    m_bodyRotationChannel = nullptr;
    m_eyelidChannel = nullptr;
    m_legsChannel = nullptr;
    m_renderSkeleton = nullptr;
}

aw::Player::~Player() {
    /* void */
}

void aw::Player::initialize() {
    Insect::initialize();
    addTag(Tag::Player);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.Transform.SetPosition(ysMath::LoadVector(-490.0f, 276.0, 0.0f));
    RigidBody.SetAlwaysAwake(true);
    RigidBody.SetRequestsInformation(true);
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.5f;
    bounds->GetAsBox()->HalfWidth = 1.5f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->Radius = 5.0f;
    //RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    //bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    //bounds->GetAsCircle()->RadiusSquared = 1.25 * 1.25;

    m_renderSkeleton = m_world->getAssetManager().BuildRenderSkeleton(
        &RigidBody.Transform, CharacterRoot);

    m_renderSkeleton->BindAction(AnimBlink, &m_animBlink);
    m_renderSkeleton->BindAction(AnimWalk, &m_animWalk);
    m_renderSkeleton->BindAction(AnimIdle, &m_animIdle);
    m_renderSkeleton->BindAction(AnimSidestep, &m_animSidestep);
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

    ysTransform *mouth = &m_renderSkeleton->GetNode("Mouth")->Transform;
    setCarryPoint(mouth);

    m_energy = 10.0f;
}

void aw::Player::grab() {
    constexpr float MouthTolerance = 0.0f;

    ysVector mouthLocation = getCarryPoint()->GetWorldPosition();
    mouthLocation = ysMath::Mask(mouthLocation, ysMath::Constants::MaskOffZ);

    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;

        GameObject *object = reinterpret_cast<GameObject *>(body->GetOwner());
        if (object->hasTag(Tag::Carryable) && !object->isBeingCarried()) {
            ysVector position = object->getPickupPointWorld();
            if (inRange(mouthLocation, position, object->getPickupRadius() + MouthTolerance)) {
                carry(object);
                break;
            }
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

bool aw::Player::enterHole() {
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

            return true;
        }
    }

    return false;
}

void aw::Player::exitHole() {
    Realm *currentRealm = getRealm();
    GameObject *exitPortal = currentRealm->getExitPortal();
    
    if (exitPortal == nullptr) return;
    
    changeRealm(exitPortal->getRealm());
    setLastPortal(exitPortal);
}

bool aw::Player::enterVehicle() {
    int collisions = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisions; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;

        GameObject *object = (GameObject *)body->GetOwner();
        if (object->hasTag(Tag::Beetle)) {
            Realm *targetRealm = object->getTargetRealm();

            if (targetRealm == nullptr) {
                targetRealm = object->generateRealm();
            }

            changeRealm(targetRealm);
            setLastPortal(object);

            return true;
        }
    }

    return false;
}

void aw::Player::exitVehicle() {
    Realm *currentRealm = getRealm();
    GameObject *exitPortal = currentRealm->getExitPortal();

    if (exitPortal == nullptr) return;

    changeRealm(exitPortal->getRealm());
    setLastPortal(exitPortal);
}

void aw::Player::enter() {
    if (enterVehicle()) return;
    if (enterHole()) return;
}

void aw::Player::exit() {
    exitVehicle();
    exitHole();
}

void aw::Player::updateMotion() {
    ysVector heading = ysMath::Constants::Zero;
    ysVector velocity = ysMath::LoadScalar(5.0f);

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_SHIFT)) {
        velocity = ysMath::LoadScalar(2.5f);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_CONTROL)) {
        velocity = ysMath::LoadScalar(50.0f);
    }

    if (isCarryingItem()) {
        velocity = ysMath::LoadScalar(4.0f);
    }

    bool moving = false;
     
    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_A)) {
        heading = ysMath::Add(heading, ysMath::Negate(ysMath::Constants::XAxis));
        moving = true;
    }

    else if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_D)) {
        heading = ysMath::Add(heading, ysMath::Constants::XAxis);
        moving = true;
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_W)) {
        heading = ysMath::Add(heading, ysMath::Constants::YAxis);
        moving = true;
    }

    else if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_S)) {
        heading = ysMath::Add(heading, ysMath::Negate(ysMath::Constants::YAxis));
        moving = true;
    }
    
    if (moving) {
        heading = ysMath::Normalize(heading);
    }

    //ysVector vel = RigidBody.GetGlobalSpace(ysMath::Mul(ysMath::Constants::YAxis, velocity));
    if (moving) RigidBody.SetVelocity(ysMath::Mul(heading, velocity));
}

void aw::Player::updateAnimation() {
    float velocity = ysMath::GetScalar(ysMath::Magnitude(RigidBody.GetVelocity()));
    float unitVelocity = 4.0f;
    float relVelocity = velocity / unitVelocity;

    float x = ysMath::GetX(RigidBody.GetVelocity());
    float y = ysMath::GetY(RigidBody.GetVelocity());
    if (!m_world->getEngine().IsKeyDown(ysKeyboard::KEY_SHIFT) && !isCarryingItem()) {
        ysAnimationChannel::ActionSettings rotationSettings;
        rotationSettings.FadeIn = 7.0f;
        rotationSettings.Speed = 0.0f;

        ysAnimationActionBinding *newOrientation = nullptr;
        if (std::abs(x) > std::abs(y)) {
            if (x < 0) {
                newOrientation = &m_faceLeft;
            }
            else if (x > 0) {
                newOrientation = &m_faceRight;
            }
        }
        else {
            if (y < 0) {
                newOrientation = &m_faceDown;
            }
            else if (y > 0) {
                newOrientation = &m_faceUp;
            }
        }

        if (newOrientation != nullptr) {
            if (m_bodyRotationChannel->GetCurrentAction() != newOrientation) {
                m_bodyRotationChannel->AddSegment(newOrientation, rotationSettings);
            }
        }
    }

    bool sideStepping = false;
    bool walkingBackwards = false;
    float sideSteppingSpeed = 0.0f;
    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_SHIFT) || isCarryingItem()) {
        if (m_bodyRotationChannel->GetCurrentAction() == &m_faceUp) {
            if (x > 0) { sideStepping = true; sideSteppingSpeed = 1.0f; }
            else if (x < 0) { sideStepping = true; sideSteppingSpeed = -1.0f; }

            if (y < 0) walkingBackwards = true;
        }
        else if (m_bodyRotationChannel->GetCurrentAction() == &m_faceDown) {
            if (x > 0) { sideStepping = true; sideSteppingSpeed = -1.0f; }
            else if (x < 0) { sideStepping = true; sideSteppingSpeed = 1.0f; }

            if (y > 0) walkingBackwards = true;
        }
        else if (m_bodyRotationChannel->GetCurrentAction() == &m_faceLeft) {
            if (y > 0) { sideStepping = true; sideSteppingSpeed = 1.0f; }
            else if (y < 0) { sideStepping = true; sideSteppingSpeed = -1.0f; }
        }
        else if (m_bodyRotationChannel->GetCurrentAction() == &m_faceRight) {
            if (y > 0) { sideStepping = true; sideSteppingSpeed = -1.0f; }
            else if (y < 0) { sideStepping = true; sideSteppingSpeed = 1.0f; }
        }
    }

    sideSteppingSpeed *= 1.5f;
    
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
        if (walkingBackwards) {
            relVelocity *= -1;
        }

        if (!sideStepping) {
            if (m_legsChannel->GetSpeed() != relVelocity) {
                m_legsChannel->ChangeSpeed(relVelocity);
                m_legsChannel->ClearQueue();
            }

            ysAnimationChannel::ActionSettings settings;
            settings.FadeIn = 10.0f / std::abs(relVelocity);
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
        else {
            if (m_legsChannel->GetSpeed() != sideSteppingSpeed * relVelocity) {
                m_legsChannel->ChangeSpeed(sideSteppingSpeed * relVelocity);
                m_legsChannel->ClearQueue();
            }

            ysAnimationChannel::ActionSettings settings;
            settings.FadeIn = 10.0f / std::abs(relVelocity);
            settings.Speed = sideSteppingSpeed * relVelocity;

            if (m_legsChannel->GetCurrentAction() != &m_animSidestep) {
                m_legsChannel->AddSegment(&m_animSidestep, settings);
                m_legsChannel->ClearQueue();
            }
            else if (!m_legsChannel->HasQueuedSegments()) {
                settings.FadeIn = 0.0f;
                m_legsChannel->QueueSegment(&m_animSidestep, settings);
            }
        }
    }

    RigidBody.UpdateDerivedData(true); // TODO: could remove this call?
    m_renderSkeleton->UpdateAnimation(m_world->getEngine().GetFrameLength() * 60.0f);
}

void aw::Player::updateAnimationSmooth() {
    float velocity = ysMath::GetScalar(ysMath::Magnitude(RigidBody.GetVelocity()));
    float unitVelocity = 4.0f;
    float relVelocity = velocity / unitVelocity;

    if (m_legsChannel->GetSpeed() != relVelocity) {
        m_legsChannel->ChangeSpeed(relVelocity);
        m_legsChannel->ClearQueue();
    }

    ysAnimationChannel::ActionSettings settings;
    settings.FadeIn = 10.0f / std::abs(relVelocity);
    settings.Speed = relVelocity;

    if (m_legsChannel->GetCurrentAction() != &m_animWalk) {
        m_legsChannel->AddSegment(&m_animWalk, settings);
        m_legsChannel->ClearQueue();
    }
    else if (!m_legsChannel->HasQueuedSegments()) {
        settings.FadeIn = 0.0f;
        m_legsChannel->QueueSegment(&m_animWalk, settings);
    }

    RigidBody.UpdateDerivedData(true); // TODO: could remove this call?
    m_renderSkeleton->UpdateAnimation(m_world->getEngine().GetFrameLength() * 60.0f);
}

void aw::Player::updateMotionSmooth() {
    ysVector force = ysMath::Constants::Zero;

    RigidBody.SetLinearDamping(0.0001f);

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_SHIFT)) {

    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_A)) {
        force = ysMath::LoadVector(0.0f, 1.0f, 0.0f);
        RigidBody.AddAngularVelocity(ysMath::LoadVector(0.0f, 0.0f, -0.2f));
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_D)) {
        force = ysMath::LoadVector(0.0f, 1.0f, 0.0f);
        RigidBody.AddAngularVelocity(ysMath::LoadVector(0.0f, 0.0f, 0.2f));
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_W)) {
        force = ysMath::LoadVector(0.0f, 10.0f, 0.0f);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_S)) {
        force = ysMath::LoadVector(0.0f, -10.0f, 0.0f);
    }

    //ysVector vel = RigidBody.GetGlobalSpace(ysMath::Mul(ysMath::Constants::YAxis, velocity));
    RigidBody.AddForceLocalSpace(force, ysMath::Constants::Zero);

    ysVector velocity = RigidBody.GetVelocity();
    RigidBody.AddForceWorldSpace(ysMath::Mul(ysMath::LoadScalar(1.0f), ysMath::Negate(velocity)), ysMath::Constants::Zero);
}

void aw::Player::interactWithBoxes() {
    int collisionCount = RigidBody.GetCollisionCount();
    float closestBoxDistance = 0.0f;
    GameObject *closestBox = nullptr;

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;

        GameObject *object = reinterpret_cast<GameObject *>(body->GetOwner());
        if (object->hasTag(Tag::Container) && !object->isBeingCarried()) {
            float distance = aw::distance(
                object->RigidBody.Transform.GetWorldPosition(), 
                RigidBody.Transform.GetWorldPosition());
            if (closestBox == nullptr || distance < closestBoxDistance) {
                closestBox = object;
                closestBoxDistance = distance;
            }
        }
    }

    if (closestBox != nullptr) {
        GameObject *carriedObject = getCarriedItem();

        Container *box = static_cast<Container *>(closestBox);

        if (carriedObject != nullptr) {
            if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_DOWN)) {
                drop();
                box->load(carriedObject);
            }
        }
        else {
            if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_UP)) {
                GameObject *object = box->unload(0);
                object->setGraceMode(true);

                ysVector localPosition = ysMath::LoadVector(0.0f, object->getPickupRadius() - 0.1f, 0.0f, 0.0f);
                carry(object);
                object->RigidBody.Transform.SetPosition(localPosition);
            }
        }
    }
}

void aw::Player::configureAssets(dbasic::AssetManager *am) {
    AnimBlink = am->GetAction("Blink");
    AnimIdle = am->GetAction("Idle");
    AnimWalk = am->GetAction("Walk");
    AnimSidestep = am->GetAction("SideStep");
    PoseFaceUp = am->GetAction("FaceUp");
    PoseFaceDown = am->GetAction("FaceDown");
    PoseFaceLeft = am->GetAction("FaceLeft");
    PoseFaceRight = am->GetAction("FaceRight");

    AnimBlink->SetLength(40.0f);
    AnimIdle->SetLength(100.0f);
    AnimWalk->SetLength(30.0f);
    AnimSidestep->SetLength(30.0f);
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
        enter();
    }

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_O)) {
        exit();
    }

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_E)) {
        eat();
    }

    updateEnergy(m_world->getEngine().GetFrameLength());
    updateMotion();
    updateAnimation();
    interactWithBoxes();
}

void aw::Player::render() {
    int color[] = { 0xf1, 0xc4, 0x0f };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    //m_world->getEngine().DrawBox(color, 5.0f, 5.0f, (int)Layer::Player);

    m_world->getEngine().DrawRenderSkeleton(m_renderSkeleton, 1.0f, (int)Layer::Player);

    dbasic::Console *console = m_world->getEngine().GetConsole();
    console->Clear();
    console->MoveToLocation(dbasic::GuiPoint(1, 2));
    console->SetFontForeColor(0, 0, 0, 1.0f);
    console->SetFontBackColor(0, 0, 0, 0.0f);

    std::stringstream msg;
    ysVector position = RigidBody.Transform.GetWorldPosition();
    msg << "Pos " << ysMath::GetX(position) << "/" << ysMath::GetY(position) << "\n";
    msg << "FPS " << m_world->getEngine().GetAverageFramerate() << "\n";
    msg << "Energy: " << m_energy << "\n";
    msg << "AO/DO/VI: " << m_realm->getAliveObjectCount() << "/" << m_realm->getDeadObjectCount() << "/" << m_realm->getVisibleObjectCount() << "\n";
    console->DrawGeneralText(msg.str().c_str());
}

void aw::Player::updateEnergy(float dt) {
    m_energy -= 0.01f * dt;
}
