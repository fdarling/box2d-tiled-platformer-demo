#pragma once

// Box2D forward declarations
class b2World;
class b2Body;

class Level
{
public:
    Level(b2World *bWorld);
    ~Level();
protected:
    void addTile(int row, int col);

    b2World *world;
    b2Body *tilesBody;
};
