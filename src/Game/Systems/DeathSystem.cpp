#include "DeathSystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/HealthComponent.h"
#include "Scene/Scene.h"
#include <vector>
#include <iostream>
#include "Game/Game.h"

void DeathSystem::update() {

    auto view = scene->r.view<HealthComponent>();

    bool playerDied = false;

    for (auto entity : view) {
        auto& health = view.get<HealthComponent>(entity);

        if (health.currentHealth <= 0) {
            if (scene->r.all_of<PlayerComponent>(entity)) {
                playerDied = true;
            }
        }
    }

    // cambio a gameover
    if (playerDied && scene->game) {
        scene->game->changeScene("GameOver");
    }
}