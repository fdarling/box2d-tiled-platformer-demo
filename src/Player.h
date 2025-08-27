#pragma once

#include "GameObject.h"

#include <set>

// Box2D forward declarations
class b2World;
class b2Body;

class Player : public GameObject
{
public:
    Player(b2World *bWorld);
    ~Player();
    void setWalkingDir(float dir);
    void jump();
    bool canJump() const;
    void beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
protected:
    b2World *world;
    b2Body *body;
    std::set<b2Fixture*> fixtures_underfoot;
};
