#ifndef ANT_WORLD_WORLD_FRAGMENT_H
#define ANT_WORLD_WORLD_FRAGMENT_H

#include "biome.h"

namespace aw {

    class World;

    class WorldFragment {
    public:
        WorldFragment();
        ~WorldFragment();

        void initialize(int x, int y, float width, float height, Biome::Type biome, const Biome::BiomeParameters &params);

        void debugRender();

        Biome::Type getBiome() const { return m_biome; }
        int getX() const { return m_x; }
        int getY() const { return m_y; }
        float getWidth() const { return m_width; }
        float getHeight() const { return m_height; }

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

    protected:
        int m_x;
        int m_y;

        float m_width;
        float m_height;

        Biome::Type m_biome;
        Biome::BiomeParameters m_parameters;

        World *m_world;
    };

} /* namespace aw */

#endif /* ANT_WORLD_WORLD_FRAGMENT_H */
