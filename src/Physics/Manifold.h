// Manifold.h

#pragma once
#include "../Math/Vec2.h"

struct Body;

struct Manifold {
    Body* A = nullptr;
    Body* B = nullptr;

    Vec2 normal;
    float penetration = 0;
    Vec2 contacts[2];
    int contactCount = 0;
};