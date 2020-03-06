#include "../include/player.h"

#include "../include/world.h"

aw::Player::Player() {
    /* void */
}

aw::Player::~Player() {
    /* void */
}

void aw::Player::process() {
    m_heading = ysMath::Constants::Zero;
    m_velocity = ysMath::LoadScalar(5.0f);
    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_LEFT)) {
        m_heading = ysMath::Add(m_heading, ysMath::Negate(ysMath::Constants::XAxis));
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_RIGHT)) {
        m_heading = ysMath::Add(m_heading, ysMath::Constants::XAxis);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_UP)) {
        m_heading = ysMath::Add(m_heading, ysMath::Constants::YAxis);
    }

    if (m_world->getEngine().IsKeyDown(ysKeyboard::KEY_DOWN)) {
        m_heading = ysMath::Add(m_heading, ysMath::Negate(ysMath::Constants::YAxis));
    }

    GameObject::process();
}

void aw::Player::render() {
    int color[] = { 0xf1, 0xc4, 0x0f };
    ysMatrix translation = ysMath::TranslationTransform(m_position);
    m_world->getEngine().SetObjectTransform(translation);
    m_world->getEngine().DrawBox(color, 2.5f, 2.5f, (int)Layer::Player);
}
