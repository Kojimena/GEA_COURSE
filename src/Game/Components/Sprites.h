#pragma once
#include <raylib.h>
#include <string>

struct SpriteLayerComponent{
    std::string path;
    int size = 32; // Default size for the sprite
    int scale = 4; // Scale factor for the sprite
    Texture2D texture; // Texture for the sprite
    int ox = 32;
    int oy = 32;
};