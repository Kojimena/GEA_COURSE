#pragma once
#include "ECS/System.h"
#include "Game/Components/Collider.h"
#include "ECS/Components.h"
#include "Game/Components/Camera.h"
#include <raylib.h>
#include "Scene/Scene.h"

class ColliderRenderSystem : public System {
public:
    void render() override;
};
