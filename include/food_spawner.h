#ifndef ANT_WORLD_FOOD_SPAWNER_H
#define ANT_WORLD_FOOD_SPAWNER_H

#include "game_object.h"

namespace aw {

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

        ysVector generateRandomLocation() const;
        ysQuaternion generateRandomOrientation() const;

        bool checkLocation(const ysVector &vec) const;

        bool attemptSpawn();
        GameObject *spawnObject();

    protected:
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
