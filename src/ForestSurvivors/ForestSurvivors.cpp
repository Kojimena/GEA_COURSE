#include "ForestSurvivors.h"
#include "Game/Components/Sprites.h"
#include "ECS/Entity.h"
#include "Game/Systems/Sprites.h"
#include "./Game/Components/Player.h"
#include "Game/Components/Enemy.h"
#include "Game/Systems/Enemy.h"
#include "Game/Systems/TilemapRender.h"
#include "Game/Systems/AutoTiling.h"
#include "Game/Components/Camera.h"
#include "Game/Systems/Camera.h"
#include "Game/Systems/TilemapSetupSystem.h"
#include "Game/Systems/IntGridRenderSystem.h"
#include "Game/Systems/ColliderRenderSystem.h"
#include "Game/Systems/CollisionSystem.h"
#include "Game/Systems/TileTriggerSystem.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Systems/EnemySpawnSystem.h"

ForestSurvivors::ForestSurvivors() : Game("ForestSurvivors", SCREEN_WIDTH, SCREEN_HEIGHT) {
    Scene* gameplayScene = createSpriteScene();
    setScene(gameplayScene);
}

ForestSurvivors::~ForestSurvivors() {
}

Scene* ForestSurvivors::createSpriteScene() {


    Scene* s = new Scene("Sprite");
    {
    }

    // PLAYER
    Entity girl = s->createEntity("girl", 100, 100);
    girl.addComponent<SpriteLayerComponent>(
            "../src/assets/walk.png",
            16, // frame width
            32, // frame height
            2, // scale factor
            4, // animation frames
            600, // animation duration
            0,  // offset x
            0,  // offset y
            0, // index x
            0); // index y
    girl.addComponent<PlayerComponent>();
    girl.addComponent<HealthComponent>(HealthComponent{
            .maxHealth = 100.0f,
            .currentHealth = 100.0f,
            .poisonDamageRate = 10.0f
    });

    girl.addComponent<ColliderComponent>(ColliderComponent{
            .ox = 0.0f,
            .oy = 0.0f,
            .w = 16.0f * 2,  // ancho
            .h = 32.0f * 2   // alto
    });

//    auto makeEnemy = [&](float x, float y, float l, float r) {
//        Entity e = s->createEntity("enemy", x, y);
//        e.addComponent<SpriteLayerComponent>(
//                "../src/assets/enemy.png",
//                16,16, 2, 8, 600, 0, 0, 0, 0
//        );
//
//        e.addComponent<EnemyAIComponent>(EnemyAIComponent{
//                .speed = 60.0f, .leftX = l, .rightX = r, .dir = 1
//        });
//        return e;
//    };


    auto* mapSetup = new TilemapSetupSystem();
    mapSetup->assetsDir = "../src/assets";
    mapSetup->tileSize  = 16;
    s->addSystem(mapSetup);



    // --- CÁMARA ---
    Entity camE = s->createEntity("camera", 0, 0);
    auto &cam = camE.addComponent<Camera2DComponent>();
    cam.followSmooth = 0.14f;
    cam.deadzoneW = 160.0f;
    cam.deadzoneH = 100.0f;
    cam.targetZoom = 1.0f;




//    makeEnemy(1000, 300, 360, 560);
//    makeEnemy(1000, 300, 760, 960);


    // Setup
    s->addSystem(new AutoTilingSetupSystem());
    s->addSystem(new CameraSetupSystem());




    // Lógica
    s->addSystem(new SpriteMovementSystem());
    s->addSystem(new CollisionSystem());
    s->addSystem(new EnemyAISystem());
    s->addSystem(new EnemySpawnSystem());
    s->addSystem(new CameraFollowSystem());
    s->addSystem(new CameraEffectsSystem());
    s->addSystem(new CameraZoomInputSystem());

    // Render
    s->addSystem(new CameraBeginRenderSystem()); // BeginMode2D


    s->addSystem(new TileMapRenderSystem());
    s->addSystem(new SpriteSystem());
    s->addSystem(new IntgridRenderSystem());


    s->addSystem(new ColliderRenderSystem());
    s->addSystem(new CameraEndRenderSystem());   // EndMode2D
    s->addSystem(new TileTriggerSystem());


    return s;
}

inline float f(int x) { return static_cast<float>(x); }