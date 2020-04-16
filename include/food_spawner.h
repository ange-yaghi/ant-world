#ifndef ANT_WORLD_FOOD_SPAWNER_H
#define ANT_WORLD_FOOD_SPAWNER_H

#include "game_object.h"

#include <queue>

namespace aw {

    class Probe;

    class FoodSpawner : public GameObject {
    public:
        enum class Type {
            Cookie,
            MM,
            Undefined
        };

    public:
        FoodSpawner();
        ~FoodSpawner();

        virtual void initialize();

        virtual void render();
        virtual void process();

        void setType(Type type) { m_type = type; }
        Type getType() const { return m_type; }

        void setAveragePeriod(float period) { m_averagePeriod = period; }
        float getAveragePeriod() const { return m_averagePeriod; }

        void setLifespan(float lifespan) { m_lifespan = lifespan; }
        float getLifespan() const { return m_lifespan; }

        void setRadius(float radius) { m_radius = radius; }
        float getRadius() const { return m_radius; }

        float getAge() const { return m_age; }

    protected:
        void resetTimer();

        ysVector generateRandomLocation();
        ysQuaternion generateRandomOrientation() const;

        bool attemptSpawn();
        Probe *newProbe(const ysVector &location, float radius);
        GameObject *spawnReal();

        std::queue<Probe *> m_spawnAttempts;

    protected:
        float m_unitRadius;
        float m_radius;
        float m_averagePeriod;
        float m_lifespan;

        float m_currentPeriod;
        float m_timer;

        float m_age;

        Type m_type;
    };

} /* namespace aw */

#endif /* ANT_WORLD_FOOD_SPAWNER_H */
