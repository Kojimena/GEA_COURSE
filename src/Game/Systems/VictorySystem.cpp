#include "VictorySystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/IntGrid.h"
#include "Game/Components/Tilemap.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include "Game/Game.h"
#include <raylib.h>

void VictorySystem::update() {
    if (victoryTriggered) return;

    Vector2 playerPos = {0, 0};
    bool hasPlayer = false;

    auto playerView = scene->r.view<PlayerComponent, TransformComponent>();
    for (auto entity : playerView) {
        const auto& tf = playerView.get<TransformComponent>(entity);
        playerPos = {tf.position.x, tf.position.y};
        hasPlayer = true;
        break;
    }

    if (!hasPlayer) return;

    const TileMapComponent* map = nullptr;
    const IntGridComponent* grid = nullptr;
    Vector2 mapOrigin = {0, 0};

    auto tmView = scene->r.view<TileMapComponent, IntGridComponent, TransformComponent>();
    for (auto ent : tmView) {
        map = &tmView.get<TileMapComponent>(ent);
        grid = &tmView.get<IntGridComponent>(ent);
        const auto& mtf = tmView.get<TransformComponent>(ent);
        mapOrigin = {mtf.position.x, mtf.position.y};
        break;
    }

    if (!map || !grid || grid->width <= 0 || grid->height <= 0) return;

    float tileScalePx = map->tiles.empty()
                        ? (float)map->tileSize
                        : map->tiles[0].scale * (float)map->tileSize;

    if (tileScalePx <= 0.0f) tileScalePx = (float)map->tileSize;

    int tx = (int)floorf((playerPos.x - mapOrigin.x) / tileScalePx);
    int ty = (int)floorf((playerPos.y - mapOrigin.y) / tileScalePx);

    if (tx >= 0 && ty >= 0 && tx < grid->width && ty < grid->height) {
        int tileValue = grid->grid[ty * grid->width + tx];

        if (tileValue == 5) {
            victoryTriggered = true;
            if (scene->game) {
                scene->game->changeScene("Victory");
            }
        }
    }
}