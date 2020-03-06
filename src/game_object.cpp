#include "../include/game_object.h"

#include "../include/world.h"

aw::GameObject::GameObject() {
    m_world = nullptr;
    m_deletionFlag = false;
    m_position = ysMath::Constants::Zero;
    m_heading = ysMath::Constants::Zero;
    m_velocity = ysMath::Constants::Zero;

    m_beingCarried = false;
    m_canBeCarried = false;
}

aw::GameObject::~GameObject() {
    /* void */
}

void aw::GameObject::process() {
    constexpr float EPSILON = 0.0001f;

    if (ysMath::GetScalar(ysMath::MagnitudeSquared3(m_heading)) > EPSILON) {
        m_heading = ysMath::Normalize(m_heading);
    }
    else m_heading = ysMath::Constants::Zero;

    ysVector dp = ysMath::Mul(m_heading, m_velocity);
    dp = ysMath::Mul(dp, ysMath::LoadScalar((float)m_world->getEngine().m_timingSystem->GetFrameDuration()));

    m_position = ysMath::Add(m_position, dp);
}

void aw::GameObject::render() {
    /* void */
}
