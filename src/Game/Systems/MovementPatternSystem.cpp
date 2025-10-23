#include "MovementPatternSystem.h"
#include "Game/Components/Player.h"
#include "Game/Components/Enemy.h"
#include "ECS/Components.h"
#include "Scene/Scene.h"
#include <raylib.h>
#include <cmath>
#include <iostream>

Scene* MovementPatternSystem::s_currentScene = nullptr;
entt::entity MovementPatternSystem::s_currentEntity = entt::null;

void MovementPatternSystem::update() {
    float dt = GetFrameTime();
    s_currentScene = scene;

    auto view = scene->r.view<MovementPatternComponent, TransformComponent>();

    for (auto entity : view) {
        auto& pattern = view.get<MovementPatternComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        s_currentEntity = entity;

        if (!pattern.initialized) {
            initializeScript(pattern);
        }

        if (pattern.L && pattern.initialized) {
            updatePattern(entity, pattern, transform, dt);
        }
    }
}

void MovementPatternSystem::initializeScript(MovementPatternComponent& pattern) {
    pattern.L = luaL_newstate();
    luaL_openlibs(pattern.L);

    lua_register(pattern.L, "GetPlayerPosition", lua_GetPlayerPosition);
    lua_register(pattern.L, "GetDeltaTime", lua_GetDeltaTime);
    lua_register(pattern.L, "GetEntityPosition", lua_GetEntityPosition);
    lua_register(pattern.L, "SetEntityPosition", lua_SetEntityPosition);
    lua_register(pattern.L, "Distance", lua_Distance);
    lua_register(pattern.L, "Lerp", lua_Lerp);
    lua_register(pattern.L, "Normalize", lua_Normalize);

    if (luaL_dofile(pattern.L, pattern.scriptPath.c_str()) != LUA_OK) {
        std::cerr << "Error loading Lua script: " << lua_tostring(pattern.L, -1) << std::endl;
        lua_close(pattern.L);
        pattern.L = nullptr;
        return;
    }

    lua_getglobal(pattern.L, "Init");
    if (lua_isfunction(pattern.L, -1)) {
        lua_newtable(pattern.L);

        lua_pushnumber(pattern.L, pattern.speed);
        lua_setfield(pattern.L, -2, "speed");

        lua_pushnumber(pattern.L, pattern.trackingDistance);
        lua_setfield(pattern.L, -2, "trackingDistance");

        lua_pushnumber(pattern.L, pattern.orbitRadius);
        lua_setfield(pattern.L, -2, "orbitRadius");

        lua_pushnumber(pattern.L, pattern.orbitSpeed);
        lua_setfield(pattern.L, -2, "orbitSpeed");

        lua_pushboolean(pattern.L, pattern.orbitAroundPlayer);
        lua_setfield(pattern.L, -2, "orbitAroundPlayer");

        lua_pushnumber(pattern.L, pattern.orbitCenterX);
        lua_setfield(pattern.L, -2, "orbitCenterX");

        lua_pushnumber(pattern.L, pattern.orbitCenterY);
        lua_setfield(pattern.L, -2, "orbitCenterY");

        lua_pushnumber(pattern.L, pattern.patrolSpeed);
        lua_setfield(pattern.L, -2, "patrolSpeed");

        // Waypoints
        lua_newtable(pattern.L);
        for (size_t i = 0; i < pattern.waypoints.size(); ++i) {
            lua_newtable(pattern.L);
            lua_pushnumber(pattern.L, pattern.waypoints[i].first);
            lua_setfield(pattern.L, -2, "x");
            lua_pushnumber(pattern.L, pattern.waypoints[i].second);
            lua_setfield(pattern.L, -2, "y");
            lua_rawseti(pattern.L, -2, i + 1);
        }
        lua_setfield(pattern.L, -2, "waypoints");

        if (lua_pcall(pattern.L, 1, 0, 0) != LUA_OK) {
            std::cerr << "Error calling Init: " << lua_tostring(pattern.L, -1) << std::endl;
        }
    } else {
        lua_pop(pattern.L, 1);
    }

    pattern.initialized = true;
}

void MovementPatternSystem::updatePattern(entt::entity entity, MovementPatternComponent& pattern,
                                          TransformComponent& transform, float dt) {
    pattern.scriptTime += dt;

    lua_getglobal(pattern.L, "Update");
    if (!lua_isfunction(pattern.L, -1)) {
        lua_pop(pattern.L, 1);
        return;
    }

    lua_pushnumber(pattern.L, dt);
    lua_pushnumber(pattern.L, pattern.scriptTime);
    lua_pushnumber(pattern.L, transform.position.x);
    lua_pushnumber(pattern.L, transform.position.y);

    if (lua_pcall(pattern.L, 4, 2, 0) != LUA_OK) {
        std::cerr << "Error calling Update: " << lua_tostring(pattern.L, -1) << std::endl;
        lua_pop(pattern.L, 1);
        return;
    }

    float newY = lua_tonumber(pattern.L, -1);
    float newX = lua_tonumber(pattern.L, -2);
    lua_pop(pattern.L, 2);

    transform.position.x = newX;
    transform.position.y = newY;
}

int MovementPatternSystem::lua_GetPlayerPosition(lua_State* L) {
    if (!s_currentScene) {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
        return 2;
    }

    auto view = s_currentScene->r.view<PlayerComponent, TransformComponent>();
    for (auto entity : view) {
        const auto& tf = view.get<TransformComponent>(entity);
        lua_pushnumber(L, tf.position.x);
        lua_pushnumber(L, tf.position.y);
        return 2;
    }

    lua_pushnumber(L, 0);
    lua_pushnumber(L, 0);
    return 2;
}

int MovementPatternSystem::lua_GetDeltaTime(lua_State* L) {
    lua_pushnumber(L, GetFrameTime());
    return 1;
}

int MovementPatternSystem::lua_GetEntityPosition(lua_State* L) {
    if (!s_currentScene || s_currentEntity == entt::null) {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
        return 2;
    }

    if (s_currentScene->r.all_of<TransformComponent>(s_currentEntity)) {
        const auto& tf = s_currentScene->r.get<TransformComponent>(s_currentEntity);
        lua_pushnumber(L, tf.position.x);
        lua_pushnumber(L, tf.position.y);
        return 2;
    }

    lua_pushnumber(L, 0);
    lua_pushnumber(L, 0);
    return 2;
}

int MovementPatternSystem::lua_SetEntityPosition(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);

    if (s_currentScene && s_currentEntity != entt::null) {
        if (s_currentScene->r.all_of<TransformComponent>(s_currentEntity)) {
            auto& tf = s_currentScene->r.get<TransformComponent>(s_currentEntity);
            tf.position.x = x;
            tf.position.y = y;
        }
    }

    return 0;
}

int MovementPatternSystem::lua_Distance(lua_State* L) {
    float x1 = luaL_checknumber(L, 1);
    float y1 = luaL_checknumber(L, 2);
    float x2 = luaL_checknumber(L, 3);
    float y2 = luaL_checknumber(L, 4);

    float dx = x2 - x1;
    float dy = y2 - y1;
    float dist = sqrtf(dx * dx + dy * dy);

    lua_pushnumber(L, dist);
    return 1;
}

int MovementPatternSystem::lua_Lerp(lua_State* L) {
    float a = luaL_checknumber(L, 1);
    float b = luaL_checknumber(L, 2);
    float t = luaL_checknumber(L, 3);

    float result = a + (b - a) * t;
    lua_pushnumber(L, result);
    return 1;
}

int MovementPatternSystem::lua_Normalize(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);

    float len = sqrtf(x * x + y * y);
    if (len > 0.0001f) {
        x /= len;
        y /= len;
    }

    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}