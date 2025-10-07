#include "TilemapSetupSystem.h"
#include "Game/Components/TilemapData.h"
#include "ECS/Entity.h"
#include "FastNoiseLite.h"
#include "Game/Components/IntGrid.h"
#include <vector>
#include <cstdlib>
#include <ctime>

void TilemapSetupSystem::setup() {
    Entity tilemapEntity = scene->createEntity(entityName.c_str(), originX, originY);
    auto& tilemap = tilemapEntity.addComponent<TileMapComponent>();

    const int H = static_cast<int>(TILEMAP_MAINMAP.size());
    const int W = static_cast<int>(TILEMAP_MAINMAP.empty() ? 0 : TILEMAP_MAINMAP[0].size());

    tilemap.width = W;
    tilemap.height = H;
    tilemap.tileSize = tileSize;

    Texture2D grassTexture = TextureManager::loadTexture((assetsDir + "/Nightgrass.png").c_str());
    Texture2D landTexture = TextureManager::loadTexture((assetsDir + "/Land.png").c_str());
    Texture2D treesTexture = TextureManager::loadTexture((assetsDir + "/trees.png").c_str());
    Texture2D poisonousTexture = TextureManager::loadTexture((assetsDir + "/venenous.png").c_str());
    Texture2D portalTexture = TextureManager::loadTexture((assetsDir + "/portal.png").c_str());

    auto& intGrid = tilemapEntity.addComponent<IntGridComponent>();
    intGrid.width = W;
    intGrid.height = H;
    intGrid.grid.assign(W * H, 1);

    srand(static_cast<unsigned int>(time(nullptr)));

    // portales
    int margin = 3;
    std::vector<std::pair<int, int>> portals = {
            {margin, margin},
            {W - margin - 1, margin},
            {margin, H - margin - 1},
            {W - margin - 1, H - margin - 1}
    };


    std::vector<std::pair<int, int>> portalPositions;
    std::vector<int> usedIndices;

    for (int i = 0; i < 2; ++i) {
        int randomIndex;
        do {
            randomIndex = rand() % 4;
        } while (std::find(usedIndices.begin(), usedIndices.end(), randomIndex) != usedIndices.end());

        usedIndices.push_back(randomIndex);
        portalPositions.push_back(portals[randomIndex]);
    }

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetSeed(static_cast<int>(time(nullptr)));
    noise.SetFrequency(0.08f);

    FastNoiseLite treeNoise;
    treeNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    treeNoise.SetSeed(static_cast<int>(time(nullptr)) + 999);
    treeNoise.SetFrequency(0.2f);

    FastNoiseLite poisonNoise;
    poisonNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    poisonNoise.SetSeed(static_cast<int>(time(nullptr)) + 1234);
    poisonNoise.SetFrequency(0.16f);

    tilemap.tiles.reserve(W * H);

    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            TileComponent tile{};
            tile.x = x; tile.y = y;

            if (x <= 2 && y <= 2) {
                tile.type = GRASS;
                tile.upTexture = grassTexture;
                tile.downTexture = landTexture;
                tile.needsAutoTiling = true;
                intGrid.grid[y * W + x] = 1;
            }
            else if (std::find(portalPositions.begin(), portalPositions.end(),
                               std::make_pair(x, y)) != portalPositions.end()) {
                tile.type = PORTAL;
                tile.upTexture = portalTexture;
                tile.downTexture = landTexture;
                tile.needsAutoTiling = false;
                intGrid.grid[y * W + x] = 3;
            }
            else {
                float num = noise.GetNoise((float)x, (float)y);
                float treeVal = treeNoise.GetNoise((float)x, (float)y);
                float poisonVal = poisonNoise.GetNoise((float)x, (float)y);

                if (num <= 0.1f && treeVal > 0.75f) {
                    tile.type = TREES;
                    tile.upTexture = treesTexture;
                    tile.downTexture = landTexture;
                    tile.needsAutoTiling = true;
                    intGrid.grid[y * W + x] = 0;
                }
                else if (num > 0.1f && poisonVal > 0.7f) {
                    tile.type = POISONOUS_PLANT;
                    tile.upTexture = poisonousTexture;
                    tile.downTexture = landTexture;
                    tile.needsAutoTiling = false;
                    intGrid.grid[y * W + x] = 2;
                }
                else if (num > 0.1f) {
                    tile.type = GRASS;
                    tile.upTexture = grassTexture;
                    tile.downTexture = landTexture;
                    tile.needsAutoTiling = true;
                    intGrid.grid[y * W + x] = 1;
                }
                else {
                    tile.type = LAND;
                    tile.upTexture = landTexture;
                    tile.downTexture = {0};
                    tile.needsAutoTiling = false;
                    intGrid.grid[y * W + x] = 1;
                }
            }

            tilemap.tiles.push_back(tile);
        }
    }
}