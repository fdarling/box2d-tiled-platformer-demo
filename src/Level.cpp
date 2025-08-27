#include "Level.h"
#include "globals.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

// #define USE_POLYGON_INSTEAD_OF_EDGES

#ifdef USE_POLYGON_INSTEAD_OF_EDGES
#include <box2d/b2_polygon_shape.h>
#else // USE_POLYGON_INSTEAD_OF_EDGES
#include <box2d/b2_edge_shape.h>
#endif // USE_POLYGON_INSTEAD_OF_EDGES

#include <vector> // for std::vector<>

Level::Level(b2World *bWorld) : world(bWorld), tilesBody(nullptr)
{
    b2BodyDef groundBodyDef;
    // groundBodyDef.type = b2_staticBody;
    tilesBody = world->CreateBody(&groundBodyDef);

    for (int col = 3; col <= 7; col++)
        addTile(5, col);
    for (int col = 8; col <= 14; col++)
        addTile(4, col);
    for (int col = 15; col <= 20; col++)
        addTile(5, col);
    addTile(6, 10);
    addTile(6, 11);
    addTile(7, 12);
    addTile(8, 13);
}

Level::~Level()
{
    world->DestroyBody(tilesBody);
}

void Level::addTile(int row, int col)
{
    // calculate screen space tile vertices
    std::vector<b2Vec2> points;
    points.emplace_back( col   *TILE_WIDTH,  row   *TILE_HEIGHT); // top left
    points.emplace_back((col+1)*TILE_WIDTH,  row   *TILE_HEIGHT); // top right
    points.emplace_back((col+1)*TILE_WIDTH, (row+1)*TILE_HEIGHT); // bottom right
    points.emplace_back( col   *TILE_WIDTH, (row+1)*TILE_HEIGHT); // bottom left

    // convert from pixels to meters
    for (std::size_t i = 0; i < points.size(); i++)
    {
        points[i].x *= METERS_PER_PIXEL;
        points[i].y *= METERS_PER_PIXEL;
    }
    

#ifdef USE_POLYGON_INSTEAD_OF_EDGES
    b2PolygonShape shape;
    shape.Set(points.data(), points.size());
    tilesBody->CreateFixture(&shape, 0.0);
#else // USE_POLYGON_INSTEAD_OF_EDGES
    b2EdgeShape shape;
    const size_t len = points.size();
    for (std::size_t i = 0; i < len; i++)
    {
        const size_t idx0 = (i+len-1)%len;
        const size_t idx1 = (i+len  )%len;
        const size_t idx2 = (i+len+1)%len;
        const size_t idx3 = (i+len+2)%len;
        shape.SetOneSided(points[idx0], points[idx1], points[idx2], points[idx3]);
        tilesBody->CreateFixture(&shape, 0.0);
    }
#endif // USE_POLYGON_INSTEAD_OF_EDGES
}
