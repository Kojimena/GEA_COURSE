#include "Game.h"
#include <format>
#include <iostream>

Game::Game(const char* title, int width, int height)
        : screen_width(width), screen_height(height)
{
    InitWindow(width, height, title);
    SetTargetFPS(60);
    std::cout << "Game start" << std::endl;
    isRunning = true;
    frameCount = 0;
    dT = 0.0f;
    FPS = 0.0f;
}

Game::~Game() {
    clean();
}

void Game::setup() {
    if (currentScene) {
        currentScene->setup();
    }
}

void Game::frameStart() {
    std::cout << "Frame Start:" << frameCount << std::endl;
    dT = GetFrameTime(); // seconds
}

void Game::frameEnd() {
    frameCount++;
    FPS = static_cast<float>(GetFPS());
    std::cout << "Frame End: " << frameCount << ", FPS: " << FPS << std::endl;
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
        update();
        render();
        frameEnd();
    }

    clean();
}

void Game::setScene(Scene* newScene) {
    currentScene = std::move(newScene);
}

Scene* Game::getCurrentScene() const {
    return currentScene;
}