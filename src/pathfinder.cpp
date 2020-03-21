#include "../include/pathfinder.h"

#include "../include/game_object.h"

#include <algorithm>

aw::Pathfinder::Pathfinder() {
    m_gridWidth = 0;
    m_gridUnit = 0;
    m_origin = ysMath::Constants::Zero;
}

aw::Pathfinder::~Pathfinder() {
    /* void */
}

void aw::Pathfinder::initialize(float gridUnit, int gridWidth) {
    m_gridUnit = gridUnit;
    m_gridWidth = gridWidth;

    m_grid = std::vector<std::vector<bool>>(m_gridWidth, std::vector<bool>(m_gridWidth));
}

void aw::Pathfinder::translate(const ysVector &v, int &gridX, int &gridY) const {
    ysVector d = ysMath::Sub(v, m_origin);
    
    float dx = (ysMath::GetX(d) / m_gridUnit) + 0.5f;
    float dy = (ysMath::GetY(d) / m_gridUnit) + 0.5f;

    gridX = clamp((int)std::floor(dx), 0, m_gridWidth - 1);
    gridY = clamp((int)std::floor(dy), 0, m_gridWidth - 1);
}

ysVector aw::Pathfinder::getLocation(int gridX, int gridY) const {
    int offsetX = gridX - m_gridWidth / 2;
    int offsetY = gridY - m_gridWidth / 2;

    ysVector offset = ysMath::LoadVector((float)offsetX * m_gridWidth, (float)offsetY * m_gridWidth, 0.0f, 1.0f);

    return ysMath::Add(offset, m_origin);
}

void aw::Pathfinder::populate() {
    for (int i = 0; i < m_gridWidth; ++i) {
        for (int j = 0; j < m_gridWidth; ++j) {
            m_grid[j][i] = false;
        }
    }

    for (GameObject *object: m_obstacles) {
        int bounds = object->getPathfinderBoundCount();
        for (int i = 0; i < bounds; ++i) {
            int minX, minY;
            int maxX, maxY;

            ysVector minPoint = object->getPathfinderBound(i).minPoint;
            ysVector maxPoint = object->getPathfinderBound(i).maxPoint;

            translate(minPoint, minX, minY);
            translate(maxPoint, maxX, maxY);

            for (int x = minX; x <= maxX; ++x) {
                for (int y = minY; y <= maxY; ++y) {
                    m_grid[y][x] = true;
                }
            }
        }
    }
}

int aw::clamp(int s, int l, int r) {
    if (s >= r) return r;
    else if (s <= l) return l;
    else return s;
}
