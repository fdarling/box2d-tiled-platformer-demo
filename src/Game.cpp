#include "Game.h"
#include "PhysicsDebugDrawer.h"
#include "globals.h"

Game::Game() : bWorld(b2Vec2(0.0, -9.8)), level(&bWorld), player(&bWorld), window(nullptr, &SDL_DestroyWindow), renderer(nullptr, &SDL_DestroyRenderer)
{
    bWorld.SetContactListener(&contactListener);
}

Game::~Game()
{
    deinit();
}

bool Game::init(int w, int h)
{
    deinit();

    // create window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    window = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(SDL_CreateWindow(
        "Box2D Tiled Platformer Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        0
    ), &SDL_DestroyWindow);
    if (!window)
    {
        fprintf(stderr, "error: SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    // create renderer
    renderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC), &SDL_DestroyRenderer);
    if (!renderer)
    {
        fprintf(stderr, "error: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }
    SDL_RenderSetScale(renderer.get(), ZOOM_FACTOR, ZOOM_FACTOR);

    physicsDebugDrawer = std::make_unique<PhysicsDebugDrawer>(renderer.get());
    {
        // show graphical debugging
        uint32 flags = 0;
        flags |= b2Draw::e_shapeBit;
        flags |= b2Draw::e_jointBit;
        // flags |= b2Draw::e_aabbBit;
        flags |= b2Draw::e_pairBit;
        flags |= b2Draw::e_centerOfMassBit;
        physicsDebugDrawer->SetFlags(flags);
    }
    bWorld.SetDebugDraw(physicsDebugDrawer.get());

    return true;
}

void Game::deinit()
{
    bWorld.SetDebugDraw(nullptr);
    physicsDebugDrawer.reset();
    renderer.reset();
    window.reset();
}

bool Game::exec()
{
    // avoid segfaults
    if (!window || !renderer)
        return false;

    // note the initial time for time delta calculations
    Uint64 oldTime = SDL_GetTicks64();

    // loop until an event says we shouldn't continue
    while (true)
    {
        // clear screen
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer.get());

        // debug draw Box2D
        bWorld.DebugDraw();

        // commit the drawing (flip buffers, ideally with vsync)
        SDL_RenderPresent(renderer.get());

        // process events, possibly quit
        if (!processEvents())
            break;

        // advance game state
        {
            const Uint64 newTime = SDL_GetTicks64();
            const Uint64 timeDeltaMs = newTime - oldTime;
            oldTime = newTime;
            const float timeDeltaS = static_cast<float>(timeDeltaMs)/1000.0f; // convert from milliseconds to decimal seconds
            advance(timeDeltaS);
        }

        // yield CPU
        SDL_Delay(1);
    }

    return true;
}

bool Game::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false; // signal that we want to quit the game
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                return false; // signal that we want to quit the game

                default:
                break;
            }
        }
    }
    return true; // signal that we want to continue running the game
}

void Game::advance(float timeStep)
{
    const Uint8 * const keys = SDL_GetKeyboardState(NULL);
    const bool left_held = keys[SDL_SCANCODE_LEFT];
    const bool right_held = keys[SDL_SCANCODE_RIGHT];
    const bool up_held = keys[SDL_SCANCODE_UP];
    float walk_dir = 0.0;
    if ( left_held && !right_held)
        walk_dir = -1.0;
    if (!left_held &&  right_held)
        walk_dir =  1.0;

    if (walk_dir)
        player.setWalkingDir(walk_dir);
    if (up_held)
        player.jump();

    static const int32 VELOCITY_ITERATIONS = 6;
    static const int32 POSITION_ITERATIONS = 2;
    bWorld.Step(timeStep, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}
