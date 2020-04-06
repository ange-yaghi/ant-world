#ifndef ANT_WORLD_PERLIN_NOISE_H
#define ANT_WORLD_PERLIN_NOISE_H

#include "delta.h"

#include <random>

namespace aw {

    class PerlinNoiseGenerator {
    public:
        struct Vec2 {
            double x;
            double y;
        };

    public:
        PerlinNoiseGenerator();
        ~PerlinNoiseGenerator();

        void setSeed(unsigned int seed);
        unsigned int getSeed() const { return m_seed; }

        void setScale(double scale) { m_scale = scale; }
        double getScale() const { return m_scale; }

        double perlin(double x, double y);
        double perlin(const ysVector2 &p, int offsetX, int offsetY);

        static unsigned __int64 szudzikHash(int x, int y);

    protected:
        double perlin(int x, int y, const Vec2 normalizedOffset);

        unsigned int crushHash(unsigned __int64 hash);
        Vec2 gradient(int x, int y);
        static double lerp(double x, double y, double t);

        double dotGridGradient(int ix, int iy, double x, double y);

        unsigned int m_seed;
        double m_scale;

        std::mt19937_64 m_engine;
        std::uniform_real_distribution<double> m_randomGenerator;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PERLIN_NOISE_H */
