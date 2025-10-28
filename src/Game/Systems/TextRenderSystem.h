#pragma once
#include "ECS/System.h"

class TextRenderSystem : public System {
public:
    void render() override;
};