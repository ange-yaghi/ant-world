#include "../include/special_building_generator.h"

#include "../include/world_grid.h"

aw::SpecialBuildingGenerator::SpecialBuildingGenerator() {
    m_minCutoff = 0.0f;
    m_gridSize = 1;
    m_modulus = 1;
    m_spacingPeriod = 1;
}

aw::SpecialBuildingGenerator::~SpecialBuildingGenerator() {
    /* void */
}

bool aw::SpecialBuildingGenerator::hasBuilding(int x, int y, WorldGrid *world) {
    int gridX = x >= 0
        ? (x / m_gridSize)
        : ((x - m_gridSize) / m_gridSize);
    int gridY = y >= 0
        ? (y / m_gridSize)
        : ((y - m_gridSize) / m_gridSize);

    int gridOriginX = gridX * m_gridSize;
    int gridOriginY = gridY * m_gridSize;

    int dx = x - gridOriginX;
    int dy = y - gridOriginY;

    unsigned __int64 hash = PerlinNoiseGenerator::szudzikHash(gridX, gridY);
    auto f = m_cache.find(hash);
    if (f != m_cache.end()) {
        Result &cached = f->second;
        return cached.buildingX == dx && cached.buildingY == dy;
    }

    float maxScore = m_minCutoff;
    int bestX = -1, bestY = -1;

    for (int i = 0; i < m_gridSize; ++i) {
        for (int j = 0; j < m_gridSize; ++j) {
            if (isExcluded(i, j)) continue;

            float populationDensity = world->getPopulationDensity({ i + gridOriginX, j + gridOriginY });
            float score = scoreFunction(populationDensity, dx, dy);
            if (score > maxScore) {
                bestX = i;
                bestY = j;
                maxScore = score;
            }
        }
    }

    m_cache[hash] = { bestX, bestY };
    return (dx == bestX && dy == bestY);
}

void aw::SpecialBuildingGenerator::setCollisionHasher(int period, int modulus) {
    m_spacingPeriod = period;
    m_modulus = modulus;
}

float aw::SpecialBuildingGenerator::scoreFunction(float populationDensity, int dx, int dy) {
    float maxD = 2 * (m_gridSize - 1) * (m_gridSize - 1);
    float d = dx * dx + dy * dy;

    return ((1.0f - d / maxD) + populationDensity) / 2.0f;
}

bool aw::SpecialBuildingGenerator::isExcluded(int x, int y) {
    unsigned __int64 hash = PerlinNoiseGenerator::szudzikHash(x, y);
    hash %= m_spacingPeriod;

    return (hash != m_modulus);
}
