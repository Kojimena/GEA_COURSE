#pragma once
#include "ECS/System.h"
#include "ECS/Components.h"
#include "Breakout/Components.h"
#include "Scene/Scene.h"
#include <imgui.h>
#include <entt/entt.hpp>

class ImGuiSystem : public System {
public:
    ImGuiSystem();
    ~ImGuiSystem();
    void setup() override;
    void render() override;
    static entt::entity selectedEntity;
    static bool         isPaused;
};