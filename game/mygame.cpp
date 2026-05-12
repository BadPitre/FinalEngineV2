#include "mygame.hh"
#include "cube_scene.hh"
#include "madnight.hh"
#include "game.hh"
#include "helpers/load_queue.hh"

#include <EASTL/vector.h>
#include "psyqo/xprintf.h"

MadnightGame g_myGame;
MadnightEngineGame &g_madnightEngineGame = g_myGame;
static CubeScene cubeScene;

psyqo::Coroutine<> MadnightGame::InitialLoad(void)
{
    printf("welcome to your game code!\n");

    eastl::vector<LoadQueue> queue = {
        {.name = "TEXTURES/STREET.TIM", .type = LoadFileType::TEXTURE,
         .x = 320, .y = 0, .clutX = 0, .clutY = 240},
        {.name = "MODELS/STREET.MB",   .type = LoadFileType::OBJECT},
        {.name = "MODELS/CUBE.MB",     .type = LoadFileType::OBJECT},
    };

    co_await g_madnightEngine.HardLoadingScreen(eastl::move(queue), &cubeScene);
}

int main() { return g_madnightEngine.run(); }
