#include "HPRenderSystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/Camera.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>

void HPRenderSystem::render() {
    auto view = scene->r.view<TransformComponent, HealthComponent>();

    Camera2DComponent* cam = nullptr;
    TransformComponent* camTf = nullptr;

    auto camView = scene->r.view<Camera2DComponent, TransformComponent>();
    for (auto camEntity : camView) {
        cam = &camView.get<Camera2DComponent>(camEntity);
        camTf = &camView.get<TransformComponent>(camEntity);
        break;
    }

    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& health = view.get<HealthComponent>(entity);

        // Dimensiones
        float barWidth = 60.0f;
        float barHeight = 8.0f;
        float barOffsetY = -15.0f;

        float worldX = transform.position.x - 10;
        float worldY = transform.position.y + barOffsetY;

        float hpRatio = health.currentHealth / health.maxHealth;
        if (hpRatio < 0) hpRatio = 0;
        if (hpRatio > 1) hpRatio = 1;

        Color barColor = GREEN;
        if (scene->r.all_of<PlayerComponent>(entity)) {
            barColor = LIME;
        }

        // fondo
        DrawRectangle(
                worldX,
                worldY,
                barWidth,
                barHeight,
                DARKGRAY
        );

        // barra de HP
        DrawRectangle(
                worldX,
                worldY,
                barWidth * hpRatio,
                barHeight,
                barColor
        );

        // borde
        DrawRectangleLines(
                worldX,
                worldY,
                barWidth,
                barHeight,
                BLACK
        );
    }
}