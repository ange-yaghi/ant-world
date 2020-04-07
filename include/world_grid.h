#ifndef ANT_WORLD_WORLD_GRID_H
#define ANT_WORLD_WORLD_GRID_H

#include "world_fragment.h"
#include "biomes.h"
#include "generation.h"

#include <map>

namespace aw {

    class WorldGrid {
    public:
        struct FragmentCoord {
            int x;
            int y;
        };

    public:
        WorldGrid();
        ~WorldGrid();

        void initialize(double fragmentSize);

        void debugRender();
        
        void setWorld(World *world);
        World *getWorld() const { return m_world; }

        WorldFragment *requestFragment(const FragmentCoord &coord);

    protected:
        WorldFragment *newFragment(const FragmentCoord &coord);
        Biome::Type getBiomeType(const Biome::BiomeParameters &parameters) const;

        double samplePopulationDensity(double posx, double posy);
        double sampleMicroPopulationDensity(double posx, double posy);

        std::map<unsigned __int64, WorldFragment *> m_fragments;

        PerlinNoiseGenerator m_precipitation;
        PerlinNoiseGenerator m_averageTemperature;
        PerlinNoiseGenerator m_temperatureFluctuation;
        PerlinNoiseGenerator m_elevation;
        PerlinNoiseGenerator m_population;
        PerlinNoiseGenerator m_microPopulation;

        double m_fragmentSize;

    protected:
        std::vector<Biome *> m_biomes;
        World *m_world;
    };

} /* namespace aw */

#endif /* ANT_WORLD_WORLD_GRID_H */
