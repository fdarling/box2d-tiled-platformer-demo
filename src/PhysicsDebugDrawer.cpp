#include "PhysicsDebugDrawer.h"
#include "globals.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include <vector>
// #include <iostream>

// for future camera panning
#define CAMERA_OFFSET_X (0)
#define CAMERA_OFFSET_Y (-600)
#define Y_SCALAR (-1)

PhysicsDebugDrawer::PhysicsDebugDrawer(SDL_Renderer *theRenderer) : renderer(theRenderer)
{
}

PhysicsDebugDrawer::~PhysicsDebugDrawer() = default;

void PhysicsDebugDrawer::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    std::vector<SDL_Point> points;
    points.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        points.push_back(SDL_Point{
            static_cast<int>(vertices[i].x*PIXELS_PER_METER) - CAMERA_OFFSET_X,
            static_cast<int>(vertices[i].y*PIXELS_PER_METER)*Y_SCALAR - CAMERA_OFFSET_Y
        });
    }
    SDL_SetRenderDrawColor(renderer, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    SDL_RenderDrawLines(renderer, points.data(), vertexCount);
    // std::cout << "DrawPolygon" << std::endl;
}

void PhysicsDebugDrawer::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    std::vector<Sint16> vx;
    std::vector<Sint16> vy;
    vx.reserve(vertexCount);
    vy.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        vx.push_back(static_cast<Sint16>(vertices[i].x*PIXELS_PER_METER - CAMERA_OFFSET_X));
        vy.push_back(static_cast<Sint16>(vertices[i].y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y));
    }
    // filledPolygonRGBA(renderer, vx.data(), vy.data(), vertexCount, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    filledPolygonRGBA(renderer, vx.data(), vy.data(), vertexCount, color.r*255.0, color.g*255.0, color.b*255.0, 128);
    // std::cout << "DrawSolidPolygon " << vertices[0].x << ", " << vertices[0].y << std::endl;
}

void PhysicsDebugDrawer::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
{
    const int x = center.x*PIXELS_PER_METER - CAMERA_OFFSET_X;
    const int y = center.y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y;
    const int rad = radius*PIXELS_PER_METER;
    // static const int TOL = 1;
    // if (!SDL_HasIntersection(&SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, &SDL_Rect{x - rad - TOL, y - rad - TOL, rad*2 + TOL*2, rad*2 + TOL*2}))
        // return;
    circleRGBA(renderer, x, y, rad, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    // std::cout << "DrawCircle" << std::endl;
}

void PhysicsDebugDrawer::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
{
    filledCircleRGBA(renderer, center.x*PIXELS_PER_METER - CAMERA_OFFSET_X, center.y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y, radius*PIXELS_PER_METER, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, color.a*255.0);
    SDL_RenderDrawLine(renderer, center.x*PIXELS_PER_METER - CAMERA_OFFSET_X, center.y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y, (center.x + axis.x*radius)*PIXELS_PER_METER - CAMERA_OFFSET_X, (center.y + axis.y*radius)*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y);
    // std::cout << "DrawSolidCircle" << std::endl;
}

void PhysicsDebugDrawer::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    const int x1 = p1.x*PIXELS_PER_METER - CAMERA_OFFSET_X;
    const int y1 = p1.y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y;
    const int x2 = p2.x*PIXELS_PER_METER - CAMERA_OFFSET_X;
    const int y2 = p2.y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    // std::cout << "DrawSegment" << std::endl;
    // std::cout << "DrawSegment (" << x1 << "," << y1 << " -> " << x2 << "," << y2 << ") color = " << color.r << "," << color.g << "," << color.b << std::endl;
}

void PhysicsDebugDrawer::DrawTransform(const b2Transform &xf)
{
    (void)xf;
    // std::cout << "DrawTransform" << std::endl;
}

void PhysicsDebugDrawer::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
    filledCircleRGBA(renderer, p.x*PIXELS_PER_METER - CAMERA_OFFSET_X, p.y*PIXELS_PER_METER*Y_SCALAR - CAMERA_OFFSET_Y, size/2.0, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // HACK, the above function messes with this renderer state :-(
    // std::cout << "DrawPoint" << std::endl;
}
