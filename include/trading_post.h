#ifndef ANT_WORLD_TRADING_POST_H
#define ANT_WORLD_TRADING_POST_H

#include "hole.h"

namespace aw {

    class TradingPost : public Hole {
    public:
        TradingPost();
        ~TradingPost();

        virtual void initialize();
        virtual void process();
        virtual void render();

        virtual void onEnter(GameObject *object);
        virtual void onExit(GameObject *object);

        virtual Realm *generateRealm();
    };

} /* namespace aw */

#endif /* ANT_WORLD_TRADING_POST_H */
