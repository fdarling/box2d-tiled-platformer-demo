#include "GameObject.h"

GameObject::GameObject() = default;

GameObject::~GameObject() = default;

void GameObject::preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;
    (void)oldManifold;
    (void)ourFixture;
    (void)otherFixture;
}

void GameObject::postSolve(b2Contact *contact, const b2ContactImpulse *impulse, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;
    (void)impulse;
    (void)ourFixture;
    (void)otherFixture;
}

void GameObject::beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;
    (void)ourFixture;
    (void)otherFixture;
}

void GameObject::endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;
    (void)ourFixture;
    (void)otherFixture;
}
