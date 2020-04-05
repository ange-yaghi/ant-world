#ifndef ANT_WORLD_MM_H
#define ANT_WORLD_MM_H

#include "food.h"

namespace aw {

    class MM : public Food {
    public:
        MM();
        ~MM();

        virtual void initialize();

        virtual void render();
        virtual void process();

        virtual float getPickupRadius() const { return 0.5f; }

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

        static dbasic::TextureAsset *MMTexture;
    };

} /* namespace aw */

#endif /* ANT_WORLD_MM_H */
