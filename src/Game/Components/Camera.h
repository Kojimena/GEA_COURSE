#pragma once
#include <raylib.h>
#include <cstdint>

struct CameraTargetTag {};

struct Camera2DComponent {
    Camera2D cam{};

    float followSmooth = 0.12f;

    float deadzoneW = 140.0f;
    float deadzoneH = 90.0f;

    // Límites del mundo
    float worldW = 0.0f;
    float worldH = 0.0f;


    float targetZoom = 1.0f;
    float zoomLerp   = 0.15f;

    Vector2 offsetBase{ 0.0f, 0.0f };
};
