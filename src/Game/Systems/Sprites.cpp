#include "Sprites.h"
#include "../Graphics/TextureManager.h"
#include "../../Scene/Scene.h"
#include "../Graphics/TextureManager.h"
#include "../Components/Sprites.h"
#include "Game/Components/Player.h"
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


void SpriteSystem::update() {
    auto view = scene->r.view<SpriteLayerComponent>();
    long nowMs = (long)(GetTime() * 1000.0); // ms

    for (auto e : view) {
        auto &sp = view.get<SpriteLayerComponent>(e);

        if (sp.texture.id == 0) {
            sp.texture = TextureManager::loadTexture(sp.path);
            if (sp.texture.id == 0) {
                std::cerr << "[Sprite][ERROR] No se cargó la textura (lazy): " << sp.path << "\n";
                // si no cargó, saltamos animación para evitar usar datos inválidos
                continue;
            }
        }

        if (sp.animationFrame <= 0) continue;

        int cycleMs = sp.animationDuration;
        if (cycleMs <= 0) cycleMs = 100;
        int msPerFrame = cycleMs / sp.animationFrame;
        if (msPerFrame <= 0) msPerFrame = 16;

        if (nowMs - sp.lastUpdate >= msPerFrame) {
            int steps = (nowMs - sp.lastUpdate) / msPerFrame;
            sp.ix = (sp.ix + steps) % sp.animationFrame;
            sp.lastUpdate = nowMs;
        }
    }
}

void SpriteSystem::render() {
    auto view = scene->r.view<TransformComponent, SpriteLayerComponent>();

    for (auto entity : view) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& sprite    = view.get<SpriteLayerComponent>(entity);

        if (sprite.texture.id == 0) {
            const_cast<SpriteLayerComponent&>(sprite).texture = TextureManager::loadTexture(sprite.path);
            if (sprite.texture.id == 0) {
                std::cerr << "[Sprite][ERROR] No se cargó la textura (render): " << sprite.path << "\n";
                continue;
            }
        }

        Rectangle src = {
                (float)sprite.ix * (float)sprite.frameW,
                (float)sprite.iy * (float)sprite.frameH,
                (float)sprite.frameW,
                (float)sprite.frameH
        };

        Rectangle dst = {
                transform.position.x,
                transform.position.y,
                (float)sprite.frameW * (float)sprite.scale,
                (float)sprite.frameH * (float)sprite.scale
        };

        DrawTexturePro(sprite.texture, src, dst, {0,0}, 0.0f, WHITE);

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


// CONTROLES Y ANIMACIÓN DEL PERSONAJE

void SpriteMovementSystem::update() {
    auto view = scene->r.view<TransformComponent, SpriteLayerComponent, PlayerComponent>();

    const float moveSpeed = 120.0f * GetFrameTime();
    const bool attackPressed = IsKeyDown(KEY_SPACE);

    for (auto e : view) {
        auto &t  = view.get<TransformComponent>(e);
        auto &sp = view.get<SpriteLayerComponent>(e);

        float dx = 0.0f, dy = 0.0f;
        if (!attackPressed) {
            if (IsKeyDown(KEY_RIGHT) ) dx += moveSpeed;
            if (IsKeyDown(KEY_LEFT)  )dx -= moveSpeed;
            if (IsKeyDown(KEY_DOWN) )dy += moveSpeed;
            if (IsKeyDown(KEY_UP) ) dy -= moveSpeed;
        }

        if (attackPressed) {
            if (sp.iy != 1) { sp.iy = 1; sp.ix = 0; sp.lastUpdate = (long)(GetTime()*1000.0); }
            sp.animationFrame    = 8;
            sp.animationDuration = 500;

        } else {
            const bool moving = (std::fabs(dx) > 0.0f || std::fabs(dy) > 0.0f);
            if (moving) {
                // caminar a la derecha
                if (dx > 0.0f) sp.iy = 3;
                // caminar a la izquierda
                else if (dx < 0.0f) sp.iy = 1;
                // caminar abajo
                else if (dy > 0.0f) sp.iy = 0;
                // caminar arriba
                else if (dy < 0.0f) sp.iy = 2;
                sp.animationFrame    = 8;
                sp.animationDuration = 600;
            } else {
                sp.iy = 0;
                sp.ix = 0;
                sp.animationFrame    = 1;
                sp.animationDuration = 1000;
            }
        }

        t.position.x += dx;
        t.position.y += dy;
    }
}
