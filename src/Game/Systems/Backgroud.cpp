#include "Background.h"
#include "Scene/Scene.h"

void BackgroundSystem::setup() {
    auto view = scene->r.view<BackgroundComponent>();
    for (auto e : view) {
        auto &bg = view.get<BackgroundComponent>(e);
        bg.texture = TextureManager::loadTexture(bg.path);
    }
}

void BackgroundSystem::render() {
    Vector2 cam{0,0};

    auto view = scene->r.view<BackgroundComponent, TransformComponent>();
    for (auto e : view) {
        const auto &t  = view.get<TransformComponent>(e);
        const auto &bg = view.get<BackgroundComponent>(e);

        float dx = -cam.x * bg.parallax;
        float dy = -cam.y * bg.parallax;

        Rectangle src{0, 0, (float)bg.texture.width, (float)bg.texture.height};

        Rectangle dst;
        if (bg.stretchToScreen) {
            dst = { t.position.x + dx, t.position.y + dy,
                    (float)GetScreenWidth(), (float)GetScreenHeight() };
        } else {
            dst = { t.position.x + dx, t.position.y + dy,
                    (float)bg.texture.width, (float)bg.texture.height };
        }

        DrawTexturePro(bg.texture, src, dst, {0,0}, 0.0f, WHITE);
    }
}
