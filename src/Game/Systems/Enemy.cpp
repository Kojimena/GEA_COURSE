#include "Enemy.h"
#include "../Components/Enemy.h"
#include "Scene/Scene.h"
#include "ECS/Components.h"
#include "Game/Components/Sprites.h"
#include <raylib.h>

void EnemyAISystem::update() {
    float dt = GetFrameTime();
    auto view = scene->r.view<TransformComponent, SpriteLayerComponent, EnemyAIComponent>();

    for (auto e : view) {
        auto &t  = view.get<TransformComponent>(e);
        auto &sp = view.get<SpriteLayerComponent>(e);
        auto &ai = view.get<EnemyAIComponent>(e);

        // Patrullaje
        t.position.x += ai.dir * ai.speed * dt;
        if (t.position.x > ai.rightX) { t.position.x = ai.rightX; ai.dir = -1; }
        if (t.position.x < ai.leftX)  { t.position.x = ai.leftX;  ai.dir =  1; }

        if (sp.iy != 0) { sp.iy = 0; sp.ix = 0; sp.lastUpdate = (long)(GetTime()*1000.0); }
        sp.animationFrame    = 8;
        sp.animationDuration = 600;
    }
}
