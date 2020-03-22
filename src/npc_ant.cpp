#include "../include/npc_ant.h"

#include "../include/world.h"

aw::NpcAnt::NpcAnt() {
    m_sightLine = nullptr;
}

aw::NpcAnt::~NpcAnt() {
    /* void */
}

void aw::NpcAnt::initialize() {
    Insect::initialize();

    m_pathfinder.initialize(0.25f, 50);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.SetPosition(ysMath::LoadVector(0.0f, -10.0f, 0.0f));
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
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;
        GameObject *object = reinterpret_cast<GameObject *>(body->GetOwner());

        if (!m_pathfinder.findObstacle(object)) {
            m_pathfinder.addObstacle(object);
        }
    }

    ysVector position = RigidBody.GetWorldPosition();

    std::vector<ysVector> path;
    m_pathfinder.setOrigin(position);
    m_pathfinder.refreshGrid();
    m_pathfinder.pathfind(ysMath::Constants::Zero, path);

    ysVector next = position;
    bool move = false;
    if (!path.empty()) {
        for (ysVector n: path) {
            float mag = ysMath::GetScalar(ysMath::MagnitudeSquared3(ysMath::Sub(n, position)));
            if (mag > 0.1f) {
                next = n;
                move = true;
                break;
            }
        }
    }

    if (move) {
        ysVector heading = ysMath::Normalize(ysMath::Sub(next, position));
        RigidBody.SetVelocity(ysMath::Mul(heading, ysMath::LoadScalar(5.0f)));
    }
    else {
        RigidBody.SetVelocity(ysMath::Constants::Zero);
    }
}

void aw::NpcAnt::render() {
    int color[] = { 0xf1, 0x14, 0x01 };
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().DrawBox(color, 2.5f, 2.5f, (int)Layer::Mob);
}
