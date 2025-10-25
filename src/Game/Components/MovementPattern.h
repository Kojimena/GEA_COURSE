#pragma once
#include <string>
#include <vector>
#include <lua.hpp>

struct MovementPatternComponent {
    std::string scriptPath;
    lua_State* L = nullptr;

    float speed = 50.0f;
    bool initialized = false;

    // Tracking movement
    float trackingDistance = 300.0f;

    // Circular movement
    float orbitRadius = 20.0f;
    float orbitSpeed = 4.0f;
    bool orbitAroundPlayer = false;
    float orbitCenterX = 0.0f;
    float orbitCenterY = 0.0f;
    float attackSpeed = 80.0f;
    float attackInterval = 2.0f;

    // Patrol movement
    std::vector<std::pair<float, float>> waypoints;
    int currentWaypoint = 0;
    float waypointThreshold = 10.0f;
    float patrolSpeed = 40.0f;

    float scriptTime = 0.0f;

    ~MovementPatternComponent() {
        if (L) {
            lua_close(L);
            L = nullptr;
        }
    }
};