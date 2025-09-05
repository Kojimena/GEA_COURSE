#include "CoffeeSurvivors.h"
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


CoffeeSurvivors::CoffeeSurvivors() : Game("CoffeeSurvivors", SCREEN_WIDTH, SCREEN_HEIGHT) {
    Scene* gameplayScene = createSpriteScene();
    setScene(gameplayScene);
}

CoffeeSurvivors::~CoffeeSurvivors() {
}

Scene* CoffeeSurvivors::createSpriteScene() {


    Scene* s = new Scene("Sprite");
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

    {
        Entity tilemapEntity = s->createEntity("tilemap", 0, 0);
        auto& tilemap = tilemapEntity.addComponent<TileMapComponent>();
        const int H = static_cast<int>(TILEMAP_MAINMAP.size());
        const int W = static_cast<int>(TILEMAP_MAINMAP.empty() ? 0 : TILEMAP_MAINMAP[0].size());

        tilemap.width = W;
        tilemap.height = H;
        tilemap.tileSize = 16;

        Texture2D grassTexture = TextureManager::loadTexture("/Users/jime/10mo semestre/game_engine/GE_entt_breakout/src/assets/CoffeeTile.png");
        Texture2D waterTexture = TextureManager::loadTexture("/Users/jime/10mo semestre/game_engine/GE_entt_breakout/src/assets/MilkTile.png");

        for (int y = 0; y < tilemap.height; y++) {
            for (int x = 0; x < tilemap.width; x++) {
                TileComponent tile;
                tile.x = x;
                tile.y = y;

                switch (TILEMAP_MAINMAP[y][x]) {
                    case 0:
                        tile.type = COFFEE;
                        tile.upTexture = grassTexture;
                        tile.downTexture = waterTexture;
                        tile.needsAutoTiling = true;
                        break;
                    case 1:
                        tile.type = MILK;
                        tile.upTexture = waterTexture;
                        tile.needsAutoTiling = false;
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