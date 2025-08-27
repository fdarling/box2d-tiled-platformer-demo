#include "Player.h"
#include "globals.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

// #include <iostream>

static const int PLAYER_WIDTH = 16; // pixels
static const int PLAYER_HEIGHT = 24; // pixels
static const int PLAYER_WALKING_VELOCITY = 3.0; // m/s
static const int PLAYER_JUMP_VELOCITY = 5.0; // m/s

Player::Player(b2World *bWorld) : world(bWorld), body(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(300*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
    body = world->CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((PLAYER_WIDTH * METERS_PER_PIXEL)/2.0, (PLAYER_HEIGHT * METERS_PER_PIXEL)/2.0, b2Vec2(0.0, 0.0), 0.0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 2.0;

    body->CreateFixture(&fixtureDef);
}

Player::~Player() = default;

void Player::setWalkingDir(float dir)
{
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = dir*PLAYER_WALKING_VELOCITY;
    body->SetLinearVelocity(vel);
}

void Player::jump()
{
    if (!canJump())
        return;
    b2Vec2 vel = body->GetLinearVelocity();
    vel.y = PLAYER_JUMP_VELOCITY;
    body->SetLinearVelocity(vel);
}

bool Player::canJump() const
{
    return !fixtures_underfoot.empty();
}

void Player::beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)ourFixture;

    b2WorldManifold contactWorldManifold;
    contact->GetWorldManifold(&contactWorldManifold);
    // std::cout << "CONTACT NORMAL:" << contactWorldManifold.normal.x << "," << contactWorldManifold.normal.y << std::endl;

    // determine if we are standing on the colliding object
    if (contactWorldManifold.normal.y > 0.2)
    {
        fixtures_underfoot.insert(otherFixture);
        contact->SetFriction(0.95);
    }
    else
    {
        fixtures_underfoot.erase(otherFixture);
        contact->SetFriction(0.0);
    }
}

void Player::endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;
    (void)ourFixture;

    fixtures_underfoot.erase(otherFixture);
}
