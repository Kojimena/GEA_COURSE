#include "TilemapRender.h"
#include "Scene/Scene.h"
#include "Game/Components/Tilemap.h"
#include <raylib.h>

void TileMapRenderSystem::render() {
    auto view = scene->r.view<TileMapComponent>();
    for (auto entity : view) {
        auto& tilemap = view.get<TileMapComponent>(entity);
        for (auto& tile : tilemap.tiles) {
            if (tile.downTexture.id > 0) {
                DrawTextureEx(tile.downTexture, {(float)tile.x * tilemap.tileSize * tile.scale, (float)tile.y * tilemap.tileSize * tile.scale}, 0, tile.scale, WHITE);
            }
            Rectangle sourceRec = {
                    (float)tile.ix,
                    (float)tile.iy,
                    (float)tilemap.tileSize,
                    (float)tilemap.tileSize
            };
            Rectangle destRec = {
                    (float)tile.x * tilemap.tileSize * tile.scale,
                    (float)tile.y * tilemap.tileSize * tile.scale,
                    (float)tilemap.tileSize * tile.scale,
                    (float)tilemap.tileSize * tile.scale
            };
            DrawTexturePro(tile.upTexture, sourceRec, destRec, {0, 0}, 0, WHITE);
        }
    }
}
