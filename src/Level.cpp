#include "Level.h"
#include "PhysicsUtilities.h"
#include "globals.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_contact.h>

// #define USE_POLYGON_INSTEAD_OF_EDGES

#ifdef USE_POLYGON_INSTEAD_OF_EDGES
#include <box2d/b2_polygon_shape.h>
#else // USE_POLYGON_INSTEAD_OF_EDGES
#include <box2d/b2_edge_shape.h>
#endif // USE_POLYGON_INSTEAD_OF_EDGES

#include <vector> // for std::vector<>
// #include <iostream>

Level::Level(b2World *bWorld) : world(bWorld), tilesBody(nullptr)
{
    b2BodyDef groundBodyDef;
    // groundBodyDef.type = b2_staticBody;
    tilesBody = world->CreateBody(&groundBodyDef);
    tilesBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    for (int col = 3; col <= 7; col++)
        addTile(5, col);
    for (int col = 8; col <= 14; col++)
        addTile(4, col);
    for (int col = 15; col <= 20; col++)
        addTile(5, col);
    addTile(6, 10, true);
    addTile(6, 11, true);
    addTile(7, 11, true);
    addTile(7, 12, true);
    addTile(8, 13, true);

    addTile(8, 6);
    addTile(8, 7, true);
    addTile(8, 8, true);
}

Level::~Level()
{
    world->DestroyBody(tilesBody);
}

bool Level::fixtureIsOneWay(const b2Fixture *fixture) const
{
    // determine the tile coordinates from the fixture pointer
    b2Fixture * const fix = const_cast<b2Fixture *>(fixture);
    std::unordered_map<b2Fixture*, std::pair<int, int> >::const_iterator it = fixtureToTileMap.find(fix);
    if (it == fixtureToTileMap.end())
        return false;

    // determine if we have a tile at that coordinate
    std::unordered_map<std::pair<int, int>, bool, coord_pair_hash>::const_iterator it2 = tiles.find(it->second);
    if (it2 == tiles.end())
        return false;

    // return the stored boolean that indicates one-way tiles
    return it2->second;
}

void Level::preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)oldManifold;

    // bail early if the tile isn't one-way (default to normal collision behavior)
    if (!fixtureIsOneWay(ourFixture))
        return;

    // if we get this far, we might need a one way collision override!
    b2WorldManifold contactWorldManifold;
    contact->GetWorldManifold(&contactWorldManifold);

    // calculate the two bounding boxes
    const b2AABB tile_aabb = get_AABB_for_fixture(ourFixture);
    const b2AABB other_aabb = get_AABB_for_fixture(otherFixture);
    const bool isAbove = other_aabb.lowerBound.y + OVERLAP_TOLERANCE > tile_aabb.upperBound.y;
    // std::cout << "Level::preSolve(): normal.y: " << contactWorldManifold.normal.y << "; tile_aabb.upperBound.y: " << tile_aabb.upperBound.y << "; other_aabb.lowerBound.y: " << other_aabb.lowerBound.y << "; isAbove: " << isAbove << std::endl;

    // disable collision if the other object is not completely above us (minus the overlap tolerance)
    if (!isAbove)
    {
        contact->SetEnabled(false);
    }
}

void Level::addTile(int row, int col, bool oneWay)
{
    const std::pair<int, int> coord_pair(row, col);

    // calculate screen space tile vertices
    std::vector<b2Vec2> points;
    if (oneWay)
    {
        // NOTE: behaviorally, we don't need these tiles to be shorter,
        //       but it makes the one-way behavior a bit more obvious
        points.emplace_back( col   *TILE_WIDTH, (row+1)*TILE_HEIGHT-2); // bottom left
        points.emplace_back((col+1)*TILE_WIDTH, (row+1)*TILE_HEIGHT-2); // bottom right
    }
    else
    {
        points.emplace_back( col   *TILE_WIDTH,  row   *TILE_HEIGHT); // bottom left
        points.emplace_back((col+1)*TILE_WIDTH,  row   *TILE_HEIGHT); // bottom right
    }
    points.emplace_back((col+1)*TILE_WIDTH, (row+1)*TILE_HEIGHT); // top right
    points.emplace_back( col   *TILE_WIDTH, (row+1)*TILE_HEIGHT); // top left

    // convert from pixels to meters
    for (std::size_t i = 0; i < points.size(); i++)
    {
        points[i].x *= METERS_PER_PIXEL;
        points[i].y *= METERS_PER_PIXEL;
    }

#ifdef USE_POLYGON_INSTEAD_OF_EDGES
    b2PolygonShape shape;
    shape.Set(points.data(), points.size());
    b2Fixture * const fixture = tilesBody->CreateFixture(&shape, 0.0);

    // save the fixture to coordinate mapping for future lookup
    fixtureToTileMap[fixture] = coord_pair;
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
        b2Fixture * const fixture = tilesBody->CreateFixture(&shape, 0.0);

        // save the fixture to coordinate mapping for future lookup
        fixtureToTileMap[fixture] = coord_pair;
    }
#endif // USE_POLYGON_INSTEAD_OF_EDGES

    // save the tile (currently only information is whether or not it is one-way)
    tiles[coord_pair] = oneWay;
}
