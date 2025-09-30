#include "ForestSurvivors.h"
#include "Game/Components/Sprites.h"
#include "ECS/Entity.h"
#include "Game/Systems/Sprites.h"
#include "./Game/Components/Player.h"
#include "Game/Components/Enemy.h"
#include "Game/Systems/Enemy.h"
#include "Game/Components/Background.h"
#include "Game/Systems/TilemapRender.h"
#include "Game/Systems/AutoTiling.h"
#include "Game/Components/Tilemap.h"
#include "Game/Components/Camera.h"
#include "Game/Systems/Camera.h"
#include "Game/Systems/TilemapSetupSystem.h"



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

    // girl player entity
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

    auto makeEnemy = [&](float x, float y, float l, float r) {
        Entity e = s->createEntity("enemy", x, y);
        e.addComponent<SpriteLayerComponent>(
                "../src/assets/enemy.png",
                16,16, 2, 8, 600, 0, 0, 0, 0
        );

        e.addComponent<EnemyAIComponent>(EnemyAIComponent{
                .speed = 60.0f, .leftX = l, .rightX = r, .dir = 1
        });
        return e;
    };

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




    makeEnemy(1000, 300, 360, 560);
    makeEnemy(1000, 300, 760, 960);

    // Setup
    s->addSystem(new AutoTilingSetupSystem());  // calcula ix/iy de tiles, ya lo tienes
    s->addSystem(new CameraSetupSystem());      // inicializa offset/zoom/world bounds

    // Lógica
    s->addSystem(new SpriteMovementSystem());   // mueve player con teclado
    s->addSystem(new EnemyAISystem());          // IA horizontal enemigos
    s->addSystem(new CameraFollowSystem());     // sigue al player (deadzone + clamp)
    s->addSystem(new CameraEffectsSystem());    // aplica shake/zoom
    s->addSystem(new CameraZoomInputSystem());


    // Render Pass (con cámara)
    s->addSystem(new CameraBeginRenderSystem()); // BeginMode2D
    s->addSystem(new TileMapRenderSystem());     // dibuja tilemap
    s->addSystem(new SpriteSystem());            // anima/dibuja sprites
    s->addSystem(new CameraEndRenderSystem());   // EndMode2D





    return s;
}

inline float f(int x) { return static_cast<float>(x); }