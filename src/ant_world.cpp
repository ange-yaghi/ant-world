#include "../include/ant_world.h"

aw::AntWorld::AntWorld() {
    /* void */
}

aw::AntWorld::~AntWorld() {
    /* void */
}

void aw::AntWorld::Initialize(void *instance, ysContextObject::DEVICE_API api) {
    world.initialize(instance, api);
}

void aw::AntWorld::Run() {
    world.run();

    world.getEngine().GetConsole()->Destroy();
    world.getAssetManager().Destroy();
    world.getEngine().Destroy();
}
