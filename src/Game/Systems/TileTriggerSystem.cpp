#include "TileTriggerSystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/Collider.h"
#include "Game/Components/TilemapData.h"
#include "Game/Components/IntGrid.h"
#include "Game/Components/Tilemap.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>
#include <cmath>
#include <vector>

static float s_playerX = 0, s_playerY = 0;
static int s_tileX = 0, s_tileY = 0;
static int s_tileValue = -1;
static bool s_outOfBounds = true;

static float s_portalCooldown = 0.0f;
static std::vector<std::pair<int, int>> s_portals;

void TileTriggerSystem::update() {
    float dt = GetFrameTime();
    if (s_portalCooldown > 0) s_portalCooldown -= dt;

    auto playerView = scene->r.view<PlayerComponent, TransformComponent, ColliderComponent>();
    auto tilemapView = scene->r.view<TileMapComponent, IntGridComponent, TransformComponent>();

    for (auto playerEntity : playerView) {
        auto& playerTf = playerView.get<TransformComponent>(playerEntity);
        auto& playerCol = playerView.get<ColliderComponent>(playerEntity);

        for (auto tilemapEntity : tilemapView) {
            const auto& tilemap = tilemapView.get<TileMapComponent>(tilemapEntity);
            const auto& intGrid = tilemapView.get<IntGridComponent>(tilemapEntity);
            const auto& tmTf = tilemapView.get<TransformComponent>(tilemapEntity);

            if (s_portals.empty()) {
                for (int y = 0; y < intGrid.height; ++y) {
                    for (int x = 0; x < intGrid.width; ++x) {
                        if (intGrid.grid[y * intGrid.width + x] == 3) {
                            s_portals.push_back({x, y});
                        }
                    }
                }
            }

            float tileScalePx = tilemap.tiles.empty()
                                ? (float)tilemap.tileSize
                                : tilemap.tiles[0].scale * (float)tilemap.tileSize;

            float centerX = playerTf.position.x + playerCol.ox + (playerCol.w / 2.0f);
            float centerY = playerTf.position.y + playerCol.oy + (playerCol.h / 2.0f);

            float relativeX = centerX - tmTf.position.x;
            float relativeY = centerY - tmTf.position.y;

            s_tileX = static_cast<int>(std::floor(relativeX / tileScalePx));
            s_tileY = static_cast<int>(std::floor(relativeY / tileScalePx));

            s_playerX = playerTf.position.x;
            s_playerY = playerTf.position.y;

            if (s_tileX >= 0 && s_tileX < intGrid.width &&
                s_tileY >= 0 && s_tileY < intGrid.height) {

                s_tileValue = intGrid.grid[s_tileY * intGrid.width + s_tileX];
                s_outOfBounds = false;

                // Teletransporte
                if (s_tileValue == 3 && s_portalCooldown <= 0 && s_portals.size() == 2) {
                    int otherPortal = (s_tileX == s_portals[0].first && s_tileY == s_portals[0].second) ? 1 : 0;

                    playerTf.position.x = tmTf.position.x + s_portals[otherPortal].first * tileScalePx;
                    playerTf.position.y = tmTf.position.y + s_portals[otherPortal].second * tileScalePx;

                    s_portalCooldown = 2.0f;
                }
            } else {
                s_outOfBounds = true;
                s_tileValue = -1;
            }
        }
    }
}