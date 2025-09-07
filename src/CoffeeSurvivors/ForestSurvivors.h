#pragma once

#include "Game/Game.h"

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 600;

class ForestSurvivors : public Game {
    // Breakout game class that inherits from Game
public:
    ForestSurvivors();
    ~ForestSurvivors();

private:
    Scene* createSpriteScene();

};