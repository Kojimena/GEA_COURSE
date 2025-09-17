#pragma once
#include "Scene/Scene.h"
#include <raylib.h>
#include "ECS/System.h"
#include "ECS/Components.h"


struct CameraSetupSystem : public System {
    void setup() override;
};

struct CameraFollowSystem : public System {
    void update() override;
};

struct CameraEffectsSystem : public System {
    void update() override;
};

struct CameraZoomInputSystem : public System {
    void update() override;
};


struct CameraBeginRenderSystem : public System {
    void render() override;
};

struct CameraEndRenderSystem : public System {
    void render() override;
};