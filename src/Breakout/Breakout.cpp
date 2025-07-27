#include "Breakout.h"
#include "Components.h"
#include "ECS/Entity.h"
#include "Systems.cpp"

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
    Entity ball = gameplayScene->createEntity("ball", 100, 100);
    ball.addComponent<SizeComponent>(f(30), f(30));
    ball.addComponent<ColliderComponent>(false);
    ball.addComponent<VelocityComponent>(Vector2{100, 100});
    ball.addComponent<SpriteComponent>(WHITE);


    // — Paddle —
    Entity paddle = gameplayScene->createEntity("paddle", (screen_width / 2) - 50, screen_height - 20);
    paddle.addComponent<SizeComponent>(f(100), f(20));
    paddle.addComponent<PlayerComponent>(f(200));
    paddle.addComponent<VelocityComponent>(Vector2{0, 0});
    paddle.addComponent<SpriteComponent>(GRAY);

    // — Bricks —
    Color pal[6]={LIME,GREEN,BLUE,SKYBLUE,PINK,ORANGE};
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 5; ++j) {
            Entity brick = gameplayScene->createEntity("brick", i * 60 + 20, j * 30 + 50);
            brick.addComponent<SizeComponent>(f(50), f(20));
            brick.addComponent<SpriteComponent>(pal[j % 6]);
            brick.addComponent<BrickComponent>();
        }
    }

    // Add systems
    gameplayScene->addSystem(new HelloSystem());
    gameplayScene->addSystem(new InputSystem());
    gameplayScene->addSystem(new MovementSystem());
    gameplayScene->addSystem(new CollisionSystem());
    gameplayScene->addSystem(new RenderSystem());

    return gameplayScene;
}