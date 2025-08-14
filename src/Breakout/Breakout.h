#pragma once

#include "Game/Game.h"

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 600;

class Breakout : public Game {
    // Breakout game class that inherits from Game
public:
    Breakout();
    ~Breakout();

private:
    Scene* createGameplayScene();
    Scene* createSpriteScene();

};