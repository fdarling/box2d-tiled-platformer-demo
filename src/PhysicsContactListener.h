#pragma once

#include <box2d/b2_world_callbacks.h>

class PhysicsContactListener : public b2ContactListener
{
public:
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};
