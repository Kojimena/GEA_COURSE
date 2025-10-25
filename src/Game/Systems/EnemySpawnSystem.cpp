#include "EnemySpawnSystem.h"
#include "ECS/Entity.h"
#include "Game/Components/Enemy.h"
#include "Game/Components/Sprites.h"
#include "Game/Components/Player.h"
#include "Game/Components/IntGrid.h"
#include "Game/Components/Tilemap.h"
#include "Game/Components/MovementPattern.h"
#include "raylib.h"
#include <cmath>
#include <cstdint>
#include <random>

// ===================== CONFIG  =====================
static float   spawnInterval = 3.0f;

static int     p1_enemiesCount = 3;
static int     p2_enemiesCount = 5;
static int     p3_enemiesCount = 4;

static float   lineSpacing = 60.0f;

static int     s_lastTx = INT32_MIN;
static int     s_lastTy = INT32_MIN;
static int     s_lastTileValue = -9999;

static const char* p1_sprite = "../src/assets/snake.png";
static const char* p2_sprite = "../src/assets/bat.png";
static const char* p3_sprite = "../src/assets/beatle.png";

void EnemySpawnSystem::setup() {
    spawnTimer = 0.0f;
    spawnInterval = spawnInterval;
}

static inline void SpawnTrackingEnemy(Scene* scene, float x, float y, const char* spritePath) {
    Entity enemy = scene->createEntity("enemy_tracking", x, y);

    enemy.addComponent<SpriteLayerComponent>(
            spritePath, 16, 16, 2, 8, 600, 0, 0, 0, 0
    );

    auto& pattern = enemy.addComponent<MovementPatternComponent>();
    pattern.scriptPath = "../src/assets/scripts/tracking_pattern.lua";
    pattern.speed = 70.0f;
    pattern.trackingDistance = 350.0f;
}

static inline void SpawnCircularEnemy(Scene* scene, Vector2 center, float radius, bool aroundPlayer) {
    Entity enemy = scene->createEntity("enemy_circular", center.x, center.y);

    enemy.addComponent<SpriteLayerComponent>(
            p2_sprite, 16, 16, 2, 8, 600, 0, 0, 0, 0
    );

    auto& pattern = enemy.addComponent<MovementPatternComponent>();
    pattern.scriptPath = "../src/assets/scripts/circular_pattern.lua";
    pattern.orbitRadius = radius;
    pattern.orbitSpeed = 1.5f;
    pattern.orbitAroundPlayer = aroundPlayer;
    pattern.orbitCenterX = center.x;
    pattern.orbitCenterY = center.y;
    pattern.attackSpeed = 60.0f;
    pattern.attackInterval = 3.0f;
}

static inline void SpawnPatrolEnemy(Scene* scene, float x, float y,
                                    const std::vector<std::pair<float, float>>& waypoints) {
    Entity enemy = scene->createEntity("enemy_patrol", x, y);

    enemy.addComponent<SpriteLayerComponent>(
            p3_sprite, 16, 16, 2, 8, 600, 0, 0, 0, 0
    );

    auto& pattern = enemy.addComponent<MovementPatternComponent>();
    pattern.scriptPath = "../src/assets/scripts/patrol_pattern.lua";
    pattern.patrolSpeed = 50.0f;
    pattern.waypoints = waypoints;
}



static inline void SpawnCircleAround(Scene* scene, Vector2 center) {
    int n = (p2_enemiesCount <= 0) ? 1 : p2_enemiesCount;
    float angleStep = 2.0f * 3.14159265f / (float)n;
    float radius = lineSpacing;

    for (int i = 0; i < n; ++i) {
        float angle = i * angleStep;
        float dx = cosf(angle) * radius;
        float dy = sinf(angle) * radius;

        SpawnCircularEnemy(scene, center, 60.0f, true);
    }
}

static inline void SpawnLineAround(Scene* scene, Vector2 center) {
    int n = (p1_enemiesCount <= 0) ? 1 : p1_enemiesCount;
    float total = (n - 1) * lineSpacing;
    float start = -total * 0.5f;

    for (int i = 0; i < n; ++i) {
        float dx = start + i * lineSpacing;

        SpawnTrackingEnemy(scene, center.x + dx, center.y, p1_sprite);
    }
}

static inline void SpawnRandomAround(Scene* scene, Vector2 center) {
    int n = (p3_enemiesCount <= 0) ? 1 : p3_enemiesCount;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-150.0, 150.0);

    for (int i = 0; i < n; ++i) {
        float spawnX = center.x + dis(gen);
        float spawnY = center.y + dis(gen);

        std::vector<std::pair<float, float>> waypoints;
        for (int w = 0; w < 4; ++w) {
            waypoints.push_back({
                                        spawnX + dis(gen),
                                        spawnY + dis(gen)
                                });
        }

        SpawnPatrolEnemy(scene, spawnX, spawnY, waypoints);
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
        playerPos = {tf.position.x, tf.position.y};
        hasPlayer = true;
        break;
    }
    if (!hasPlayer) return;

    const TileMapComponent* map = nullptr;
    const IntGridComponent* grid = nullptr;
    Vector2 mapOrigin = {0, 0};

    auto tmView = scene->r.view<TileMapComponent, IntGridComponent, TransformComponent>();
    for (auto ent : tmView) {
        map = &tmView.get<TileMapComponent>(ent);
        grid = &tmView.get<IntGridComponent>(ent);
        const auto& mtf = tmView.get<TransformComponent>(ent);
        mapOrigin = {mtf.position.x, mtf.position.y};
        break;
    }

    int tileValue = -1;
    int tx = INT32_MIN, ty = INT32_MIN;

    if (map && grid && grid->width > 0 && grid->height > 0 &&
        (int)grid->grid.size() >= grid->width * grid->height) {
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

    // Spawn según tile
    if (tx != s_lastTx || ty != s_lastTy || tileValue != s_lastTileValue) {
        s_lastTx = tx; s_lastTy = ty; s_lastTileValue = tileValue;

        switch (tileValue) {
            case 2: { // PLANTA VENENOSA- LINEA - PATRULLA
                SpawnLineAround(scene, playerPos);
            } break;
            case 3: { // PORTAL - CIRCULO-  ORBITAN
                SpawnCircleAround(scene, playerPos);
            } break;
            case 4: { // TIERRA - RANDOM- PATRULLA CON WAYPOINTS
                if (spawnTimer >= spawnInterval) {
                    spawnTimer = 0.0f;
                    SpawnRandomAround(scene, playerPos);
                }
            } break;
            default:
                break;
        }
    }
}