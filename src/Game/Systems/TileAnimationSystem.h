#pragma once
#include "Scene/Scene.h"
#include "ECS/System.h"

class TileAnimationSystem : public System {
public:
    void update() override;

private:
    float animationTime = 10.0f;
};