#pragma once
#include "ECS/System.h"
#include "Game/Components/Tilemap.h"
#include "Game/Components/IntGrid.h"
#include "Game/Components/Camera.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>

class IntgridRenderSystem : public System {
public:
    void render() override;
};