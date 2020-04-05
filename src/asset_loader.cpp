#include "../include/asset_loader.h"

#include "../include/cookie.h"
#include "../include/mm.h"
#include "../include/player.h"

aw::AssetLoader::AssetLoader() {
    /* void */
}

aw::AssetLoader::~AssetLoader() {
    /* void */
}

void aw::AssetLoader::createAllMaterials(dbasic::AssetManager *am) {
    dbasic::Material *lightFill = am->NewMaterial();
    lightFill->SetName("LightFill");
    lightFill->SetDiffuseColor(ysVector4(0xEF / 255.0f, 0x38 / 255.0f, 0x37 / 255.0f, 1.0f));

    dbasic::Material *lineColor = am->NewMaterial();
    lineColor->SetName("LineColor");
    lineColor->SetDiffuseColor(ysVector4(0x91 / 255.0f, 0x1A / 255.0f, 0x1D / 255.0f, 1.0f));

    dbasic::Material *darkFill = am->NewMaterial();
    darkFill->SetName("DarkFill");
    darkFill->SetDiffuseColor(ysVector4(0xC4 / 255.0f, 0x21 / 255.0f, 0x26 / 255.0f, 1.0f));

    dbasic::Material *highlight = am->NewMaterial();
    highlight->SetName("Highlight");
    highlight->SetDiffuseColor(ysVector4(1.0f, 1.0f, 1.0f, 1.0f - 0.941667f));
}

void aw::AssetLoader::loadAllTextures(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    am->LoadTexture(getPath("food/Cookie_ChocChip.png", assetPath).c_str(), "Cookie");
    am->LoadTexture(getPath("food/MM_Red.png", assetPath).c_str(), "MM");
}

void aw::AssetLoader::loadAllAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    loadAllTextures(assetPath, am);
    createAllMaterials(am);

    am->CompileInterchangeFile(getPath("characters/ant/ant_rigged", assetPath).c_str(), 1.0f, true);
    am->LoadSceneFile(getPath("characters/ant/ant_rigged", assetPath).c_str());

    am->LoadAnimationFile(getPath("characters/ant/ant_rigged.dimo", assetPath).c_str());

    am->ResolveNodeHierarchy();

    Player::configureAssets(am);
    MM::configureAssets(am);
    Cookie::configureAssets(am);
}

std::string aw::AssetLoader::getPath(const char *path, const dbasic::Path &assetPath) {
    return assetPath.Append(dbasic::Path(path)).ToString();
}
