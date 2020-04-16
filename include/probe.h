#ifndef ANT_WORLD_PROBE_H
#define ANT_WORLD_PROBE_H

#include "game_object.h"

namespace aw {

    class Probe : public GameObject {
    public:
        Probe();
        ~Probe();

        virtual void initialize();
        virtual void process();
        virtual void render();

        void setRadius(float radius) { m_radius = radius; }
        float getRadius() const { return m_radius; }

        bool isSafe();

    protected:
        float m_radius;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PROBE_H */
