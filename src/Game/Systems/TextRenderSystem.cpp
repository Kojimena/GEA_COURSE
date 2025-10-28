#include "TextRenderSystem.h"
#include "Game/Components/TextComponent.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>

void TextRenderSystem::render() {
    auto view = scene->r.view<TransformComponent, TextComponent>();

    for (auto entity : view) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& textComp = view.get<TextComponent>(entity);

        float x = transform.position.x;
        float y = transform.position.y;

        int textWidth = MeasureText(textComp.text.c_str(), textComp.fontSize);

        float centeredX = x - textWidth / 2.0f;

        DrawText(
                textComp.text.c_str(),
                (int)centeredX,
                (int)y,
                textComp.fontSize,
                textComp.color
        );
    }
}