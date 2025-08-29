#include "PhysicsUtilities.h"

#include <box2d/b2_fixture.h>

#include <climits>

b2AABB get_AABB_for_fixture(const b2Fixture *fixture)
{
    b2AABB aabb;
    aabb.lowerBound.Set(FLT_MAX, FLT_MAX);
    aabb.upperBound.Set(-FLT_MAX, -FLT_MAX);
    const int32 childCount = fixture->GetShape()->GetChildCount();
    for (int i = 0; i < childCount; i++)
    {
        b2AABB child_aabb;
        fixture->GetShape()->ComputeAABB(&child_aabb, fixture->GetBody()->GetTransform(), i);
        aabb.Combine(child_aabb);
    }
    return aabb;
}
