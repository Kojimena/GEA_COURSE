#pragma once
#include "Scene/Scene.h"
#include "ECS/System.h"

class VictorySystem : public System {
public:
    void update() override;

private:
    bool victoryTriggered = false;
};