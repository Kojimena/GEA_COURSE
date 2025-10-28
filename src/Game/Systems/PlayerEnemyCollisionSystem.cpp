#include "PlayerEnemyCollisionSystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/Enemy.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/MovementPattern.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>
#include <iostream>

void PlayerEnemyCollisionSystem::update() {
    float currentTime = GetTime();

    auto playerView = scene->r.view<PlayerComponent, TransformComponent, ColliderComponent, HealthComponent>();

    auto enemyView = scene->r.view<MovementPatternComponent, TransformComponent>();

    for (auto playerEntity : playerView) {
        auto& playerTf = playerView.get<TransformComponent>(playerEntity);
        auto& playerCol = playerView.get<ColliderComponent>(playerEntity);
        auto& playerHealth = playerView.get<HealthComponent>(playerEntity);

        if (currentTime - playerHealth.lastDamageTime < playerHealth.damageCooldown) {
            continue;
        }

        Rectangle playerRect = {
                playerTf.position.x + playerCol.ox,
                playerTf.position.y + playerCol.oy,
                playerCol.w,
                playerCol.h
        };

        for (auto enemyEntity : enemyView) {
            auto& enemyTf = enemyView.get<TransformComponent>(enemyEntity);

            Rectangle enemyRect = {
                    enemyTf.position.x,
                    enemyTf.position.y,
                    16.0f,
                    16.0f
            };

            if (CheckCollisionRecs(playerRect, enemyRect)) {
                playerHealth.currentHealth -= enemyDamage;
                playerHealth.lastDamageTime = currentTime;

                break;
            }
        }
    }
}