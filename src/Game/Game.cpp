#include "Game.h"
#include "Scene/Scene.h"
#include "ImGui/ImGui.h"
#include <format>
#include <iostream>

Game::Game(const char* title, int width, int height)
        : screen_width(width), screen_height(height) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
    std::cout << "Game start" << std::endl;
    isRunning = true;
    frameCount = 0;
    dT = 0.0f;
    FPS = 0.0f;
    currentScene = nullptr;
}

Game::~Game() {
    for (auto& pair : scenes) {
        delete pair.second;
    }
    scenes.clear();
    clean();
}

void Game::setup() {
    if (currentScene) {
        currentScene->setup();
    }
}

void Game::frameStart() {
    dT = GetFrameTime();
}

void Game::frameEnd() {
    frameCount++;
    FPS = static_cast<float>(GetFPS());
}

void Game::handleEvents() {
    if (WindowShouldClose()) {
        isRunning = false;
    }
}

void Game::update() {
    if (currentScene) {
        currentScene->update();
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (currentScene) {
        currentScene->render();
    }

    DrawText(TextFormat("FPS: %.2f", FPS), 10, 10, 20, DARKGRAY);

    EndDrawing();
}

void Game::clean() {
    if (!WindowShouldClose()) {
        CloseWindow();
    }
    std::cout << "Game Over" << std::endl;
}

bool Game::running() const {
    return isRunning;
}

void Game::run() {
    setup();

    while (running()) {
        frameStart();
        handleEvents();

        if (!ImGuiSystem::isPaused) {
            update();
        }
        render();
        frameEnd();
    }

    clean();
}

void Game::setScene(Scene* newScene) {
    if (newScene) {
        currentScene = newScene;
        scenes[newScene->getName()] = newScene;
        newScene->setup();
        std::cout << "Escena establecida: " << newScene->getName() << std::endl;
    }
}

void Game::changeScene(const std::string& sceneName) {
    auto it = scenes.find(sceneName);
    if (it != scenes.end()) {
        std::cout << "Cambiando a escena: " << sceneName << std::endl;
        currentScene = it->second;
    } else {
        std::cout << "Error: Escena '" << sceneName << "' no encontrada" << std::endl;
    }
}

Scene* Game::getCurrentScene() const {
    return currentScene;
}