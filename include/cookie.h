#ifndef ANT_WORLD_COOKIE_H
#define ANT_WORLD_COOKIE_H

#include "food.h"

namespace aw {

    class Cookie : public Food {
    public:
        Cookie();
        ~Cookie();

        virtual void initialize();

        virtual void render();
        virtual void process();

        virtual float getPickupRadius() const { return 1.0f; }

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

        static dbasic::TextureAsset *CookieTexture;
    };

} /* namespace aw */

#endif /* ANT_WORLD_COOKIE_H */
