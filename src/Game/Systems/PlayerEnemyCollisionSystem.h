#pragma once
#include "ECS/System.h"

class PlayerEnemyCollisionSystem : public System {
public:
    void update() override;

private:
    int enemyDamage = 5;
};