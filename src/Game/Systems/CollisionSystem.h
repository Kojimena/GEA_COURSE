#pragma once
#include "ECS/System.h"
#include <vector>
#include <cmath>

class CollisionSystem : public System {
public:
    void update() override;

private:
    bool checkTileCollision(float worldX, float worldY,
                            const std::vector<int>& grid,
                            int gridWidth, int gridHeight,
                            float scaledTileSize,
                            float tilemapOffsetX,
                            float tilemapOffsetY);

    // Variables para guardar la última posición válida
    float lastValidX = 0.0f;
    float lastValidY = 0.0f;
};