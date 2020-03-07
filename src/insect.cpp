#include "../include/insect.h"

aw::Insect::Insect() {
    m_carryItem = nullptr;
}

aw::Insect::~Insect() {
    /* void */
}

void aw::Insect::carry(GameObject *object) {
    m_carryItem = object;
    object->setBeingCarried(true);
}

void aw::Insect::drop() {
    m_carryItem->setBeingCarried(false);
    m_carryItem = nullptr;
}
