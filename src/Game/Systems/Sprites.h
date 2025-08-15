#pragma once
#include "ECS/System.h"

//setup
class SpriteSystem : public System {
public:

    void setup() override;
    void update() override;

    void render() override;

};

class SpriteMovementSystem : public System {
public:

    void update() override;
};
