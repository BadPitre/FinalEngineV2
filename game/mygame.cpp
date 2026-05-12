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
        {.name = "TEXTURES/STREET.TIM", .type = LoadFileType::TEXTURE,
         .x = 320, .y = 0, .clutX = 0, .clutY = 240},
        {.name = "MODELS/SUZANNE.MB",  .type = LoadFileType::OBJECT},
    };

    co_await g_madnightEngine.HardLoadingScreen(eastl::move(queue), &gameplayScene);

    auto* obj = GameObjectManager::CreateGameObject(
        "SUZANNE", {.x = 0, .y = 0, .z = 1.5_fp}, {0, 0, 0}, GameObjectTag::ENVIRONMENT);
    if (obj) {
        obj->SetQuadType(GameObjectQuadType::GouraudTextureQuad);
        obj->SetMesh("MODELS/SUZANNE.MB");
        obj->SetTexture("TEXTURES/STREET.TIM");
    }
}

int main() { return g_madnightEngine.run(); }
