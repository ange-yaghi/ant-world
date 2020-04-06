#include "../include/food_spawner.h"

#include "../include/world.h"
#include "../include/cookie.h"
#include "../include/mm.h"

#include <algorithm>

aw::FoodSpawner::FoodSpawner() {
    m_radius = 0.0f;
    m_age = 0.0f;
    m_averagePeriod = 0.0f;
    m_lifespan = 0.0f;
    m_type = Type::Undefined;
    m_timer = 0.0f;
    m_currentPeriod = 0.0f;
}

aw::FoodSpawner::~FoodSpawner() {
    /* void */
}

void aw::FoodSpawner::initialize() {
    resetTimer();
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
}

void aw::FoodSpawner::resetTimer() {
    m_timer = 0.0f;
    m_currentPeriod = m_averagePeriod + (0.5f - ysMath::UniformRandom()) * m_averagePeriod;
    if (m_currentPeriod < 0) m_currentPeriod = 0.0f;
}

ysVector aw::FoodSpawner::generateRandomLocation() const {
    float r0 = ysMath::UniformRandom() * m_radius;
    float r1 = ysMath::UniformRandom() * ysMath::Constants::TWO_PI;

    float x = r0 * std::sin(r1);
    float y = r0 * std::cos(r1);

    ysVector delta = ysMath::LoadVector(x, y, 0.0f, 0.0f);
    return ysMath::Add(RigidBody.GetWorldPosition(), delta);
}

ysQuaternion aw::FoodSpawner::generateRandomOrientation() const {
    float r0 = ysMath::UniformRandom() * ysMath::Constants::TWO_PI;
    return ysMath::LoadQuaternion(r0, ysMath::Constants::ZAxis);
}

bool aw::FoodSpawner::checkLocation(const ysVector &vec) const {
    return true;
}

bool aw::FoodSpawner::attemptSpawn() {
    ysVector p = generateRandomLocation();
    if (!checkLocation(p)) return false;

    ysQuaternion o = generateRandomOrientation();
    
    aw::GameObject *newObject = spawnObject();
    if (newObject == nullptr) return false;

    newObject->RigidBody.SetPosition(p);
    newObject->RigidBody.SetOrientation(o);

    return true;
}

aw::GameObject *aw::FoodSpawner::spawnObject() {
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
