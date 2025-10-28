#include "TileAnimationSystem.h"
#include "Game/Components/Tilemap.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>
#include <iostream>

void TileAnimationSystem::update() {
    float dt = GetFrameTime();
    animationTime += dt;

    auto view = scene->r.view<TileMapComponent>();

    for (auto entity : view) {
        auto& tilemap = view.get<TileMapComponent>(entity);

        for (auto& tile : tilemap.tiles) {
            if (tile.isAnimated && tile.type == WIN_TILE) {
                int totalFrames = 4;
                float frameDuration = 0.15f;

                int currentFrame = (int)(animationTime / frameDuration) % totalFrames;

                tile.ixa = currentFrame * 16;
                tile.iya = 0;

                std::cout << "Animating WIN_TILE: frame=" << currentFrame
                          << " ixa=" << tile.ixa << " iya=" << tile.iya << "\n";
            }
        }
    }
}
