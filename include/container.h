#ifndef ANT_WORLD_CONTAINER_H
#define ANT_WORLD_CONTAINER_H

#include "game_object.h"

namespace aw {

    class Container : public GameObject {
    public:
        Container();
        ~Container();

        virtual void initialize();

        virtual void render();
        virtual void process();

        int getCountInside() const { return (int)m_contents.size(); }

        virtual float getPickupRadius() const { return 2.0f; }

        void load(GameObject *object);
        GameObject *unload(int objectIndex);

    protected:
        std::vector<GameObject *> m_contents;
    };

} /* namespace aw */

#endif /* ANT_WORLD_CONTAINER_H */