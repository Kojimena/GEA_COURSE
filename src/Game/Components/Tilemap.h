#pragma once
#include <vector>
#include <cstdint>
#include <raylib.h>


enum TileType {
    LAND,
    GRASS,
    TREES,
    POISONOUS_PLANT,
    PORTAL,
    WIN_TILE
};


struct TileComponent {
    int x;
    int y;
    TileType type;
    Texture2D upTexture;
    Texture2D downTexture;
    float scale = 5.0;
    bool needsAutoTiling = false;
    int ix = 0;
    int iy = 0;

    int ixa = 0, iya = 0;

    bool isAnimated = false;
};

struct TileMapComponent {
    int width;
    int height;
    int tileSize;
    std::vector<TileComponent> tiles;
};