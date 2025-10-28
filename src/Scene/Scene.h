#pragma once

#include <string>
#include <vector>
#include <entt/entt.hpp>
#include "ECS/Components.h"

class Entity;
class System;
class Game;

class Scene {
private:
    std::vector<System*> systems;
    std::string name;

public:
    Scene(const std::string&);
    ~Scene();

    entt::registry r;

    Game* game = nullptr;
    std::string getName() const { return name; }


    Entity createEntity(
            const std::string& name = "NO NAME",
            int x = 0,
            int y = 0,
            Color color = WHITE
    );

    void addSystem(System* system);

    void setup();
    void update();
    void render();
};