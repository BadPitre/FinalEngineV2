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
}

int main() { return g_madnightEngine.run(); }
