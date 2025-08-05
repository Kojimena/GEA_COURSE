#include "Breakout.h"
#include "Components.h"
#include "ECS/Entity.h"
#include "Systems.cpp"
#include "ImGui/ImGui.h"

Breakout::Breakout() : Game("Breakout", SCREEN_WIDTH, SCREEN_HEIGHT) {
    Scene* gameplayScene = createGameplayScene();
    setScene(gameplayScene);
}

Breakout::~Breakout() {
}

inline float f(int x) { return static_cast<float>(x); }

Scene* Breakout::createGameplayScene() {
    Scene* gameplayScene = new Scene("Gameplay");

    // — Ball —
    Entity ball = gameplayScene->createEntity("ball", 100, 200);
    ball.addComponent<SizeComponent>(f(24), f(24));
    ball.addComponent<ColliderComponent>(false);
    ball.addComponent<VelocityComponent>(Vector2{100, 100});
    ball.addComponent<SpriteComponent>(WHITE);


    // — Paddle —
    Entity paddle = gameplayScene->createEntity("paddle", (screen_width / 2) - 50, screen_height - 20);
    paddle.addComponent<SizeComponent>(f(200), f(20));
    paddle.addComponent<PlayerComponent>(f(200));
    paddle.addComponent<VelocityComponent>(Vector2{0, 0});
    paddle.addComponent<SpriteComponent>(GRAY);

    // — Bricks —
    Color blockColors[6] = { LIME, GREEN, BLUE, SKYBLUE, PINK, ORANGE };
    const int rows = 5;
    const int cols = 8;
    float bw = GetScreenWidth()  / float(cols);
    float bh = 20.0f;

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            auto blk = gameplayScene->createEntity("block",
                                                   j * bw,
                                                   i * bh
            );
            blk.addComponent<SizeComponent>(bw - 2.0f, bh - 2.0f);
            blk.addComponent<BrickComponent>();
            blk.addComponent<SpriteComponent>(blockColors[i]);
        }
    }

    // Add systems
    gameplayScene->addSystem(new HelloSystem());
    gameplayScene->addSystem(new InputSystem());
    gameplayScene->addSystem(new MovementSystem());
    gameplayScene->addSystem(new BoundsSystem());
    gameplayScene->addSystem(new CollisionSystem());
    gameplayScene->addSystem(new RenderSystem());
    gameplayScene->addSystem(new ImGuiSystem());


    return gameplayScene;
}