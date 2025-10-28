#pragma once
#include "ECS/System.h"

class HPRenderSystem : public System {
public:
    void render() override;
};