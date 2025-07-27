#pragma once

#include "Game/Game.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Breakout : public Game {
    // Breakout game class that inherits from Game
public:
    Breakout();
    ~Breakout();

private:
    Scene* createGameplayScene();
};