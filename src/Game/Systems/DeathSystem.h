#pragma once
#include "ECS/System.h"

class DeathSystem : public System {
public:
    void update() override;
};