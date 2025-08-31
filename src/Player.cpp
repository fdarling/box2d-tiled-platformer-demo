#include "Player.h"
#include "Level.h"
#include "PhysicsUtilities.h"
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
static const int PLAYER_DROP_VELOCITY = 1.0; // m/s

Player::Player(b2World *bWorld, Level *lvl) : world(bWorld), body(nullptr), level(lvl)
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

void Player::dropDown()
{
    for (std::set<b2Fixture*>::iterator it = fixtures_underfoot.begin(); it != fixtures_underfoot.end(); ++it)
    {
        b2Fixture * const fixture = *it;
        if (!level->fixtureIsOneWay(fixture))
        {
            // std::cout << "Can't drop!" << std::endl;
            return;
        }
    }
    // std::cout << "Can drop!" << std::endl;

    // set it up so that we'll phase through these fixtures
    drop_through_fixtures = fixtures_underfoot;
    fixtures_underfoot.clear();

    // set our velocity downward
    b2Vec2 vel = body->GetLinearVelocity();
    vel.y = -PLAYER_DROP_VELOCITY;
    body->SetLinearVelocity(vel);
}

bool Player::canJump() const
{
    return !fixtures_underfoot.empty();
}

void Player::preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)oldManifold;
    (void)ourFixture;

    // check if we are done dropping through
    if (drop_through_fixtures.empty())
        return;

    // check if this is not even a one-way tile (where we can never drop through)
    if (!level->fixtureIsOneWay(otherFixture))
        return;

    // check if this is a fixture we have already marked as no-contact
    if (drop_through_fixtures.find(otherFixture) != drop_through_fixtures.end())
    {
        contact->SetEnabled(false);
        return;
    }

    // check to see if this fixture is at the same level as the ones we marked
    b2Fixture * const markedFixture = *drop_through_fixtures.begin();
    const b2AABB marked_aabb = get_AABB_for_fixture(markedFixture);
    const b2AABB tile_aabb = get_AABB_for_fixture(otherFixture);
    if (tile_aabb.upperBound.y == marked_aabb.upperBound.y)
    {
        // drop through this tile too
        drop_through_fixtures.insert(otherFixture);
        contact->SetEnabled(false);
    }
}

void Player::beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    b2WorldManifold contactWorldManifold;
    contact->GetWorldManifold(&contactWorldManifold);
    // std::cout << "CONTACT NORMAL:" << contactWorldManifold.normal.x << "," << contactWorldManifold.normal.y << std::endl;

    const b2AABB player_aabb = get_AABB_for_fixture(ourFixture);
    const b2AABB tile_aabb = get_AABB_for_fixture(otherFixture);
    const bool isAbove = player_aabb.lowerBound.y + OVERLAP_TOLERANCE >= tile_aabb.upperBound.y;
    // std::cout << "Player::beginContact(): normal.y: " << contactWorldManifold.normal.y << "; tile_aabb.upperBound.y: " << tile_aabb.upperBound.y << "; player_aabb.lowerBound.y: " << player_aabb.lowerBound.y << "; isAbove: " << isAbove << std::endl;

    // determine if we are standing on the colliding object
    if (contactWorldManifold.normal.y > 0.2 && isAbove)
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
    drop_through_fixtures.erase(otherFixture);
}
