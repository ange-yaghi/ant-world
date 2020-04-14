#ifndef ANT_WORLD_BEETLE_H
#define ANT_WORLD_BEETLE_H

#include "game_object.h"

namespace aw {

    class Realm;

    class Beetle : public GameObject {
    public:
        Beetle();
        ~Beetle();

        virtual void initialize();
        virtual void process();
        virtual void render();

        virtual void onEnter(GameObject *object);
        virtual void onExit(GameObject *object);

        Realm *getTargetRealm() { return m_targetRealm; }
        virtual Realm *generateRealm();

    protected:
        Realm *m_targetRealm;
    };

} /* namespace aw */

#endif /* ANT_WORLD_BEETLE_H */

