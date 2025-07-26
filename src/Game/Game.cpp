#include <raylib.h>
#include <iostream>
#include "Game.h"
#include <vector>


Rectangle ball;
Rectangle paddle;
std::vector<Rectangle> blocks;
std::vector<Color> blockColors = {
         LIME , GREEN, BLUE, SKYBLUE, PINK, ORANGE
};
std::vector<Color> colors_asigned;

int blocks_quantity = 0;
int ball_sx = 150;
int ball_sy = 150;
int paddle_sx = 200;

Game::Game(const char* title, int width, int height)
        : screen_width(width), screen_height(height) {
    InitWindow(width, height, title);
    std::cout << "Game started" << std::endl;
    isRunning = true;
    counter = 0;
}

Game::~Game() {

}

void Game::setup() {
    SetTargetFPS(60);

    ball = Rectangle{10, 100, 15, 15};
    paddle = Rectangle{(float)(screen_width/2) - ball.width*5, (float)screen_height - 15, ball.width*10, 15};
    int rows = 5;
    int cols = 8;
    float blockWidth = screen_width / cols;
    float blockHeight = 20;

    blocks.clear();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Rectangle b = {
                    j * blockWidth,
                    i * blockHeight,
                    blockWidth - 2,
                    blockHeight - 2
            };
            blocks.push_back(b);

            Color c = blockColors[i % blockColors.size()];
            colors_asigned.push_back(c);
        }
    }

    blocks_quantity = blocks.size();
}

void Game::frame_start() {
    std::cout << "Frame " << counter << " started" << std::endl;
    BeginDrawing();
}

void Game::frame_end() {
    std::cout << "Frame " << counter << " ended" << std::endl;
    EndDrawing();
    counter++;
}

void Game::handle_events() {
    float dT = GetFrameTime();  // seconds
    if (WindowShouldClose()) {
        isRunning = false;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        paddle.x += paddle_sx * dT;
        if (paddle.x + paddle.width > screen_width) {
            paddle.x = screen_width - paddle.width;
        }
    }

    if (IsKeyDown(KEY_LEFT)) {
        paddle.x -= paddle_sx * dT;
        if (paddle.x < 0) {
            paddle.x = 0;
        }
    }
}

void Game::update() {
    float dT = GetFrameTime();
    if (ball.x >= screen_width) {
        ball_sx *= -1;
    }
    if (ball.x < 0) {
        ball_sx *= -1;
    }
    if (ball.y >= screen_height) {
        std::cout << "Game over:(" << std::endl;
        exit(1);
    }
    if (CheckCollisionRecs(ball, paddle)) {
        float hitPos = (ball.x + ball.width / 2.0f) - (paddle.x + paddle.width / 2.0f);
        float normalized = hitPos / (paddle.width / 2.0f);
        ball_sx = normalized * 200;
        ball_sy *= -1;
    }
    if (ball.y < 0) {
        ball_sy *= -1;
    }

    ball.x += ball_sx * dT;
    ball.y += ball_sy * dT;

    // Collision with blocks
    for (size_t i = 0; i < blocks.size(); i++) {
        if (CheckCollisionRecs(ball, blocks[i])) {
            ball_sy *= -1;
            blocks.erase(blocks.begin() + i);
            colors_asigned.erase(colors_asigned.begin() + i);
            blocks_quantity--;

            break;
        }
    }

    if (blocks_quantity <= 0) {
        std::cout << "You win!!!" << std::endl;
        exit(0);
    }

    ball.x += ball_sx * dT;
    ball.y += ball_sy * dT;

}

void Game::render() {
    ClearBackground(WHITE);

    DrawRectangleRec(ball, BLACK);
    DrawRectangleRec(paddle, BLACK);
    for (size_t i = 0; i < blocks.size(); i++) {
        DrawRectangleRec(blocks[i], colors_asigned[i]);
    }
    DrawText(TextFormat("Blocks left: %d", blocks_quantity), screen_width - 200, screen_height - 80, 20, GRAY);
}

void Game::clean() {
    CloseWindow();
}

bool Game::running() {
    return isRunning;
}