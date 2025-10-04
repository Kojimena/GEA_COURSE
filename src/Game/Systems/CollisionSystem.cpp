#include "CollisionSystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/Collider.h"
#include "Game/Components/TilemapData.h"
#include "Game/Components/IntGrid.h"
#include "Game/Components/Tilemap.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>

void CollisionSystem::update() {
    auto playerView = scene->r.view<PlayerComponent, TransformComponent, ColliderComponent>();
    auto tilemapView = scene->r.view<TileMapComponent, IntGridComponent, TransformComponent>();

    for (auto playerEntity : playerView) {
        auto& playerTf = playerView.get<TransformComponent>(playerEntity);
        auto& playerCol = playerView.get<ColliderComponent>(playerEntity);

        static bool firstFrame = true;
        if (firstFrame) {
            lastValidX = playerTf.position.x;
            lastValidY = playerTf.position.y;
            firstFrame = false;
        }

        // Posición previa
        float previousX = lastValidX;
        float previousY = lastValidY;

        for (auto tilemapEntity : tilemapView) {
            const auto& tilemap = tilemapView.get<TileMapComponent>(tilemapEntity);
            const auto& intGrid = tilemapView.get<IntGridComponent>(tilemapEntity);
            const auto& tmTf = tilemapView.get<TransformComponent>(tilemapEntity);

            float tileScalePx = tilemap.tiles.empty()
                                ? (float)tilemap.tileSize
                                : tilemap.tiles[0].scale * (float)tilemap.tileSize;

            // Posición del collider del jugador
            float colliderX = playerTf.position.x + playerCol.ox;
            float colliderY = playerTf.position.y + playerCol.oy;

            // Esquinas del collider
            float left = colliderX;
            float right = colliderX + playerCol.w;
            float top = colliderY;
            float bottom = colliderY + playerCol.h;

            bool collision = false;

            if (checkTileCollision(left, top, intGrid.grid,
                                   intGrid.width, intGrid.height, tileScalePx,
                                   tmTf.position.x, tmTf.position.y) ||
                checkTileCollision(right, top, intGrid.grid,
                                   intGrid.width, intGrid.height, tileScalePx,
                                   tmTf.position.x, tmTf.position.y) ||
                checkTileCollision(left, bottom, intGrid.grid,
                                   intGrid.width, intGrid.height, tileScalePx,
                                   tmTf.position.x, tmTf.position.y) ||
                checkTileCollision(right, bottom, intGrid.grid,
                                   intGrid.width, intGrid.height, tileScalePx,
                                   tmTf.position.x, tmTf.position.y)) {
                collision = true;
            }

            // Si hubo colisión, revertir a la posición previa
            if (collision) {
                playerTf.position.x = previousX;
                playerTf.position.y = previousY;
                playerCol.triggered = true;
            } else {
                lastValidX = playerTf.position.x;
                lastValidY = playerTf.position.y;
                playerCol.triggered = false;
            }
        }
    }
}

bool CollisionSystem::checkTileCollision(float worldX, float worldY,
                                         const std::vector<int>& grid,
                                         int gridWidth, int gridHeight,
                                         float scaledTileSize,
                                         float tilemapOffsetX,
                                         float tilemapOffsetY) {
    float relativeX = worldX - tilemapOffsetX;
    float relativeY = worldY - tilemapOffsetY;

    int tileX = static_cast<int>(std::floor(relativeX / scaledTileSize));
    int tileY = static_cast<int>(std::floor(relativeY / scaledTileSize));

    // Limites del grid
    if (tileX < 0 || tileX >= gridWidth || tileY < 0 || tileY >= gridHeight) {
        return true;
    }

    return grid[tileY * gridWidth + tileX] == 0;
}