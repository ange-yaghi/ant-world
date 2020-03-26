#include "../include/npc_ant.h"

#include "../include/world.h"

aw::NpcAnt::NpcAnt() {
    m_sightLine = nullptr;
    m_destination = ysMath::Constants::Zero;
}

aw::NpcAnt::~NpcAnt() {
    /* void */
}

void aw::NpcAnt::initialize() {
    Insect::initialize();

    m_pathfinder.initialize(0.25f, 50);
    setDestination(ysMath::Constants::Zero);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    //RigidBody.SetPosition(ysMath::LoadVector(0.0f, -10.0f, 0.0f));
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

    RigidBody.CollisionGeometry.NewRayObject(&m_sightLine);
    m_sightLine->SetMode(dphysics::CollisionObject::Mode::Sensor);
    m_sightLine->GetAsRay()->MaxDistance = 10.0f;
    m_sightLine->GetAsRay()->RelativeDirection = ysMath::Constants::YAxis;
}

void aw::NpcAnt::process() {
    int collisionCount = RigidBody.GetCollisionCount();
    bool newObstacle = false;
    bool hitSomething = false;
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;
        GameObject *object = reinterpret_cast<GameObject *>(body->GetOwner());

        if (!col->m_sensor) {
            hitSomething = true;
        }

        newObstacle = m_pathfinder.addObstacle(object);
    }

    ysVector position = RigidBody.GetWorldPosition();
    ysVector next;

    bool move = m_pathfinder.update(position, hitSomething, newObstacle, &next);
    if (move) {
        ysVector delta = ysMath::Sub(next, position);
        ysVector heading = ysMath::Normalize(delta);

        float velocity = 5.0f;
        if (m_pathfinder.isEvading()) 
            velocity = 10.0f;

        RigidBody.SetVelocity(ysMath::Mul(heading, ysMath::LoadScalar(velocity)));
    }
    else {
        RigidBody.SetVelocity(ysMath::Constants::Zero);
    }
}

void aw::NpcAnt::render() {
    int color[] = { 0xf1, 0x14, 0x01 };
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 2.5f, 2.5f, (int)Layer::Mob);

    int pathFinderColor[] = { 0x00, 0x00, 0x00 };
    int pathFinderGrey[] = { 0xAA, 0xAA, 0xAA };
    int pathFinderPink[] = { 255, 192, 203 };
    m_world->getEngine().SetObjectTransform(ysMath::TranslationTransform(m_destination));
    m_world->getEngine().DrawBox(pathFinderColor, 0.15f, 0.15f, (int)Layer::Player);

    for (int i = 0; i < m_pathfinder.getPathLength(); ++i) {
        ysVector p = m_pathfinder.getWaypoint(i);
        
        int pathFinderColor[] = { 0x0, 0x0, 0x0 };
        m_world->getEngine().SetObjectTransform(ysMath::TranslationTransform(p));

        int *color = pathFinderColor;
        if (i == m_pathfinder.getCurrentPathIndex()) color = pathFinderPink;
        else if (i < m_pathfinder.getCurrentPathIndex()) color = pathFinderGrey;

        m_world->getEngine().DrawBox(color, 0.05f, 0.05f, (int)Layer::Player);
    }
}

void aw::NpcAnt::setDestination(const ysVector &destination) {
    m_destination = destination;
}

ysVector aw::NpcAnt::getDestination() const {
    return m_destination;
}
