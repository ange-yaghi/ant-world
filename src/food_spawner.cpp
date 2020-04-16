#include "../include/food_spawner.h"

#include "../include/world.h"
#include "../include/cookie.h"
#include "../include/mm.h"
#include "../include/probe.h"

#include <algorithm>

aw::FoodSpawner::FoodSpawner() {
    m_radius = 0.0f;
    m_age = 0.0f;
    m_averagePeriod = 0.0f;
    m_lifespan = 0.0f;
    m_type = Type::Undefined;
    m_timer = 0.0f;
    m_currentPeriod = 0.0f;
    m_unitRadius = 0.0f;
}

aw::FoodSpawner::~FoodSpawner() {
    /* void */
}

void aw::FoodSpawner::initialize() {
    GameObject::initialize();

    resetTimer();

    // TEMP
    switch (m_type) {
    case Type::Cookie:
        m_unitRadius = 2.0f;
        break;
    case Type::MM:
        m_unitRadius = 1.0f;
        break;
    }
}

void aw::FoodSpawner::render() {
    /* void */
}

void aw::FoodSpawner::process() {
    m_timer += m_world->getEngine().GetFrameLength();
    if (m_timer > m_currentPeriod && m_currentPeriod > 0) {
        bool attemptSuccessful = attemptSpawn();
        if (attemptSuccessful) {
            resetTimer();
        }
    }

    while (!m_spawnAttempts.empty()) {
        Probe *u = m_spawnAttempts.front();
        if (!u->isReal()) break;

        m_spawnAttempts.pop();
        if (!u->isSafe()) {
            u->setDeletionFlag();
        }
        else {
            aw::GameObject *newSpawn = spawnReal();
            ysQuaternion o = generateRandomOrientation();

            newSpawn->RigidBody.Transform.SetPosition(u->RigidBody.Transform.GetWorldPosition());
            newSpawn->RigidBody.Transform.SetOrientation(o);
        }
    }
}

void aw::FoodSpawner::resetTimer() {
    m_timer = 0.0f;
    m_currentPeriod = m_averagePeriod + (0.5f - ysMath::UniformRandom()) * m_averagePeriod;
    if (m_currentPeriod < 0) m_currentPeriod = 0.0f;
}

ysVector aw::FoodSpawner::generateRandomLocation() {
    float r0 = ysMath::UniformRandom() * m_radius;
    float r1 = ysMath::UniformRandom() * ysMath::Constants::TWO_PI;

    float x = r0 * std::sin(r1);
    float y = r0 * std::cos(r1);

    ysVector delta = ysMath::LoadVector(x, y, 0.0f, 0.0f);
    return ysMath::Add(RigidBody.Transform.GetWorldPosition(), delta);
}

ysQuaternion aw::FoodSpawner::generateRandomOrientation() const {
    float r0 = ysMath::UniformRandom() * ysMath::Constants::TWO_PI;
    return ysMath::LoadQuaternion(r0, ysMath::Constants::ZAxis);
}

bool aw::FoodSpawner::attemptSpawn() {
    ysVector p = generateRandomLocation();
    
    aw::Probe *probe = newProbe(p, m_unitRadius);
    if (probe == nullptr) return false;

    m_spawnAttempts.push(probe);

    return true;
}

aw::Probe *aw::FoodSpawner::newProbe(const ysVector &location, float radius) {
    aw::Probe *probe = m_realm->spawn<aw::Probe>();
    if (probe == nullptr) return nullptr;

    probe->RigidBody.Transform.SetPosition(location);
    probe->setRadius(radius);
}

aw::GameObject *aw::FoodSpawner::spawnReal() {
    if (m_type == Type::Cookie) {
        Cookie *newCookie = m_realm->spawn<Cookie>();
        if (newCookie == nullptr) return nullptr;

        return newCookie;
    }
    else if (m_type == Type::MM) {
        MM *newMM = m_realm->spawn<MM>();
        if (newMM == nullptr) return nullptr;

        return newMM;
    }
    else return nullptr;
}
