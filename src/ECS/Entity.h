#pragma once

#include <entt/entt.hpp>
#include <iostream>
#include "Scene/Scene.h"

class Entity
{
public:
    Entity(entt::entity e, Scene* s) {
        std::cout << "Entity Created: " << std::endl;

        handle = e;
        scene = s;
    }

    ~Entity() {
        std::cout << "Entity Destroyed: "<< std::endl;
    }

    template<typename T>
    auto& addComponent(auto&&... args) {
        return scene->r.emplace_or_replace<T>(handle, std::forward<decltype(args)>(args)...);
    }

    template<typename T>
    void removeComponent() {
        scene->r.remove<T>(handle);
    }

private:
    entt::entity handle;
    Scene* scene;
};