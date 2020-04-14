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

    object->RigidBody.Transform.SetPosition(
        m_carryPoint->WorldToLocalSpace(
            object->RigidBody.Transform.GetWorldPosition()));
    object->RigidBody.Transform.SetOrientation(
        ysMath::QuatMultiply(
            object->RigidBody.Transform.GetWorldOrientation(), 
            ysMath::QuatInvert(m_carryPoint->GetWorldOrientation())));
    object->RigidBody.Transform.SetParent(m_carryPoint);
    object->RigidBody.SetVelocity(ysMath::Constants::Zero);
    RigidBody.AddChild(&object->RigidBody);

    object->incrementReferenceCount();
}

void aw::Insect::drop() {
    if (m_carryPoint == nullptr) return;

    m_carryItem->setBeingCarried(false);

    dphysics::RigidBody &droppedBody = m_carryItem->RigidBody;

    ysVector position = droppedBody.Transform.GetWorldPosition();
    ysQuaternion orientation = droppedBody.Transform.GetWorldOrientation();

    droppedBody.Transform.SetParent(nullptr);
    droppedBody.Transform.SetPosition(position);
    droppedBody.Transform.SetOrientation(orientation);
    RigidBody.RemoveChild(&droppedBody);

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
