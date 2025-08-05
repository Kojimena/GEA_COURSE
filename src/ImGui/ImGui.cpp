#include "ImGui.h"
#include "Scene/Scene.h"
#include "rlImGui.h"
#include "imgui.h"
#include "ECS/Components.h"
#include "Breakout/Components.h"

void ShowComponentProperties(entt::registry& registry, entt::entity entity);
entt::entity ImGuiSystem::selectedEntity = entt::null;
bool ImGuiSystem::isPaused = false;

ImGuiSystem::ImGuiSystem() {
    rlImGuiSetup(true);
}

ImGuiSystem::~ImGuiSystem() {
    rlImGuiShutdown();
}

void ImGuiSystem::setup() {
}

void ImGuiSystem::render() {
    rlImGuiBegin();

    // —— Panel de ENTIDADES ——
    ImGui::Begin("Entities");
    for (auto e : scene->r.view<NameComponent>()) {
        const char* tag = scene->r.get<NameComponent>(e).tag.c_str();
        ImGui::PushID((int)(uint64_t)e);
        if (ImGui::Selectable(tag, selectedEntity == e, 0)) {
            selectedEntity = e;
        }
        ImGui::PopID();
    }
    ImGui::End();


    // —— Panel de COMPONENTES ——
    ImGui::Begin("Components");
    if (selectedEntity != entt::null) {
        ShowComponentProperties(scene->r, selectedEntity);
    } else {
        ImGui::TextDisabled("Select an entity to edit its components");
    }
    ImGui::End();

    // —— Panel de CONTROLES / Pausa ——
    ImGui::Begin("Controls");
    if (ImGui::Button(isPaused ? "Resume" : "Pause")) {
        isPaused = !isPaused;
    }
    ImGui::Text("Game is %s", isPaused ? "paused" : "running");
    ImGui::End();

    rlImGuiEnd();
}



void ShowComponentProperties(entt::registry& registry, entt::entity entity) {

    //  TransformComponent
    if (registry.all_of<TransformComponent>(entity)) {
        auto& transform = registry.get<TransformComponent>(entity);

        ImGui::SliderFloat("pX", &transform.position.x, 0.0f, 1200.0f);
        ImGui::SliderFloat("pY", &transform.position.y, 0.0f, 600.0f);}

    //  VelocityComponent
    if (registry.all_of<VelocityComponent>(entity)) {
        auto& velocity = registry.get<VelocityComponent>(entity);
        ImGui::SliderFloat("vX", &velocity.velocity.x, -100.0f, 100.0f);
        ImGui::SliderFloat("vY", &velocity.velocity.y, -100.0f, 100.0f);
    }

    //  SpriteComponent
//    if (registry.all_of<SpriteComponent>(entity)) {
//        auto& sprite = registry.get<SpriteComponent>(entity);
//        ImGui::ColorEdit3("Color", (float*)&sprite.color);
//    }
}