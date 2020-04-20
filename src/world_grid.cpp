#include "../include/world_grid.h"

#include "../include/world_fragment.h"
#include "../include/hole.h"
#include "../include/world.h"
#include "../include/trading_post.h"

#include <assert.h>

aw::WorldGrid::WorldGrid() {
    m_fragmentSize = 10.0f;
    m_world = nullptr;
}

aw::WorldGrid::~WorldGrid() {
    /* void */
}

void aw::WorldGrid::initialize(double fragmentSize) {
    m_fragmentSize = fragmentSize;

    m_biomes.push_back(new SavannaBiome);
    m_biomes.push_back(new DesertBiome);
    m_biomes.push_back(new ContinentalWarmBiome);
    m_biomes.push_back(new ContinentalColdBiome);
    m_biomes.push_back(new ArcticBiome);
    m_biomes.push_back(new TropicalBiome);

    int seed = 2001;

    m_precipitation.setScale(1.0 / 100);
    m_precipitation.setSeed(seed);

    m_temperatureFluctuation.setScale(1.0 / 200);
    m_temperatureFluctuation.setSeed(seed + 1);

    m_averageTemperature.setScale(1.0 / 200);
    m_averageTemperature.setSeed(seed + 2);

    m_elevation.setScale(1.0 / 200.0);
    m_elevation.setSeed(seed + 3);

    m_population.setSeed(seed + 4);
    m_population.setScale(1.0 / 200.0f);

    m_microPopulation.setSeed(seed + 5);
    m_microPopulation.setScale(1 / 15.0f);

    m_foodTradeCenter.setGridSize(20);
    m_foodTradeCenter.setCollisionHasher(1, 0);
    m_foodTradeCenter.setMaxAveragePopulation(1.0f);
    m_foodTradeCenter.setMinAveragePopulation(0.5f);
}

void aw::WorldGrid::debugRender() {
    AABB cameraExtents = m_world->getCameraExtents();

    for (auto fragment : m_fragments) {
        if (fragment.second->getBounds().intersects2d(cameraExtents)) {
            fragment.second->debugRender();
        }
    }
}

void aw::WorldGrid::setWorld(World *world) {
    m_world = world;
}

aw::WorldFragment *aw::WorldGrid::newFragment(const FragmentCoord &coord) {
    aw::WorldFragment *newFragment = new aw::WorldFragment;
    
    double posx = coord.x * m_fragmentSize;
    double posy = coord.y * m_fragmentSize;

    Biome::BiomeParameters param;
    param.Precipitation = m_precipitation.perlin(posx, posy);
    param.AverageTemperature = m_averageTemperature.perlin(posx, posy);
    param.TemperatureFluctation = m_temperatureFluctuation.perlin(posx, posy);
    param.Elevation = m_elevation.perlin(posx, posy);
    param.PopulationDensity = samplePopulationDensity(posx, posy);

    Biome::Type type = getBiomeType(param);
    assert(type != Biome::Type::Unknown);

    newFragment->initialize(coord.x, coord.y, m_fragmentSize, m_fragmentSize, type, param);
    newFragment->setWorld(m_world);

    double populationNoise = sampleMicroPopulationDensity(posx, posy);
    bool hasHome = populationNoise + param.PopulationDensity > 1.3f;
    bool hasCommunityCenter = m_foodTradeCenter.hasBuilding(coord.x, coord.y, this);

    if (hasCommunityCenter) {
        TradingPost *newTradingPost = m_world->getMainRealm()->spawn<TradingPost>();

        float holeOffsetX, holeOffsetY;
        holeOffsetX = (ysMath::UniformRandom() - 0.5f) * m_fragmentSize * 0.75f;
        holeOffsetY = (ysMath::UniformRandom() - 0.5f) * m_fragmentSize * 0.75f;
        newTradingPost->RigidBody.Transform.SetPosition(ysMath::LoadVector(
            posx + holeOffsetX, posy + holeOffsetY, 0.0f, 1.0f));
    }
    else if (hasHome) {
        Hole *newHole = m_world->getMainRealm()->spawn<Hole>();

        float holeOffsetX, holeOffsetY;
        holeOffsetX = (ysMath::UniformRandom() - 0.5f) * m_fragmentSize * 0.75f;
        holeOffsetY = (ysMath::UniformRandom() - 0.5f) * m_fragmentSize * 0.75f;
        newHole->RigidBody.Transform.SetPosition(ysMath::LoadVector(
            posx + holeOffsetX, posy + holeOffsetY, 0.0f, 1.0f));
    }

    return newFragment;
}

aw::Biome::Type aw::WorldGrid::getBiomeType(
    const Biome::BiomeParameters &parameters) const 
{
    float minDeviation = 0;
    Biome *closestBiome = nullptr;

    for (Biome *biome : m_biomes) {
        float score = biome->checkBiome(parameters);
        if (closestBiome == nullptr || score < minDeviation) {
            closestBiome = biome;
            minDeviation = score;
        }
    }

    if (closestBiome == nullptr) return Biome::Type::Unknown;
    else return closestBiome->getType();
}

double aw::WorldGrid::samplePopulationDensity(double posx, double posy) {
    double density = m_population.perlin(posx, posy);
    density = (density + 1) / 2.0;

    return density;
}

double aw::WorldGrid::sampleMicroPopulationDensity(double posx, double posy) {
    double noiseFunction = m_microPopulation.perlin(posx, posy);
    noiseFunction = (noiseFunction + 1) / 2.0f;

    return noiseFunction;
}

aw::WorldFragment *aw::WorldGrid::getFragment(double x, double y) {
    FragmentCoord coord;
    coord.x = (x >= 0)
        ? x / m_fragmentSize
        : (x - m_fragmentSize) / m_fragmentSize;
    coord.y = (y >= 0)
        ? y / m_fragmentSize
        : (y - m_fragmentSize) / m_fragmentSize;

    return requestFragment(coord);
}

aw::WorldFragment *aw::WorldGrid::requestFragment(const FragmentCoord &coord) {
    unsigned __int64 hash = PerlinNoiseGenerator::szudzikHash(coord.x, coord.y);

    auto f = m_fragments.find(hash);
    if (f == m_fragments.end()) {
        return m_fragments[hash] = newFragment(coord);
    }
    else return f->second;
}

float aw::WorldGrid::getPopulationDensity(const FragmentCoord &coord) {
    double posx = coord.x * m_fragmentSize;
    double posy = coord.y * m_fragmentSize;

    return (sampleMicroPopulationDensity(posx, posy) + samplePopulationDensity(posx, posy)) / 2.0f;
}
