#ifndef ANT_WORLD_HOLE_H
#define ANT_WORLD_HOLE_H

#include "game_object.h"

#include "interior_generation.h"

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

        virtual Realm *generateRealm();
        Realm *getTargetRealm() { return m_targetRealm; }

        void setHighlight(bool highlight) { m_highlight = highlight; }

    protected:
        static InteriorGenerator *m_generator;
        Realm *m_targetRealm;
        
        bool m_highlight;
    };

} /* namespace aw */

#endif /* ANT_WORLD_HOLE_H */
