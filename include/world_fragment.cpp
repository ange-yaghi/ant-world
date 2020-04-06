#include "../include/world_fragment.h"

#include "../include/world.h"
#include "../include/game_object.h"

aw::WorldFragment::WorldFragment() {
    m_x = 0;
    m_y = 0;
    m_width = 0.0f;
    m_height = 0.0f;
    m_biome = Biome::Type::Unknown;
    m_world = nullptr;
}

aw::WorldFragment::~WorldFragment() {
    /* void */
}

void aw::WorldFragment::initialize(int x, int y, float width, float height, Biome::Type biome, const Biome::BiomeParameters &params) {
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_biome = biome;
    m_parameters = params;
}

void aw::WorldFragment::debugRender() {
    const int arcticColor[] = {255, 255, 255};
    const int tropicalColor[] = { 57, 255, 20 };
    const int savannaColor[] = { 130, 108, 52 };
    const int continentalColdColor[] = { 11, 102, 35 };
    const int continentalWarmColor[] = { 50, 152, 35 };
    const int desertColor[] = { 237, 201, 175 };
    const int unknownColor[] = { 255, 0, 0 };

    const int *color = unknownColor;
    switch (m_biome) {
    case Biome::Type::Arctic:
        color = arcticColor;
        break;
    case Biome::Type::ContinentalWarm:
        color = continentalWarmColor;
        break;
    case Biome::Type::ContinentalCold:
        color = continentalColdColor;
        break;
    case Biome::Type::Savanna:
        color = savannaColor;
        break;
    case Biome::Type::Desert:
        color = desertColor;
        break;
    case Biome::Type::Tropical:
        color = tropicalColor;
        break;
    }

    const int precColor[] = {((m_parameters.Precipitation + 1.0) / 2.0) * 255, 0, 0};
    const int elevColor[] = { ((m_parameters.Elevation + 1.0) / 2.0) * 255, 0, 0 };
    const int avgTempColor[] = { ((m_parameters.AverageTemperature + 1.0) / 2.0) * 255, 0, 0 };
    const int tempFlucColor[] = { ((m_parameters.TemperatureFluctation + 1.0) / 2.0) * 255, 0, 0 };
    //color = precColor;

    ysMatrix transform = ysMath::TranslationTransform(
        ysMath::LoadVector(m_x * m_width, m_y * m_height, 0.0f, 1.0f));

    m_world->getEngine().SetObjectTransform(transform);
    m_world->getEngine().DrawBox(color, m_width, m_height, (int)GameObject::Layer::Ground);
}
