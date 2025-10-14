#include "EnemySpawnSystem.h"
#include "Game/Components/Enemy.h"
#include "Game/Components/Sprites.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "raylib.h"
#include <iostream>

void EnemySpawnSystem::setup() {
}

void EnemySpawnSystem::update() {

    static bool spawned = false;
    if (spawned) return;
    spawned = true;

    Scene* scene = this->scene;
    Entity enemy = scene->createEntity("enemy", 140, 100); // <- cerca del player


    enemy.addComponent<SpriteLayerComponent>(
            "../src/assets/enemy.png", 16,16, 2, 8, 600, 0, 0, 0, 0
    );
}
