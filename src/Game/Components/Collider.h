#pragma once
struct ColliderComponent {
    float ox = 0;   // offset X
    float oy = 0;   // offset Y
    float w  = 16;  // ancho
    float h  = 16;  // alto
    bool triggered = false; // para detectar colisiones sin bloquear
};
