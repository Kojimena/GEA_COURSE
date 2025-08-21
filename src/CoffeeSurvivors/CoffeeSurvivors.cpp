#include "CoffeeSurvivors.h"
#include "Game/Components/Sprites.h"
#include "ECS/Entity.h"
#include "Game/Systems/Sprites.h"
#include "ImGui/ImGui.h"
#include "./Game/Components/Sprites.h"
#include "Game/Systems/Sprites.h"
#include "Game/Systems/Background.h"

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
            
    s->addSystem(new BackgroundSystem());
    s->addSystem(new SpriteSystem());
    s->addSystem(new SpriteMovementSystem());



    return s;
}

inline float f(int x) { return static_cast<float>(x); }