#include "../include/insect.h"

aw::Insect::Insect() {
    m_carryItem = nullptr;

    m_carryLocation = ysMath::LoadVector(1.0f, 0.0f, 0.0f, 1.0f);
    m_dropLocation = ysMath::LoadVector(2.0f, 0.0f, 0.0f, 1.0f);
}

aw::Insect::~Insect() {
    /* void */
}

void aw::Insect::carry(GameObject *object) {
    m_carryItem = object;
    object->setBeingCarried(true);
    RigidBody.AddChild(&object->RigidBody);
    object->RigidBody.SetPosition(getCarryLocation());
}

void aw::Insect::drop() {
    m_carryItem->setBeingCarried(false);

    dphysics::RigidBody &droppedBody = m_carryItem->RigidBody;

    ysVector position = RigidBody.GetGlobalSpace(getDropLocation());
    ysQuaternion orientation = droppedBody.GetWorldOrientation();
    RigidBody.RemoveChild(&droppedBody);

    droppedBody.SetPosition(position);
    droppedBody.SetOrientation(orientation);

    m_carryItem = nullptr;
}
