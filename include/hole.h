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

        virtual void onEnter(GameObject *object);
        virtual void onExit(GameObject *object);

        Realm *getTargetRealm() { return m_targetRealm; }
        virtual Realm *generateRealm();

        void setHighlight(bool highlight) { m_highlight = highlight; }

    protected:
        Realm *m_targetRealm;
        
        bool m_highlight;
    };

} /* namespace aw */

#endif /* ANT_WORLD_HOLE_H */
