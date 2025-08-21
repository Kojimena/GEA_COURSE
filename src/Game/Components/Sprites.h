#pragma once
#include <raylib.h>
#include <string>

struct SpriteLayerComponent{
    std::string path;
    int size = 16; // Default size for the sprite
    int scale = 4; // Scale factor for the sprite

    int animationFrame = 8;
    int animationDuration = 600; // Speed of the animation

    int ox = 32;
    int oy = 32;

    int ix = 0; // Index for sprite sheet
    int iy = 0; // Index for sprite sheet

    Texture2D texture; // Texture for the sprite

    long lastUpdate = 0; // Last update time for animation
};
