#include "../include/insect.h"

aw::Insect::Insect() {
    m_carryItem = nullptr;

    m_carryPoint = nullptr;
    m_dropLocation = ysMath::LoadVector(2.0f, 0.0f, 0.0f, 1.0f);
}

aw::Insect::~Insect() {
    /* void */
}

void aw::Insect::carry(GameObject *object) {
    if (m_carryPoint == nullptr) return;

    m_carryItem = object;
    object->setBeingCarried(true);
    m_carryPoint->UpdateDerivedData(true);

    object->RigidBody.SetPosition(m_carryPoint->GetLocalSpace(ysMath::ExtendVector(object->RigidBody.GetWorldPosition())));
    object->RigidBody.SetOrientation(ysMath::QuatMultiply(object->RigidBody.GetWorldOrientation(), ysMath::QuatInvert(m_carryPoint->GetWorldOrientation())));
    m_carryPoint->AddChild(&object->RigidBody);

    object->incrementReferenceCount();
}

void aw::Insect::drop() {
    if (m_carryPoint == nullptr) return;

    m_carryItem->setBeingCarried(false);

    dphysics::RigidBody &droppedBody = m_carryItem->RigidBody;

    ysVector position = droppedBody.GetWorldPosition();
    ysQuaternion orientation = droppedBody.GetWorldOrientation();
    m_carryPoint->RemoveChild(&droppedBody);

    droppedBody.SetPosition(position);
    droppedBody.SetOrientation(orientation);

    m_carryItem->decrementReferenceCount();
    m_carryItem = nullptr;
}

void aw::Insect::initialize() {
    GameObject::initialize();
    addTag(Tag::Insect);
}

void aw::Insect::changeRealm(Realm *newRealm) {
    GameObject::changeRealm(newRealm);

    if (isCarryingItem()) {
        m_carryItem->changeRealm(newRealm);
    }
}

void aw::Insect::process() {
    GameObject::process();
}
