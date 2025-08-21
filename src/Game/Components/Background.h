#pragma once
#include <raylib.h>
#include "ECS/System.h"
#include "ECS/Components.h"
#include "Game/Graphics/TextureManager.h"

struct BackgroundComponent {
    std::string path;
    Texture2D texture{};
    float parallax = 0.0f;
    bool stretchToScreen = true;
};