#include "Game/Systems/Camera.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Sprites.h"
#include "Game/Components/Tilemap.h"
#include "Game/Components/Player.h"
#include "Scene/Scene.h"
#include <cmath>
#include <algorithm>

static inline float lerp(float a, float b, float t) { return a + (b - a) * t; }

void CameraSetupSystem::setup() {
    auto view = scene->r.view<Camera2DComponent>();
    for (auto e : view) {
        auto &c = view.get<Camera2DComponent>(e);
        c.cam.zoom = 1.0f;
        c.cam.rotation = 0.0f;

        // Offset
        c.cam.offset = { (float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f };
        c.offsetBase = c.cam.offset;

        auto tview = scene->r.view<TileMapComponent>();
        for (auto te : tview) {
            auto &tm = tview.get<TileMapComponent>(te);
            float guessScale = 1.0f;
            if (!tm.tiles.empty()) guessScale = tm.tiles[0].scale;
            c.worldW = tm.width  * tm.tileSize * guessScale;
            c.worldH = tm.height * tm.tileSize * guessScale;
            break;
        }
    }
}

void CameraFollowSystem::update() {
    auto camView = scene->r.view<Camera2DComponent>();
    auto targetView = scene->r.view<TransformComponent, PlayerComponent>();


    auto camEnt = *camView.begin();
    auto &camc = camView.get<Camera2DComponent>(camEnt);
    auto tarEnt = *targetView.begin();
    auto &t = targetView.get<TransformComponent>(tarEnt);

    Vector2 targetPos = t.position;

    float dzHalfW = camc.deadzoneW * 0.5f;
    float dzHalfH = camc.deadzoneH * 0.5f;

    Vector2 desiredTarget = camc.cam.target;

    Vector2 rel = {
            targetPos.x - camc.cam.target.x,
            targetPos.y - camc.cam.target.y
    };

    if      (rel.x >  dzHalfW) desiredTarget.x += (rel.x - dzHalfW);
    else if (rel.x < -dzHalfW) desiredTarget.x += (rel.x + dzHalfW);
    if      (rel.y >  dzHalfH) desiredTarget.y += (rel.y - dzHalfH);
    else if (rel.y < -dzHalfH) desiredTarget.y += (rel.y + dzHalfH);

    float dt = GetFrameTime();
    float tSmooth = 1.0f - std::pow(1.0f - camc.followSmooth, dt * 60.0f);
    camc.cam.target.x = lerp(camc.cam.target.x, desiredTarget.x, tSmooth);
    camc.cam.target.y = lerp(camc.cam.target.y, desiredTarget.y, tSmooth);

    if (camc.worldW > 0 && camc.worldH > 0) {
        float vw = (float)GetScreenWidth()  / camc.cam.zoom;
        float vh = (float)GetScreenHeight() / camc.cam.zoom;

        float minX = vw * 0.5f;
        float minY = vh * 0.5f;
        float maxX = camc.worldW  - vw * 0.5f;
        float maxY = camc.worldH  - vh * 0.5f;

        camc.cam.target.x = std::clamp(camc.cam.target.x, minX, std::max(minX, maxX));
        camc.cam.target.y = std::clamp(camc.cam.target.y, minY, std::max(minY, maxY));
    }
}


void CameraZoomInputSystem::update() {
    auto view = scene->r.view<Camera2DComponent>();
    if (view.empty()) return;

    auto e = *view.begin();
    auto &cam = view.get<Camera2DComponent>(e);

    // mouse
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) cam.targetZoom += wheel * 0.1f;

    // teclas
    if (IsKeyDown(KEY_Q)) cam.targetZoom -= 0.02f;
    if (IsKeyDown(KEY_E)) cam.targetZoom += 0.02f;

    // límites
    cam.targetZoom = std::clamp(cam.targetZoom, 0.5f, 3.0f);
}


void CameraEffectsSystem::update() {
    auto camView = scene->r.view<Camera2DComponent, TransformComponent>();
    for (auto e : camView) {
        auto &c = camView.get<Camera2DComponent>(e);

        c.cam.zoom = lerp(c.cam.zoom, c.targetZoom, c.zoomLerp);

        c.cam.offset = c.offsetBase;

    }
}

void CameraBeginRenderSystem::render() {
    auto view = scene->r.view<Camera2DComponent>();
    for (auto e : view) {
        const auto &c = view.get<Camera2DComponent>(e);
        BeginMode2D(c.cam);
        break;
    }
}

void CameraEndRenderSystem::render() {
    EndMode2D();
}