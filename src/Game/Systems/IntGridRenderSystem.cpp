#include "IntgridRenderSystem.h"

void IntgridRenderSystem::render() {
    auto view = scene->r.view<TileMapComponent, IntGridComponent, TransformComponent>();

    for (auto e : view) {
        const auto& tilemap = view.get<TileMapComponent>(e);
        const auto& intgrid = view.get<IntGridComponent>(e);
        const auto& tmTf    = view.get<TransformComponent>(e);

        const float tileScalePx = tilemap.tiles.empty()
                                  ? (float)tilemap.tileSize
                                  : tilemap.tiles[0].scale * (float)tilemap.tileSize;

        for (int y = 0; y < intgrid.height; ++y) {
            for (int x = 0; x < intgrid.width; ++x) {
                if (intgrid.grid[y * intgrid.width + x] == 0) {
                    const float px = tmTf.position.x + x * tileScalePx;
                    const float py = tmTf.position.y + y * tileScalePx;
                    DrawRectangle(px, py, tileScalePx, tileScalePx, Fade(RED, 0.30f));
                }
            }
        }
    }
}
