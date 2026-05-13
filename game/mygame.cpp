#include "mygame.hh"
#include "madnight.hh"
#include "scenes/gameplay.hh"
#include "core/object/gameobject_manager.hh"
#include "helpers/load_queue.hh"

#include <EASTL/vector.h>
#include "psyqo/xprintf.h"

using namespace psyqo::fixed_point_literals;
using namespace psyqo::trig_literals;

MadnightGame g_myGame;
MadnightEngineGame &g_madnightEngineGame = g_myGame;
static GameplayScene gameplayScene;

psyqo::Coroutine<> MadnightGame::InitialLoad(void)
{
    printf("welcome to your game code!\n");

    eastl::vector<LoadQueue> queue = {
        {.name = "MODELS/SUZANNE.MB", .type = LoadFileType::OBJECT},
        {.name = "MODELS/ROOM.MB",    .type = LoadFileType::OBJECT},
        {.name = "TEXTURES/ROOM.TIM", .type = LoadFileType::TEXTURE,
         .x = 320, .y = 0, .clutX = 0, .clutY = 240},
    };

    co_await g_madnightEngine.HardLoadingScreen(eastl::move(queue), &gameplayScene);

    struct SpawnSpot { const char *name; psyqo::Vec3 pos; };
    const SpawnSpot spawns[] = {
        {"SUZANNE_NE", {.x =  0.8_fp, .y = 0, .z =  2.3_fp}},
        {"SUZANNE_NW", {.x = -0.8_fp, .y = 0, .z =  2.3_fp}},
        {"SUZANNE_SE", {.x =  0.8_fp, .y = 0, .z =  0.7_fp}},
        {"SUZANNE_SW", {.x = -0.8_fp, .y = 0, .z =  0.7_fp}},
    };

    for (const auto &spot : spawns) {
        auto *obj = GameObjectManager::CreateGameObject(
            spot.name, spot.pos, {0, 0, 0}, GameObjectTag::ENVIRONMENT);
        if (obj) {
            obj->SetQuadType(GameObjectQuadType::GouraudQuad);
            obj->SetMesh("MODELS/SUZANNE.MB");
        }
    }

    auto *room = GameObjectManager::CreateGameObject(
        "ROOM", {.x = 0, .y = 0, .z = 1.5_fp}, {0, 0, 0}, GameObjectTag::ENVIRONMENT);
    if (room) {
        room->SetQuadType(GameObjectQuadType::GouraudTextureQuad);
        room->SetMesh("MODELS/ROOM.MB");
        room->SetTexture("TEXTURES/ROOM.TIM");
    }
}

int main() { return g_madnightEngine.run(); }
