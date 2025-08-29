#pragma once

// Box2D forward declarations
class b2AABB;
class b2Fixture;

b2AABB get_AABB_for_fixture(const b2Fixture *fixture);
