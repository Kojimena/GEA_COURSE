#pragma once
#include "ECS/System.h"
#include "Game/Components/MovementPattern.h"
#include <lua.hpp>
#include <entt/entt.hpp>
#include "ECS/Components.h"

class MovementPatternSystem : public System {
public:
    void update() override;

private:
    void initializeScript(MovementPatternComponent& pattern);
    void updatePattern(entt::entity entity, MovementPatternComponent& pattern,
                       TransformComponent& transform, float dt);

    static int lua_GetPlayerPosition(lua_State* L);
    static int lua_GetDeltaTime(lua_State* L);
    static int lua_GetEntityPosition(lua_State* L);
    static int lua_SetEntityPosition(lua_State* L);
    static int lua_Distance(lua_State* L);
    static int lua_Lerp(lua_State* L);
    static int lua_Normalize(lua_State* L);

    static Scene* s_currentScene;
    static entt::entity s_currentEntity;
};