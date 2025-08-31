#pragma once

#include "GameObject.h"

#include <utility>
#include <unordered_map>

// Box2D forward declarations
class b2World;
class b2Body;

class Level : public GameObject
{
public:
    Level(b2World *bWorld);
    ~Level();

    bool fixtureIsOneWay(const b2Fixture *fixture) const;

    void preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
protected:
    void addTile(int row, int col, bool oneWay = false);

    struct coord_pair_hash
    {
        std::size_t operator() (const std::pair<int, int> &p) const
        {
            return std::hash<int>()(p.first << 16 | p.second);
        }
    };

    b2World *world;
    b2Body *tilesBody;
    std::unordered_map<std::pair<int, int>, bool, coord_pair_hash> tiles;
    std::unordered_map<b2Fixture*, std::pair<int, int> > fixtureToTileMap;
};
