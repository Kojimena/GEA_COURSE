#include "CoffeeSurvivors.h"
#include "Game/Components/Sprites.h"
#include "ECS/Entity.h"
#include "Game/Systems/Sprites.h"
#include "ImGui/ImGui.h"
#include "./Game/Components/Player.h"
#include "Game/Systems/Sprites.h"
#include "Game/Systems/Background.h"
#include "Game/Components/Enemy.h"
#include "Game/Systems/Enemy.h"


CoffeeSurvivors::CoffeeSurvivors() : Game("CoffeeSurvivors", SCREEN_WIDTH, SCREEN_HEIGHT) {
    Scene* gameplayScene = createSpriteScene();
    setScene(gameplayScene);
}

CoffeeSurvivors::~CoffeeSurvivors() {
}

Scene* CoffeeSurvivors::createSpriteScene() {


    Scene* s = new Scene("Sprite");
    // ====== FONDO ======
    {
        Entity bg = s->createEntity("bg", 0, 0);
        bg.addComponent<BackgroundComponent>(BackgroundComponent{
                .path = "../src/assets/bgcoffee.png",
                .texture = {},
                .parallax = 0.0f,
                .stretchToScreen = true
        });
    }

    // girl player entity
    Entity girl = s->createEntity("girl", 100, 100);
    girl.addComponent<SpriteLayerComponent>(
            "../src/assets/girl.png",
            32, //sprite size
            4, // scale factor
            8, // animation frames
            600, // animation duration
            0,  // offset x
            0,  // offset y
            1, // index x
            0); // index y
    girl.addComponent<PlayerComponent>();

    auto makeEnemy = [&](float x, float y, float l, float r) {
        Entity e = s->createEntity("enemy", x, y);
        e.addComponent<SpriteLayerComponent>(
                "../src/assets/zombie.png",
                32, 4, 8, 600, 0, 0, 0, 0
        );

        e.addComponent<EnemyAIComponent>(EnemyAIComponent{
                .speed = 60.0f, .leftX = l, .rightX = r, .dir = 1
        });
        return e;
    };

// Ejemplos:
    makeEnemy(1000, 300, 360, 560);
    makeEnemy(1000, 300, 760, 960);


    s->addSystem(new BackgroundSystem());
    s->addSystem(new SpriteMovementSystem());  // jugador: mueve y setea anim
    s->addSystem(new EnemyAISystem());         // enemigos: mueven y setean anim (lo agregamos abajo)
    s->addSystem(new SpriteSystem());          // avanza frames y DIBUJA



    return s;
}

inline float f(int x) { return static_cast<float>(x); }