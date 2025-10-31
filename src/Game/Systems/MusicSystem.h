#pragma once
#include "ECS/System.h"
#include <raylib.h>

class MusicSystem : public System {
private:
    Sound backgroundSound;

public:
    void setup() override;
    void update() override;
    ~MusicSystem();
};