#ifndef ANT_WORLD_VEHICLE_H
#define ANT_WORLD_VEHICLE_H

#include "game_object.h"

#include "wheel.h"

namespace aw {

    class Vehicle : public GameObject {
    public:
        Vehicle();
        ~Vehicle();

        virtual void initialize();
        virtual void process();
        virtual void render();

        float m_wheelSpeed = 0.0f;

    protected:
        Wheel *m_flWheel;
        Wheel *m_frWheel;
        Wheel *m_rlWheel;
        Wheel *m_rrWheel;
    };

} /* namespace aw */

#endif /* ANT_WORLD_CAR_H */
