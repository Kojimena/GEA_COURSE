#include "TilemapSetupSystem.h"
#include "Game/Components/TilemapData.h"
#include "ECS/Entity.h"
#include "FastNoiseLite.h"

void TilemapSetupSystem::setup() {
    Entity tilemapEntity = scene->createEntity(entityName.c_str(), originX, originY);
    auto& tilemap = tilemapEntity.addComponent<TileMapComponent>();

    const int H = static_cast<int>(TILEMAP_MAINMAP.size());
    const int W = static_cast<int>(TILEMAP_MAINMAP.empty() ? 0 : TILEMAP_MAINMAP[0].size());

    tilemap.width = W;
    tilemap.height = H;
    tilemap.tileSize = tileSize;

    Texture2D grassTexture = TextureManager::loadTexture((assetsDir + "/Nightgrass.png").c_str());
    Texture2D landTexture  = TextureManager::loadTexture((assetsDir + "/Land.png").c_str());
    Texture2D treesTexture = TextureManager::loadTexture((assetsDir + "/trees.png").c_str());

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetSeed(static_cast<int>(time(nullptr)));
    noise.SetFrequency(0.05f);

    FastNoiseLite treeNoise;
    treeNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    treeNoise.SetSeed(static_cast<int>(time(nullptr)) + 999);
    treeNoise.SetFrequency(0.1f);

    tilemap.tiles.reserve(W * H);

    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            TileComponent tile{};
            tile.x = x;
            tile.y = y;

            float num = noise.GetNoise((float)x, (float)y);

            if (num > 0.1f) {
                tile.type = GRASS;
                tile.upTexture = grassTexture;
                tile.downTexture = landTexture;
                tile.needsAutoTiling = true;
            }
            else {
                float treeVal = treeNoise.GetNoise((float)x, (float)y);
                if (treeVal > 0.65f) {
                    tile.type = TREES;
                    tile.upTexture = treesTexture;
                    tile.downTexture = landTexture;
                    tile.needsAutoTiling = true;
                } else {
                    tile.type = LAND;
                    tile.upTexture = landTexture;
                    tile.downTexture = {0};
                    tile.needsAutoTiling = false;
                }
            }
            tilemap.tiles.push_back(tile);
        }
    }
}
