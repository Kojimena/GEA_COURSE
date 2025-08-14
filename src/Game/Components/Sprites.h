#pragma once
#include <raylib.h>
#include <string>

struct SpriteComponent{
    std::string path;
    int size = 48; // Default size for the sprite
    Texture2D texture; // Texture for the sprite
};