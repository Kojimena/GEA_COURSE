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
#include "Game/Components/TilemapData.h"


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

    {
        Entity tilemapEntity = s->createEntity("tilemap", 0, 0);
        auto& tilemap = tilemapEntity.addComponent<TileMapComponent>();
        const int H = static_cast<int>(TILEMAP_MAINMAP.size());
        const int W = static_cast<int>(TILEMAP_MAINMAP.empty() ? 0 : TILEMAP_MAINMAP[0].size());

        tilemap.width = W;
        tilemap.height = H;
        tilemap.tileSize = 16;

        Texture2D grassTexture = TextureManager::loadTexture("/Users/jime/10mo semestre/game_engine/GE_entt_breakout/src/assets/Nightgrass.png");
        Texture2D landTexture = TextureManager::loadTexture("/Users/jime/10mo semestre/game_engine/GE_entt_breakout/src/assets/Land.png");
        Texture2D tressTexture = TextureManager::loadTexture("/Users/jime/10mo semestre/game_engine/GE_entt_breakout/src/assets/trees.png");

        for (int y = 0; y < tilemap.height; y++) {
            for (int x = 0; x < tilemap.width; x++) {
                TileComponent tile;
                tile.x = x;
                tile.y = y;

                switch (TILEMAP_MAINMAP[y][x]) {
                    case 0:
                        tile.type = GRASS;
                        tile.upTexture = grassTexture;
                        tile.downTexture = landTexture;
                        tile.needsAutoTiling = true;
                        break;
                    case 1:
                        tile.type = LAND;
                        tile.upTexture = landTexture;
                        tile.needsAutoTiling = false;
                        break;
                    case 2:
                        tile.type = TREES;
                        tile.upTexture = tressTexture;
                        tile.downTexture = landTexture;
                        tile.needsAutoTiling = true;
                        break;
                }
                tilemap.tiles.push_back(tile);


            }
        }

    }



    makeEnemy(1000, 300, 360, 560);
    makeEnemy(1000, 300, 760, 960);

    s->addSystem(new AutoTilingSetupSystem());
    s->addSystem(new TileMapRenderSystem());
    s->addSystem(new SpriteMovementSystem());
    s->addSystem(new EnemyAISystem());
    s->addSystem(new SpriteSystem());



    return s;
}

inline float f(int x) { return static_cast<float>(x); }