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
        {.name = "MODELS/STREET.MB",   .type = LoadFileType::OBJECT},
        {.name = "MODELS/CUBE.MB",     .type = LoadFileType::OBJECT},
    };

    co_await g_madnightEngine.HardLoadingScreen(eastl::move(queue), &gameplayScene);

    auto* street = GameObjectManager::CreateGameObject(
        "STREET", {0, 0, 0}, {0, 0, 0}, GameObjectTag::ENVIRONMENT);
    if (street) {
        street->SetQuadType(GameObjectQuadType::GouraudTextureQuad);
        street->SetMesh("MODELS/STREET.MB");
        street->SetTexture("TEXTURES/STREET.TIM");
    }

    auto* cube = GameObjectManager::CreateGameObject(
        "CUBE", {.x = 0, .y = 0, .z = 0.5_fp}, {0, 0, 0}, GameObjectTag::ENVIRONMENT);
    if (cube) {
        cube->SetQuadType(GameObjectQuadType::GouraudQuad);
        cube->SetMesh("MODELS/CUBE.MB");
    }
}

int main() { return g_madnightEngine.run(); }
