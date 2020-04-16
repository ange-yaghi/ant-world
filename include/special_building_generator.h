#ifndef ANT_WORLD_SPECIAL_BUILDING_GENERATOR_H
#define ANT_WORLD_SPECIAL_BUILDING_GENERATOR_H

#include <map>

namespace aw {

    class WorldGrid;

    class SpecialBuildingGenerator {
    public:
        struct Result {
            int buildingX;
            int buildingY;
        };

    public:
        SpecialBuildingGenerator();
        ~SpecialBuildingGenerator();

        bool hasBuilding(int x, int y, WorldGrid *world);

        void setGridSize(int gridSize) { m_gridSize = gridSize; }
        int getGridSize() const { return m_gridSize; }

        void setCollisionHasher(int period, int modulus);
        int getModulus() const { return m_modulus; }
        int getSpacingPeriod() const { return m_spacingPeriod; }

        void setMinCutoff(float cutoff) { m_minCutoff = cutoff; }
        float getMinCutoff() const { return m_minCutoff; }

        float scoreFunction(float populationDensity, int dx, int dy);

        bool isExcluded(int x, int y);

    protected:
        std::map<unsigned __int64, Result> m_cache;

        float m_minCutoff;

        int m_spacingPeriod;
        int m_modulus;
        int m_gridSize;
    };

} /* namespace aw */

#endif /* ANT_WORLD_SPECIAL_BUILDING_GENERATOR_H */
