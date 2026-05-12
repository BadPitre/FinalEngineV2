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

    auto* obj = GameObjectManager::CreateGameObject(
        "SUZANNE", {.x = 0, .y = 0, .z = 0.5_fp}, {0, 1.0_pi, 0}, GameObjectTag::ENVIRONMENT);
    if (obj) {
        obj->SetQuadType(GameObjectQuadType::GouraudQuad);
        obj->SetMesh("MODELS/SUZANNE.MB");
    }
}

int main() { return g_madnightEngine.run(); }
