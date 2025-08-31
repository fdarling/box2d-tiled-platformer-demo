#pragma once

#include "GameObject.h"

#include <set>

// forward declaration
class Level;

// Box2D forward declarations
class b2World;
class b2Body;

class Player : public GameObject
{
public:
    Player(b2World *bWorld, Level *lvl);
    ~Player();
    void setWalkingDir(float dir);
    void jump();
    void dropDown();
    bool canJump() const;
    void preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
protected:
    b2World *world;
    b2Body *body;
    Level *level;
    std::set<b2Fixture*> fixtures_underfoot;
    std::set<b2Fixture*> drop_through_fixtures;
};
