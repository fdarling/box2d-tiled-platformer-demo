#include "Game.h"

#include <SDL2/SDL.h>

#include <box2d/b2_body.h>

#include <iostream> // for std::cout and std::endl
#include <cstdio> // for EXIT_SUCCESS and EXIT_FAILURE

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

int main(int argc, char **argv)
{
    // we don't use command line arguments
    (void)argc;
    (void)argv;

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "error: SDL_Init failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // main game loop
    Game game;
    if (!game.init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        SDL_Quit();
        return EXIT_FAILURE;
    }
    const int returnValue = game.exec() ? EXIT_SUCCESS : EXIT_FAILURE;

    // shutdown SDL
    SDL_Quit();

    // pass on return code
    return returnValue;
}
