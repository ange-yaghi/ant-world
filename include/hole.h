#ifndef ANT_WORLD_HOLE_H
#define ANT_WORLD_HOLE_H

#include "game_object.h"

namespace aw {

    class Realm;

    class Hole : public GameObject {
    public:
        Hole();
        ~Hole();

        virtual void initialize();
        virtual void process();
        virtual void render();

        Realm *getTargetRealm() { return m_targetRealm; }
        virtual Realm *generateRealm();

    protected:
        Realm *m_targetRealm;
    };

} /* namespace aw */

#endif /* ANT_WORLD_HOLE_H */
