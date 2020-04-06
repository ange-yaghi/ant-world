#include "../include/biome.h"

#include <algorithm>
#include <math.h>

aw::Biome::Biome(Type type) {
    m_type = type;
}

aw::Biome::~Biome() {
    /* void */
}

float aw::Biome::checkBiome(const BiomeParameters &parameters) const {
    float d0 = 0, d1 = 0, d2 = 0, d3 = 0;

    /*
    if (parameters.AverageTemperature < m_minRequirements.AverageTemperature) {
        d0 = m_minRequirements.AverageTemperature - parameters.AverageTemperature;
    }
    else if (parameters.AverageTemperature > m_maxRequirements.AverageTemperature) {
        d0 = parameters.AverageTemperature - m_maxRequirements.AverageTemperature;
    }

    if (parameters.Elevation < m_minRequirements.Elevation) {
        d1 = m_minRequirements.Elevation - parameters.Elevation;
    }
    else if (parameters.Elevation > m_maxRequirements.Elevation) {
        d1 = parameters.Elevation - m_maxRequirements.Elevation;
    }

    if (parameters.Precipitation < m_minRequirements.Precipitation) {
        d2 = m_minRequirements.Precipitation - parameters.Precipitation;
    }
    else if (parameters.Precipitation > m_maxRequirements.Precipitation) {
        d2 = parameters.Precipitation - m_maxRequirements.Precipitation;
    }

    if (parameters.TemperatureFluctation < m_minRequirements.TemperatureFluctation) {
        d3 = m_minRequirements.TemperatureFluctation - parameters.TemperatureFluctation;
    }
    else if (parameters.TemperatureFluctation > m_maxRequirements.TemperatureFluctation) {
        d3 = parameters.TemperatureFluctation - m_maxRequirements.TemperatureFluctation;
    }
    */

    d0 = std::abs(parameters.TemperatureFluctation - m_minRequirements.TemperatureFluctation);
    d1 = std::abs(parameters.AverageTemperature - m_minRequirements.AverageTemperature);
    d2 = std::abs(parameters.Elevation - m_minRequirements.Elevation);
    d3 = std::abs(parameters.Precipitation - m_minRequirements.Precipitation);

    return pow(d0, 0.5f) + pow(d1, 0.5f) + pow(d2, 0.5f) + pow(d3, 0.5f);
}
