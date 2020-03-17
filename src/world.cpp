#include "../include/world.h"

#include "../include/player.h"
#include "../include/leaf.h"
#include "../include/hole.h"

aw::World::World() {
    m_mainRealm = nullptr;
}

aw::World::~World() {
    /* void */
}

void aw::World::initialize(void *instance, ysContextObject::DEVICE_API api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../../dependencies/delta/engines/basic";
    std::string assetPath = "../../assets";
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);

        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();

        confFile.close();
    }

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    m_engine.CreateGameWindow(
        "Ant World",
        instance,
        api,
        (enginePath + "/shaders/").c_str());

    m_engine.SetClearColor(0x34, 0x98, 0xdb);

    m_assetManager.SetEngine(&m_engine);
}

void aw::World::initialSpawn() {
    m_mainRealm = newRealm<Realm>();

    Leaf *leaf1 = m_mainRealm->spawn<Leaf>();
    leaf1->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f));

    Leaf *leaf2 = m_mainRealm->spawn<Leaf>();
    leaf2->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 2.0f, 0.0f, 0.0f));

    Hole *hole = m_mainRealm->spawn<Hole>();
    hole->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 5.0f, 0.0f, 0.0f));

    m_player = m_mainRealm->spawn<Player>();
}

void aw::World::run() {
    initialSpawn();

    while (m_engine.IsOpen()) {
        frameTick();
    }
}

void aw::World::frameTick() {
    m_engine.StartFrame();

    process();
    render();

    m_engine.EndFrame();
}

/*
int color[] = { 0xf1, 0xc4, 0x0f };
ysMatrix translation = ysMath::TranslationTransform(ysMath::LoadVector(-3.0f, 0.0f, 0.0f));
m_engine.SetObjectTransform(translation);
m_engine.DrawBox(color, 2.5f, 2.5f);

m_engine.SetMultiplyColor(ysVector4(1.0f, 1.0f, 1.0f, 1.0f));
m_engine.SetObjectTransform(ysMath::LoadIdentity());
m_engine.DrawImage(m_demoTexture, 0, (float)m_demoTexture->GetWidth() / m_demoTexture->GetHeight());
*/

void aw::World::render() {
    ysVector playerPosition = m_player->RigidBody.GetWorldPosition();

    m_engine.SetCameraPosition(ysMath::GetX(playerPosition), ysMath::GetY(playerPosition));
    m_engine.SetCameraAltitude(10.0f);

    m_mainRealm->render();
}

void aw::World::process() {
    m_mainRealm->process();
}
