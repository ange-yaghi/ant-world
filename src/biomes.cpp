#include "../include/biomes.h"

aw::DesertBiome::DesertBiome() : Biome(Biome::Type::Desert) {
    m_minRequirements.Precipitation = -0.9f;
    m_minRequirements.AverageTemperature = 0.9f;
    m_minRequirements.TemperatureFluctation = 0.0f;
    m_minRequirements.Elevation = 0.0f;
}

aw::DesertBiome::~DesertBiome() {
    /* void */
}

aw::SavannaBiome::SavannaBiome() : Biome(Biome::Type::Savanna) {
    m_minRequirements.Precipitation = -0.5f;
    m_minRequirements.AverageTemperature = 0.8f;
    m_minRequirements.TemperatureFluctation = -0.7f;
    m_minRequirements.Elevation = 0.0f;
}

aw::SavannaBiome::~SavannaBiome() {
    /* void */
}

aw::ContinentalColdBiome::ContinentalColdBiome() : Biome(Biome::Type::ContinentalCold) {
    m_minRequirements.Precipitation = 0.0f;
    m_minRequirements.AverageTemperature = -0.1f;
    m_minRequirements.TemperatureFluctation = 0.8f;
    m_minRequirements.Elevation = 0.0f;
}

aw::ContinentalColdBiome::~ContinentalColdBiome() {
    /* void */
}

aw::ContinentalWarmBiome::ContinentalWarmBiome() : Biome(Biome::Type::ContinentalWarm) {
    m_minRequirements.Precipitation = 0.0f;
    m_minRequirements.AverageTemperature = 0.1f;
    m_minRequirements.TemperatureFluctation = 0.8f;
    m_minRequirements.Elevation = 0.0f;
}

aw::ContinentalWarmBiome::~ContinentalWarmBiome() {
    /* void */
}

aw::ArcticBiome::ArcticBiome() : Biome(Biome::Type::Arctic) {
    m_minRequirements.Precipitation = -0.6f;
    m_minRequirements.AverageTemperature = -1.0f;
    m_minRequirements.TemperatureFluctation = 0.0f;
    m_minRequirements.Elevation = 0.0f;
}

aw::ArcticBiome::~ArcticBiome() {
    /* void */
}

aw::TropicalBiome::TropicalBiome() : Biome(Biome::Type::Tropical) {
    m_minRequirements.Precipitation = 0.9f;
    m_minRequirements.AverageTemperature = 0.8f;
    m_minRequirements.TemperatureFluctation = -1.0f;
    m_minRequirements.Elevation = 0.0f;
}

aw::TropicalBiome::~TropicalBiome() {
    /* void */
}
