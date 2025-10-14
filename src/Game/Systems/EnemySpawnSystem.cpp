#include "EnemySpawnSystem.h"
#include "ECS/Entity.h"
#include "Game/Components/Enemy.h"
#include "Game/Components/Sprites.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Player.h"
#include "Game/Components/IntGrid.h"
#include "Game/Components/Tilemap.h"
#include "raylib.h"
#include <cmath>
#include <cstdint>
#include <random>

// ===================== CONFIG  =====================
static float   spawnInterval = 2.0f;  // timer

static int     p1_enemiesCount = 4;     // cant enemigos línea
static int    p2_enemiesCount = 8;     // cant enemigos círculo
static int     p3_enemiesCount = 6;    // cant enemigos random

static float   lineSpacing     = 36.0f;    // separación px

static int     s_lastTx          = INT32_MIN;
static int     s_lastTy          = INT32_MIN;
static int     s_lastTileValue   = -9999;

static const char* p1_sprite = "../src/assets/snake.png";      // sprite línea
static const char* p2_sprite = "../src/assets/bat.png";    // sprite círculo
static const char* p3_sprite = "../src/assets/beatle.png";    // sprite aleatorio


void EnemySpawnSystem::setup() {
    spawnTimer    = 0.0f;
    spawnInterval = spawnInterval; // usa la config de arriba
}

static inline void SpawnEnemyAt(Scene* scene, float x, float y, const char* spritePath, float speed) {
    Entity enemy = scene->createEntity("enemy", x, y);

    enemy.addComponent<SpriteLayerComponent>(
            spritePath,
            16, 16,
            2,
            8,
            600,
            0, 0, 0, 0
    );

    enemy.addComponent<EnemyAIComponent>(EnemyAIComponent{
            .speed = speed, .leftX = x - 40.0f, .rightX = x + 40.0f, .dir = 1
    });
}

static inline void SpawnCircleAround(Scene* scene, Vector2 center) {
    int   n      = (p2_enemiesCount <= 0) ? 1 : p2_enemiesCount;
    float angleStep = 2.0f * 3.14159265f / (float)n;
    float radius    = lineSpacing; // radio del círculo

    for (int i = 0; i < n; ++i) {
        float angle = i * angleStep;
        float dx = cosf(angle) * radius;
        float dy = sinf(angle) * radius;
        SpawnEnemyAt(scene, center.x + dx, center.y + dy, p2_sprite, 30.0f);
    }
}

static inline void SpawnLineAround(Scene* scene, Vector2 center) {
    int   n      = (p1_enemiesCount <= 0) ? 1 : p1_enemiesCount;
    float total  = (n - 1) * lineSpacing;
    float start  = -total * 0.5f;

    for (int i = 0; i < n; ++i) {
        float dx = start + i * lineSpacing;
        float dy =  0.0f;
        SpawnEnemyAt(scene, center.x + dx, center.y + dy, p1_sprite, 50.0f);
    }
}

static inline void SpawnRandomAround(Scene* scene, Vector2 center) {
    int n = (p3_enemiesCount <= 0) ? 1 : p3_enemiesCount;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-200.0, 200.0);

    for (int i = 0; i < n; ++i) {
        float spawnX = center.x + dis(gen);
        float spawnY = center.y + dis(gen);
        SpawnEnemyAt(scene, spawnX, spawnY, p3_sprite, 40.0f);
    }
}

void EnemySpawnSystem::update() {
    float dt = GetFrameTime();
    spawnTimer += dt;

    Vector2 playerPos = {0, 0};
    bool hasPlayer = false;

    auto pview = scene->r.view<PlayerComponent, TransformComponent>();
    for (auto e : pview) {
        const auto& tf = pview.get<TransformComponent>(e);
        playerPos = { tf.position.x, tf.position.y };
        hasPlayer = true;
        break;
    }
    if (!hasPlayer) return;

    const TileMapComponent* map  = nullptr;
    const IntGridComponent* grid = nullptr;
    Vector2 mapOrigin = {0, 0};

    auto tmView = scene->r.view<TileMapComponent, IntGridComponent, TransformComponent>();
    for (auto ent : tmView) {
        map  = &tmView.get<TileMapComponent>(ent);
        grid = &tmView.get<IntGridComponent>(ent);
        const auto& mtf = tmView.get<TransformComponent>(ent);
        mapOrigin = { mtf.position.x, mtf.position.y };
        break;
    }

    int tileValue = -1;
    int tx = INT32_MIN, ty = INT32_MIN;

    if (map && grid && grid->width > 0 && grid->height > 0 &&
        (int)grid->grid.size() >= grid->width * grid->height)
    {
        float tileScalePx = map->tiles.empty()
                            ? (float)map->tileSize
                            : map->tiles[0].scale * (float)map->tileSize;
        if (tileScalePx <= 0.0f) tileScalePx = (float)map->tileSize;

        tx = (int)floorf((playerPos.x - mapOrigin.x) / tileScalePx);
        ty = (int)floorf((playerPos.y - mapOrigin.y) / tileScalePx);

        if (tx >= 0 && ty >= 0 && tx < grid->width && ty < grid->height) {
            tileValue = grid->grid[ty * grid->width + tx];
        }
    }

    // Patron según tile
    if (tx != s_lastTx || ty != s_lastTy || tileValue != s_lastTileValue) {
        s_lastTx = tx; s_lastTy = ty; s_lastTileValue = tileValue;

        switch (tileValue) {
            case 2: { // PLANTA VENENOSA- LÍNEA
                SpawnLineAround(scene, playerPos);
            } break;
            case 3: { // PORTAL -CÍRCULO
                SpawnCircleAround(scene, playerPos);
            } break;
            case 4: { // TIERRA - RANDOM
                if (spawnTimer >= spawnInterval) {
                    spawnTimer = 0.0f;
                    SpawnRandomAround(scene, playerPos);
                }
            } break;
            default:
                // otros tiles
                break;
        }
    }

}