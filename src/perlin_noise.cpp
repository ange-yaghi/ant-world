#include "../include/perlin_noise.h"

aw::PerlinNoiseGenerator::PerlinNoiseGenerator() {
    m_seed = 0;
    m_scale = 1.0f;

    m_randomGenerator = std::uniform_real_distribution<double>(0.0, ysMath::Constants::TWO_PI);
}

aw::PerlinNoiseGenerator::~PerlinNoiseGenerator() {
    /* void */
}

void aw::PerlinNoiseGenerator::setSeed(unsigned int seed) {
    m_seed = seed;
}

double aw::PerlinNoiseGenerator::perlin(double x, double y) {
    x *= m_scale;
    y *= m_scale;

    int x0 = (int)std::floor(x);
    int x1 = x0 + 1;
    int y0 = (int)std::floor(y);
    int y1 = y0 + 1;
    
    Vec2 offset = { x - (double)x0, y - (double)y0 };

    double sx = offset.x;
    double sy = offset.y;

    // Interpolate between grid point gradients
    double n0, n1, ix0, ix1;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);

    return lerp(ix0, ix1, sy);
}

double aw::PerlinNoiseGenerator::perlin(const ysVector2 &p, int offsetX, int offsetY) {
    int x0 = (int)offsetX;
    int y0 = (int)offsetY;

    return 0.0f;
}

double aw::PerlinNoiseGenerator::perlin(int x, int y, const Vec2 normalizedOffset) {
    // Based on Wikipedia article: 
    //  https://en.wikipedia.org/wiki/Perlin_noise

    // Determine grid cell coordinates
    int x0 = x;
    int x1 = x0 + 1;
    int y0 = y;
    int y1 = y0 + 1;

    double sx = normalizedOffset.x;
    double sy = normalizedOffset.y;

    // Interpolate between grid point gradients
    double n0, n1, ix0, ix1;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);

    return lerp(ix0, ix1, sy);
}

aw::PerlinNoiseGenerator::Vec2 aw::PerlinNoiseGenerator::gradient(int x, int y) {
    unsigned __int64 rawHash = szudzikHash(x, y);
    unsigned int crushedHash = crushHash(rawHash);
    unsigned __int64 biasedHash = szudzikHash(crushedHash, m_seed);

    m_engine.seed(biasedHash);

    double angle = m_randomGenerator(m_engine);

    double g_x = std::cos(angle);
    double g_y = std::sin(angle);

    return { g_x, g_y };
}

double aw::PerlinNoiseGenerator::lerp(double x, double y, double t) {
    return x * (1.0 - t) + t * y;
}

unsigned __int64 aw::PerlinNoiseGenerator::szudzikHash(int x, int y) {
    void *data0 = reinterpret_cast<void *>(&x);
    void *data1 = reinterpret_cast<void *>(&y);

    unsigned int *u0 = reinterpret_cast<unsigned int *>(data0);
    unsigned int *u1 = reinterpret_cast<unsigned int *>(data1);

    unsigned __int64 a = (unsigned __int64)(*u0);
    unsigned __int64 b = (unsigned __int64)(*u1);

    return (a >= b)
        ? a * a + a + b
        : b * b + a;
}

unsigned int aw::PerlinNoiseGenerator::crushHash(unsigned __int64 hash) {
    unsigned int upper = (hash >> 32);
    unsigned int lower = hash & 0xFFFFFFFF;

    return upper ^ lower;
}

double aw::PerlinNoiseGenerator::dotGridGradient(int ix, int iy, double x, double y) {
    double dx = x - (double)ix;
    double dy = y - (double)iy;

    Vec2 grad = gradient(ix, iy);

    return dx * grad.x + dy * grad.y;
}
