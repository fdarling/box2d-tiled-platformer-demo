#pragma once

#include "PhysicsContactListener.h"
#include "Level.h"
#include "Player.h"

#include <SDL.h> // for SDL_Window and SDL_Renderer

#include <box2d/b2_world.h> // for b2World

#include <memory> // for std::unique_ptr<>

class PhysicsDebugDrawer; // forward declaration

class Game
{
public:
    Game();
    ~Game();
    bool init(int w, int h);
    bool exec();
protected:
    void deinit();
    bool processEvents();
    void advance(float timeStep);

    PhysicsContactListener contactListener;
    b2World bWorld;
    Level level;
    Player player;
    std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> window;
    std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> renderer;
    std::unique_ptr<PhysicsDebugDrawer> physicsDebugDrawer;
};
