#include "PhysicsContactListener.h"
#include "GameObject.h"

#include <box2d/b2_contact.h>

void PhysicsContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->preSolve(contact, oldManifold, fixtureA, fixtureB);
    if (objB)
        objB->preSolve(contact, oldManifold, fixtureB, fixtureA);
}

void PhysicsContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->postSolve(contact, impulse, fixtureA, fixtureB);
    if (objB)
        objB->postSolve(contact, impulse, fixtureB, fixtureA);
}

void PhysicsContactListener::BeginContact(b2Contact *contact)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->beginContact(contact, fixtureA, fixtureB);
    if (objB)
        objB->beginContact(contact, fixtureB, fixtureA);
}

void PhysicsContactListener::EndContact(b2Contact *contact)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->endContact(contact, fixtureA, fixtureB);
    if (objB)
        objB->endContact(contact, fixtureB, fixtureA);
}
