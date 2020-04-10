#ifndef DELTA_BASIC_RIGID_BODY_SYSTEM_H
#define DELTA_BASIC_RIGID_BODY_SYSTEM_H

#include "delta_core.h"

#include "collision_object.h"
#include "rigid_body.h"
#include "collision_detector.h"
#include "rigid_body_link.h"
#include "grid_partition_system.h"

#include <Windows.h>
#include <fstream>

namespace dphysics {

    class DeltaEngine;
    class RigidBodySystem : public ysObject {
    public:
        static const int ResolutionIterationLimit = 1000000;
        static float ResolutionPenetrationEpsilon;

        struct CollisionGenerationCallData {
            RigidBodySystem *System;
            int Start;
            int Count;
            int ThreadID;
        };

    public:
        RigidBodySystem();
        ~RigidBodySystem();

        // Collision Detector Interface
        CollisionDetector CollisionDetector;

        void RegisterRigidBody(RigidBody *body);
        void RemoveRigidBody(RigidBody *body);

        void Update(float timeStep);

        template<typename TYPE>
        TYPE *CreateLink(RigidBody *body1, RigidBody *body2) {
            TYPE *newLink = m_rigidBodyLinks.NewGeneric<TYPE>(16);
            newLink->SetRigidBodies(body1, body2);
            return newLink;
        }

        void DeleteLink(RigidBodyLink *link);

        void ProcessGridCell(int x, int y);

    protected:
        void GenerateCollisions();
        void GenerateCollisions(RigidBody *body1, RigidBody *body2);

        void ResolveCollisions();
        void ResolveCollision(Collision *collision, ysVector *velocityChange, ysVector *rotationDirection, float rotationAmount[2], float penetration);

        void GenerateForces(float timeStep);
        void Integrate(float timeStep);
        void UpdateDerivedData();
        void CheckAwake();

        void OrderPrimitives(CollisionObject **prim1, CollisionObject **prim2, RigidBody **body1, RigidBody **body2);

        void GenerateCollisions(int start, int count);

    protected:
        ysRegistry<RigidBody, 512> m_rigidBodyRegistry;

        ysDynamicArray<RigidBodyLink, 512> m_rigidBodyLinks;

        ysDynamicArray<Collision, 4> m_dynamicCollisions;
        ysExpandingArray<Collision *, 8192> m_collisionAccumulator;

        int m_lastLoadMeasurement;
        int m_loadMeasurement;
        float m_currentStep;

        // TEST
        GridPartitionSystem m_gridPartitionSystem;
        std::ofstream m_loggingOutput;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_RIGID_BODY_SYSTEM_H */
