#ifndef ANT_WORLD_BIOMES_H
#define ANT_WORLD_BIOMES_H

#include "biome.h"

namespace aw {

    class DesertBiome : public Biome {
    public:
        DesertBiome();
        ~DesertBiome();
    };

    class SavannaBiome : public Biome {
    public:
        SavannaBiome();
        ~SavannaBiome();
    };

    class ContinentalWarmBiome : public Biome {
    public:
        ContinentalWarmBiome();
        ~ContinentalWarmBiome();
    };

    class ContinentalColdBiome : public Biome {
    public:
        ContinentalColdBiome();
        ~ContinentalColdBiome();
    };

    class ArcticBiome : public Biome {
    public:
        ArcticBiome();
        ~ArcticBiome();
    };

    class TropicalBiome : public Biome {
    public:
        TropicalBiome();
        ~TropicalBiome();
    };

} /* namespace aw */

#endif /* ANT_WORLD_BIOMES_H */
