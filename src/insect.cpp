#include "../include/insect.h"

aw::Insect::Insect() {

}

aw::Insect::~Insect() {

}

void aw::Insect::carry(GameObject *object) {
    m_carryItem = object;
    object->setBeingCarried(true);
}

void aw::Insect::drop() {
    m_carryItem->setBeingCarried(false);
    m_carryItem = nullptr;
}
