#include "TilemapRender.h"
#include "Scene/Scene.h"
#include "Game/Components/Tilemap.h"
#include <raylib.h>

void TileMapRenderSystem::render() {
    auto view = scene->r.view<TileMapComponent>();
    for (auto entity : view) {
        const auto& tm = view.get<TileMapComponent>(entity);
        const float tileW = (float)tm.tileSize;
        const float tileH = (float)tm.tileSize;

        for (const auto& t : tm.tiles) {
            const float sx = t.x * tileW * t.scale;
            const float sy = t.y * tileH * t.scale;

            const Rectangle dst = { sx, sy, tileW * t.scale, tileH * t.scale };
            const Rectangle srcFull = { 0, 0, tileW, tileH };

            // Base
            if (t.downTexture.id > 0) {
                DrawTexturePro(t.downTexture, srcFull, dst, {0,0}, 0.0f, WHITE);
            }

            // Capa superior
            if (t.upTexture.id > 0) {
                if (t.needsAutoTiling) {
                    const Rectangle srcMask = { (float)t.ix, (float)t.iy, tileW, tileH };
                    DrawTexturePro(t.upTexture, srcMask, dst, {0,0}, 0.0f, WHITE);
                } else {
                    DrawTexturePro(t.upTexture, srcFull, dst, {0,0}, 0.0f, WHITE);
                }
            }
        }
    }
}

