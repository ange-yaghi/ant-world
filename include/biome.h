#ifndef ANT_WORLD_BIOME_H
#define ANT_WORLD_BIOME_H

namespace aw {

    class Biome {
    public:
        enum class Type {
            Desert,
            Savanna,
            ContinentalWarm,
            ContinentalCold,
            Arctic,
            Tropical,
            Unknown
        };

        struct BiomeParameters {
            float Precipitation = 0.0f;
            float AverageTemperature = 0.0f;
            float TemperatureFluctation = 0.0f;
            float Elevation = 0.0f;    

            bool operator>=(const BiomeParameters &a) const {
                return
                    Precipitation >= a.Precipitation &&
                    AverageTemperature >= a.AverageTemperature &&
                    TemperatureFluctation >= a.TemperatureFluctation &&
                    Elevation >= a.Elevation;
            }

            bool operator<=(const BiomeParameters &a) const {
                return
                    Precipitation <= a.Precipitation &&
                    AverageTemperature <= a.AverageTemperature &&
                    TemperatureFluctation <= a.TemperatureFluctation &&
                    Elevation <= a.Elevation;
            }
        };

    public:
        Biome(Type type);
        virtual ~Biome();

        virtual float checkBiome(const BiomeParameters &parameters) const;

        Type getType() const { return m_type; }

    protected:
        BiomeParameters m_minRequirements;
        BiomeParameters m_maxRequirements;

    private:
        Type m_type;
    };

} /* namespace aw */

#endif /* ANT_WORLD_BIOME_H */
