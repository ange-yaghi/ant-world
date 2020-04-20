#include "../include/trading_post.h"

#include "../include/world.h"

aw::TradingPost::TradingPost() {
    /* void */
}

aw::TradingPost::~TradingPost() {
    /* void */
}

void aw::TradingPost::initialize() {
    Hole::initialize();
}

void aw::TradingPost::process() {
    Hole::process();
}

void aw::TradingPost::render() {
    int red[] = { 0xFF, 0x0, 0x0 };

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawBox(red, 7.5f, 7.5f, (int)Layer::Holes);
}

void aw::TradingPost::onEnter(GameObject *object) {
    Hole::onEnter(object);
}

void aw::TradingPost::onExit(GameObject *object) {
    Hole::onExit(object);
}

aw::Realm *aw::TradingPost::generateRealm() {
    m_targetRealm = m_world->newRealm<Realm>();
    m_targetRealm->setExitPortal(this);

    InteriorGenerator::WallsParam param;
    param.GridSize = 10.0f;
    param.MaxCells = 40;
    param.MinCells = 20;
    m_generator->generateWalls(m_targetRealm, param);
    m_targetRealm->setIndoor(true);

    return m_targetRealm;
}
