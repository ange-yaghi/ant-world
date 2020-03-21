#ifndef ANT_WORLD_PATHFINDER_H
#define ANT_WORLD_PATHFINDER_H

#include "delta.h"

#include <vector>

namespace aw {

    int clamp(int s, int l, int r);

    class GameObject;

    class Pathfinder {
    public:
        Pathfinder();
        ~Pathfinder();

        void initialize(float gridUnit, int gridWidth);

        ysVector getLocation(int gridX, int gridY) const;
        void translate(const ysVector &v, int &gridX, int &gridY) const;

        void setOrigin(const ysVector &origin) { m_origin = origin; }
        ysVector getOrigin() const { return m_origin; }

        float getGridUnit() const { return m_gridUnit; }
        int getGridWidth() const { return m_gridWidth; }

    protected:
        void populate();

    protected:
        std::vector<GameObject *> m_obstacles;

        std::vector<std::vector<bool>> m_grid;
        float m_gridUnit;
        int m_gridWidth;

        ysVector m_origin;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PATHFINDER_H */
