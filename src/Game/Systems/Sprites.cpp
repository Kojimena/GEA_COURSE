#include "Sprites.h"
#include "../Graphics/TextureManager.h"
#include "../../Scene/Scene.h"
#include "../Graphics/TextureManager.h"
#include "../Components/Sprites.h"
#include <iostream>
#include <filesystem>


void SpriteSystem::setup() {
    auto view = scene->r.view<SpriteLayerComponent>();
    for (auto entity : view) {
        auto& sprite = view.get<SpriteLayerComponent>(entity);

        // Log ruta y CWD
        std::cout << "[Sprite] cwd=" << std::filesystem::current_path() << "\n";
        std::cout << "[Sprite] intento cargar: " << sprite.path << "\n";

        sprite.texture = TextureManager::loadTexture(sprite.path);

        if (sprite.texture.id == 0) {
            std::cerr << "[Sprite][ERROR] No se cargó la textura: " << sprite.path << "\n";
        } else {
            std::cout << "[Sprite] OK " << sprite.texture.width << "x" << sprite.texture.height
                      << " id=" << sprite.texture.id << "\n";
        }
    }
}

void SpriteSystem::render() {
    auto view = scene->r.view<TransformComponent, SpriteLayerComponent>();

    for (auto entity : view) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& sprite    = view.get<SpriteLayerComponent>(entity);

        Rectangle src = {
                (float)0.0 + (float)sprite.ox,
                (float)0.0 + (float)sprite.oy,
                (float)sprite.size,
                (float)sprite.size,
        };

        Rectangle dst = {
                transform.position.x,
                transform.position.y,
                (float)sprite.size * (float)sprite.scale,
                (float)sprite.size * (float)sprite.scale,
        };
        const float alpha = 0.4f;
        DrawTexturePro(
            sprite.texture,
            src,
            dst,
            {0, 0},
            0.0f,
            WHITE
        );
    }
}
