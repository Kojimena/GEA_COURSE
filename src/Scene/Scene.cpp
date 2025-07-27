#include "Scene.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include <iostream>

Scene::Scene(const std::string& name)
        : name(name)
{
    std::cout << "Scene constructed: " << name << std::endl;
}

Scene::~Scene()
{
    std::cout << "Scene Destroyed" << std::endl;
    // Do not delete systems; user manages their lifetime
}

Entity Scene::createEntity(const std::string& name, int x, int y, Color color)
{
    Entity entity = { r.create(), this };
    entity.addComponent<NameComponent>(name);
    entity.addComponent<TransformComponent>(Vector2{ static_cast<float>(x), static_cast<float>(y) });
    entity.addComponent<SpriteComponent>(color);
    return entity;
}

void Scene::addSystem(System* system)
{
    systems.push_back(system);
    system->setScene(this);
}

void Scene::setup()
{
    std::cout << "Scene Setup" << std::endl;
    for (auto sys : systems) {
        sys->setup();
    }
}

void Scene::update()
{
    std:: cout << "Scene Update" << std::endl;
    for (auto sys : systems) {
        sys->update();
    }
}

void Scene::render()
{
    std::cout << "Scene Render" << std::endl;
    for (auto sys : systems) {
        sys->render();
    }
}