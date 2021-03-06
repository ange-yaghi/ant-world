#ifndef ANT_WORLD_WORLD_FRAGMENT_H
#define ANT_WORLD_WORLD_FRAGMENT_H

#include "delta.h"

#include "biome.h"
#include "aabb.h"
#include "game_object.h"

#include <vector>

namespace aw {

    class World;

    class WorldFragment {
    public:
        WorldFragment();
        ~WorldFragment();

        void initialize(int x, int y, float width, float height, Biome::Type biome, const Biome::BiomeParameters &params);

        ysVector2 getPosition() const;

        void debugRender();

        Biome::Type getBiome() const { return m_biome; }
        int getX() const { return m_x; }
        int getY() const { return m_y; }
        float getWidth() const { return m_width; }
        float getHeight() const { return m_height; }

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        static void configureAssets(dbasic::AssetManager *am);

        AABB getBounds() const;

        bool isLoaded() const { return m_loaded; }
        void unload();
        void load();

        void addFixture(GameObject *fixture);

    protected:
        int m_x;
        int m_y;

        float m_width;
        float m_height;
        float m_blockAngle;
        float m_blockScale;

        Biome::Type m_biome;
        Biome::BiomeParameters m_parameters;

        World *m_world;

    protected:
        static dbasic::ModelAsset *m_terrainBlock;
        static dbasic::TextureAsset *m_terrainTexture;

    protected:
        bool m_loaded;
        std::vector<GameObject *> m_fixtures;
    };

} /* namespace aw */

#endif /* ANT_WORLD_WORLD_FRAGMENT_H */
