#pragma once
#include "Scene/Scene.h"
#include "Game/Components/Tilemap.h"   // TileMapComponent, TileComponent, enums (GRASS/LAND/TREES)
#include "Game/Graphics/TextureManager.h"
#include <raylib.h>
#include "ECS/System.h"


class TilemapSetupSystem : public System {
public:
    std::string entityName = "tilemap";
    int originX = 0;
    int originY = 0;
    int tileSize = 16;
    std::string assetsDir = "../src/assets";

    void setup() override;
};
