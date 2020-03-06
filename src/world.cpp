#include "../include/world.h"

#include "../include/player.h"
#include "../include/leaf.h"

aw::World::World() {
    /* void */
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
    spawn<Leaf>();
    spawn<Player>();
}

void aw::World::run() {
    initialSpawn();

    while (m_engine.IsOpen()) {
        frameTick();
    }
}

void aw::World::frameTick() {
    m_engine.StartFrame();

    cleanObjectList();
    spawnObjects();

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
    m_engine.SetCameraPosition(0.0f, 0.0f);
    m_engine.SetCameraAltitude(10.0f);

    for (GameObject *obj : m_gameObjects) {
        obj->render();
    }
}

void aw::World::process() {
    for (GameObject *obj : m_gameObjects) {
        obj->process();
    }
}

void aw::World::spawnObjects() {
    while (!m_spawnQueue.empty()) {
        GameObject *u = m_spawnQueue.front(); m_spawnQueue.pop();
        m_gameObjects.push_back(u);
    }
}

void aw::World::addToSpawnQueue(GameObject *object) {
    m_spawnQueue.push(object);
}

void aw::World::cleanObjectList() {
    int N = m_gameObjects.size();
    int writeIndex = 0;
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->getDeletionFlag()) {
            destroyObject(m_gameObjects[i]);
        }
        else {
            m_gameObjects[writeIndex++] = m_gameObjects[i];
        }
    }
}

void aw::World::destroyObject(GameObject *object) {
    _aligned_free((void *)object);
}
