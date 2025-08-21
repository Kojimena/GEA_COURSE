#pragma once
#include <raylib.h>
#include "ECS/System.h"
#include "ECS/Components.h"
#include "Game/Graphics/TextureManager.h"

class BackgroundSystem : public System {
public:
    void setup() override;
    void render() override;
};