#include "../include/leaf.h"

#include "../include/world.h"

aw::Leaf::Leaf() {
    setCanBeCarried(true);
}

aw::Leaf::~Leaf() {
    /* void */
}

void aw::Leaf::process() {
    /* void */
}

void aw::Leaf::render() {
    int color[] = { 0xc7, 0xea, 0x46 };
    ysMatrix translation = ysMath::TranslationTransform(m_position);
    m_world->getEngine().SetObjectTransform(translation);
    m_world->getEngine().DrawBox(color, 1.0f, 1.0f, (int)Layer::Items);
    
}
