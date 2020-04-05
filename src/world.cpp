#include "../include/world.h"

#include "../include/player.h"
#include "../include/food.h"
#include "../include/hole.h"
#include "../include/npc_ant.h"
#include "../include/cookie.h"
#include "../include/mm.h"

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

    m_assetPath = dbasic::Path(assetPath);

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    m_engine.CreateGameWindow(
        "Ant World",
        instance,
        api,
        (enginePath + "/shaders/").c_str());

    m_engine.SetClearColor(0xf4, 0xa4, 0x60);

    m_assetManager.SetEngine(&m_engine);

    AssetLoader::loadAllAssets(dbasic::Path(assetPath), &m_assetManager);
}

void aw::World::initialSpawn() {
    m_mainRealm = newRealm<Realm>();

    //Food *leaf1 = m_mainRealm->spawn<Food>();
    //leaf1->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f));

    //Food *leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 2.0f, 0.0f, 0.0f));

    Cookie *cookie = m_mainRealm->spawn<Cookie>();
    cookie->RigidBody.SetPosition(ysMath::LoadVector(5.0f, -2.0f, 0.0f, 1.0f));

    cookie = m_mainRealm->spawn<Cookie>();
    cookie->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f, 1.0f));

    cookie = m_mainRealm->spawn<Cookie>();
    cookie->RigidBody.SetPosition(ysMath::LoadVector(-4.0f, 3.0f, 0.0f, 1.0f));

    MM *mm = m_mainRealm->spawn<MM>();
    mm->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 5.0f, 0.0f, 1.0f));

    mm = m_mainRealm->spawn<MM>();
    mm->RigidBody.SetPosition(ysMath::LoadVector(2.0f, -6.0f, 0.0f, 1.0f));

    //Hole *hole = m_mainRealm->spawn<Hole>();
    //hole->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(-1.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(-2.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(-3.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(1.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(2.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(3.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(4.0f, -5.0f, 0.0f, 0.0f));

    //NpcAnt *npc = m_mainRealm->spawn<NpcAnt>();
    //npc->RigidBody.SetPosition(ysMath::LoadVector(0.0f, -10.0f, 0.0f, 0.0f));

    //npc = m_mainRealm->spawn<NpcAnt>();
    //npc->RigidBody.SetPosition(ysMath::LoadVector(-10.0f, 0.0f, 0.0f, 0.0f));

    //npc = m_mainRealm->spawn<NpcAnt>();
    //npc->RigidBody.SetPosition(ysMath::LoadVector(10.0f, 0.0f, 0.0f, 0.0f));

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

    updateRealms();

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
    m_engine.SetCameraAltitude(30.0f);

    m_player->getRealm()->render();
}

void aw::World::process() {
    m_player->getRealm()->process();
}

void aw::World::updateRealms() {
    for (Realm *realm : m_realms) {
        realm->updateRealms();
    }
}
