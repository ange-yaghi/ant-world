#ifndef ANT_WORLD_WHEEL_H
#define ANT_WORLD_WHEEL_H

#include "delta.h"

namespace aw {

    class Wheel : public dphysics::ForceGenerator {
    public:
        static dbasic::ModelAsset *m_wheelModel;

    public:
        Wheel();
        ~Wheel();

        dphysics::RigidBody RigidBody;

        void debugRender();

        virtual void Initialize(dphysics::RigidBody *rigidBody);
        virtual void GenerateForces(float dt);

        ysVector getForwardDirection() const;
        ysVector getShearDirection() const;

        void setCoeffStaticFriction(float u) { m_coeffStaticFriction = u; }
        float getCoeffStaticFriction() const { return m_coeffStaticFriction; }

        void setCoeffDynamicFriction(float u) { m_coeffDynamicFriction = u; }
        float getCoeffDynamicFriction() const { return m_coeffDynamicFriction; }
        
        float getStaticVelocityCorrectionLimit(float dt) const;
        float getDynamicVelocityCorrectionLimit(float dt) const;

        void setEngine(dbasic::DeltaEngine *engine) { m_engine = engine; }
        dbasic::DeltaEngine *getEngine() const { return m_engine; }

        void setRadius(float radius) { m_radius = radius; }
        float getRadius() const { return m_radius; }

        static void configureAssets(dbasic::AssetManager *am);

        void setInputTorque(float inputTorque) { m_inputTorque = inputTorque; }
        void setResistance(float resistance) { m_resistance = resistance; }

        bool isSlipping() const { return m_slipping; }
        

    public:
        float m_radius;

        float m_coeffStaticFriction;
        float m_coeffDynamicFriction;

        float m_inputTorque;
        float m_resistance;

        bool m_slipping;

        dbasic::DeltaEngine *m_engine;
    };

} /* namespace aw */

#endif /* ANT_WORLD_WHEEL_H */
