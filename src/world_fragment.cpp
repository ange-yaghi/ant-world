#include "../include/world_fragment.h"

#include "../include/world.h"
#include "../include/game_object.h"

dbasic::ModelAsset *aw::WorldFragment::m_terrainBlock = nullptr;
dbasic::TextureAsset *aw::WorldFragment::m_terrainTexture = nullptr;

aw::WorldFragment::WorldFragment() {
    m_x = 0;
    m_y = 0;
    m_width = 0.0f;
    m_height = 0.0f;
    m_biome = Biome::Type::Unknown;
    m_world = nullptr;
    m_blockAngle = 0.0f;
    m_blockScale = 1.0f;
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

    m_blockAngle = ysMath::UniformRandom() * ysMath::Constants::TWO_PI;
    m_blockScale = ysMath::UniformRandom() + 1;
}

void aw::WorldFragment::debugRender() {
    const int arcticColor[] = { 128,194,178 };
    const int tropicalColor[] = { 57, 255, 20 };
    const int savannaColor[] = { 166,145,80 };
    const int continentalColdColor[] = { 194,178,128 };
    const int continentalWarmColor[] = { 211,199,162 };
    const int desertColor[] = { 219,209,180 };
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
    const int populationColor[] = { m_parameters.PopulationDensity * 255, 0, 0 };
    //color = populationColor;

    ysMatrix transform = ysMath::TranslationTransform(
        ysMath::LoadVector(m_x * m_width, m_y * m_height, 0.0f, 1.0f));
    ysMatrix rotation = ysMath::RotationTransform(ysMath::Constants::ZAxis, m_blockAngle);

    m_world->getEngine().SetMultiplyColor(ysVector4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f));
    m_world->getEngine().SetObjectTransform(ysMath::MatMult(transform, rotation));
    m_world->getEngine().DrawModel(m_terrainBlock, ysMath::MatMult(transform, rotation), 1.0f, nullptr, (int)GameObject::Layer::Ground);
    //m_world->getEngine().DrawImage(m_terrainTexture->GetTexture(), (int)GameObject::Layer::Ground, m_width * m_blockScale * 2, m_height * m_blockScale * 2);
    //m_world->getEngine().DrawBox(color, m_width, m_height, (int)GameObject::Layer::Ground);
}

void aw::WorldFragment::configureAssets(dbasic::AssetManager *am) {
    m_terrainBlock = am->GetModelAsset("TerrainBlock");
    m_terrainTexture = am->GetTexture("Terrain::TransparencyMap");
}
