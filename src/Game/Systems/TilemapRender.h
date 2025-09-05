#pragma once
#include "ECS/System.h"

class TileMapRenderSystem : public System {
public:
    void render() override;
};
