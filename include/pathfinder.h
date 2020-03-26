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
        void translate(const ysVector &v, int &gridX, int &gridY, bool clampOutput = true) const;

        void setOrigin(const ysVector &origin) { m_origin = origin; }
        ysVector getOrigin() const { return m_origin; }

        float getGridUnit() const { return m_gridUnit; }
        int getGridWidth() const { return m_gridWidth; }

        int getObstacleCount() const { return (int)m_obstacles.size(); }
        bool findObstacle(GameObject *obstacle);
        bool isInRange(GameObject *obstacle);
        bool addObstacle(GameObject *obstacle);
        void clearObstacles();
        void refreshGrid();

        bool update(const ysVector &currentLocation, bool collisionDetected, bool newObstacle, ysVector *nextTarget);

        void clearPath();
        int getPathLength() const { return (int)m_path.size(); }
        int getCurrentPathIndex() const { return m_pathIndex; }
        ysVector getWaypoint(int index) { return getLocation(m_path[index]); }

        bool isEvading() const { return m_evading; }

    protected:
        bool evade(std::vector<int> &path) const;
        bool pathfind(const ysVector &destination, std::vector<int> &path) const;

        void removeObstacle(int index);

    protected:
        int encode(int x, int y) const { return x + y * m_gridWidth; };
        void decode(int d, int &x, int &y) const { x = (d % m_gridWidth); y = d / m_gridWidth; };

        ysVector getLocation(int encodedIndex) const;

    protected:
        std::vector<GameObject *> m_obstacles;

        bool **m_grid;
        float m_gridUnit;
        int m_gridWidth;

        ysVector m_origin;
        ysVector m_destination;

    protected:
        std::vector<int> m_path;
        int m_pathIndex;

        bool m_evading;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PATHFINDER_H */
