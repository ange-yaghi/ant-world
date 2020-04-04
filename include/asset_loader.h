#ifndef ANT_WORLD_ASSET_LOADER_H
#define ANT_WORLD_ASSET_LOADER_H

#include "delta.h"

namespace aw {

    class AssetLoader {
    public:
        AssetLoader();
        ~AssetLoader();

        static void createAllMaterials(dbasic::AssetManager *am);
        static void loadAllTextures(const dbasic::Path &assetPath, dbasic::AssetManager *am);
        static void loadAllAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am);

    protected:
        static std::string getPath(const char *path, const dbasic::Path &assetPath);
    };

} /* namespace aw */

#endif /* ANT_WORLD_ASSET_LOADER_H */
