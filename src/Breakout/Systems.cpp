//#include "ECS/System.h"
//#include "ECS/Components.h"
//#include "Breakout/Components.h"
//#include "Scene/Scene.h"
//#include <iostream>
//
//class HelloSystem : public System {
//public:
//    void setup() override {
//        std::cout << "Hello, BreakOut!" << std::endl;
//    }
//};
//
//class InputSystem : public System {
//public:
//    void update() override {
//        auto view = scene->r.view<PlayerComponent, VelocityComponent>();
//        for (auto entity : view) {
//            auto& player = view.get<PlayerComponent>(entity);
//            auto& vel = view.get<VelocityComponent>(entity);
//
//            vel.velocity.x = 0.0f;
//            if (IsKeyDown(KEY_LEFT))  vel.velocity.x = -player.moveSpeed;
//            if (IsKeyDown(KEY_RIGHT)) vel.velocity.x =  player.moveSpeed;
//        }
//    }
//};
//
//class MovementSystem : public System {
//public:
//    void update() override {
//        float dT = GetFrameTime();
//        auto view = scene->r.view<TransformComponent, VelocityComponent>();
//        for (auto entity : view) {
//            auto& pos = view.get<TransformComponent>(entity);
//            auto& vel = view.get<VelocityComponent>(entity);
//            pos.position.x += vel.velocity.x * dT;
//            pos.position.y += vel.velocity.y * dT;
//        }
//    }
//};
//
//class CollisionSystem : public System {
//public:
//    void update() override {
//        auto ballView = scene->r.view<NameComponent, TransformComponent, SizeComponent, VelocityComponent, ColliderComponent>();
//        auto paddleView = scene->r.view<PlayerComponent, TransformComponent, SizeComponent>();
//        auto brickView = scene->r.view<TransformComponent, SizeComponent, BrickComponent>();
//        constexpr float speedUp = 1.05f;
//
//        for (auto ball : ballView) {
//            auto& ballPos = ballView.get<TransformComponent>(ball).position;
//            auto& ballSize = ballView.get<SizeComponent>(ball);
//            auto& ballVel  = ballView.get<VelocityComponent>(ball).velocity;
//            auto& ballCol  = ballView.get<ColliderComponent>(ball);
//
//            // left
//            if (ballPos.x <= 0.0f) {
//                // invert velocity and speed up
//                ballVel.x =  std::abs(ballVel.x) * speedUp;
//                ballPos.x = 0.0f;
//            }
//            // right
//            else if (ballPos.x + ballSize.width >= GetScreenWidth()) {
//                ballVel.x = -std::abs(ballVel.x) * speedUp;
//                ballPos.x = GetScreenWidth() - ballSize.width;
//            }
//
//            // Ceiling collision
//            if (ballPos.y <= 0.0f) {
//                ballVel.y =  std::abs(ballVel.y) * speedUp;
//                ballPos.y = 0.0f;
//            }
//
//            // Floor (lose)
//            if (ballPos.y + ballSize.height >= GetScreenHeight()) {
//                std::cout << "¡You lost!" << std::endl;
//                BeginDrawing();
//                ClearBackground(BLACK);
//                DrawText("¡You lost!",
//                         GetScreenWidth()/2 - MeasureText("¡You lost!", 40)/2,
//                         GetScreenHeight()/2 - 20,
//                         40, RED);
//                EndDrawing();
//
//                WaitTime(2.0f);
//
//                CloseWindow();
//                exit(0);
//            }
//
//            // Paddle collision
//            for (auto paddle : paddleView) {
//                auto& padPos  = paddleView.get<TransformComponent>(paddle).position;
//                auto& padSize = paddleView.get<SizeComponent>(paddle);
//
//                bool overlapX = ballPos.x < padPos.x + padSize.width
//                                && ballPos.x + ballSize.width > padPos.x;
//                bool overlapY = ballPos.y + ballSize.height > padPos.y
//                                && ballPos.y < padPos.y + padSize.height;
//
//                if (overlapX && overlapY) {
//                    if (!ballCol.triggered) {
//                        // increase ball velocity and bounce
//                        ballVel.y = -ballVel.y;
//                        ballVel.y *= 1.1f;
//                        ballVel.x *= 1.1f;
//                        ballCol.triggered = true;
//                    }
//                } else {
//                    ballCol.triggered = false;
//                }
//            }
//
//
//            // Brick collision
//            for (auto brick : brickView) {
//                auto& brickPos = brickView.get<TransformComponent>(brick).position;
//                auto& brickSize = brickView.get<SizeComponent>(brick);
//
//                bool overlapX = ballPos.x < brickPos.x + brickSize.width && ballPos.x + ballSize.width > brickPos.x;
//                bool overlapY = ballPos.y + ballSize.height > brickPos.y && ballPos.y < brickPos.y + brickSize.height;
//
//                if (overlapX && overlapY) {
//                    if (!ballCol.triggered) {
//                        // increase ball velocity and bounce
//                        ballVel.y = -ballVel.y;
//                        ballVel.y *= 1.1f;
//                        ballVel.x *= 1.1f;
//                        ballCol.triggered = true;
//                    }
//                    scene->r.destroy(brick); // destroy the brick
//                }
//            }
//
//            // Check if all bricks are destroyed
//            if (scene->r.view<BrickComponent>().empty()) {
//                std::cout << "¡You won!" << std::endl;
//                BeginDrawing();
//                ClearBackground(BLACK);
//                DrawText("¡You won!",
//                         GetScreenWidth()/2 - MeasureText("¡You won!", 40)/2,
//                         GetScreenHeight()/2 - 20,
//                         40, GREEN);
//                EndDrawing();
//                WaitTime(2.0f);
//                CloseWindow();
//                exit(0);
//            }
//        }
//    }
//};
//
//class BoundsSystem : public System {
//public:
//    void update() override {
//        auto view = scene->r.view<TransformComponent, SizeComponent, PlayerComponent>();
//        for (auto e : view) {
//            auto& pos  = view.get<TransformComponent>(e).position;
//            auto& size = view.get<SizeComponent>(e);
//            pos.x = std::clamp(pos.x,
//                               0.0f,
//                               GetScreenWidth() - size.width);
//        }
//    }
//};
//
//
//class RenderSystem : public System {
//public:
//    void render() override {
//        auto view = scene->r.view<TransformComponent, SizeComponent, NameComponent>();
//        for (auto entity : view) {
//            const auto& pos = view.get<TransformComponent>(entity).position;
//            const auto& size = view.get<SizeComponent>(entity);
//            const auto& name = view.get<NameComponent>(entity).tag;
//            const auto& sprite = scene->r.get<SpriteComponent>(entity);
//
//            DrawRectangle(
//                    static_cast<int>(pos.x),
//                    static_cast<int>(pos.y),
//                    static_cast<int>(size.width),
//                    static_cast<int>(size.height),
//                    sprite.color
//            );
//        }
//    }
//};