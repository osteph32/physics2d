// Collision.h

#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Shape.h"
#include <cmath>

inline bool CircleVsCircle(Manifold& m, Body* a, Body* b) {
    auto* ca = static_cast<Circle*>(a->shape);
    auto* cb = static_cast<Circle*>(b->shape);

    Vec2 diff = b->position - a->position;
    float dist = diff.Len();
    float radii = ca->radius + cb->radius;

    if (dist >= radii) return false;

    m.A = a;
    m.B = b;

    if (dist < 1e-6f) {
        m.penetration = ca->radius;
        m.normal = {1.f, 0.f};
        m.contacts[0] = a->position;
    } else {
        m.penetration = radii - dist;
        m.normal = diff / dist;
        m.contacts[0] = m.normal * ca->radius + a->position;
    }

    m.contactCount = 1;
    return true;
}

inline bool Collide(Manifold& m, Body* a, Body* b) {
    ShapeType ta = a -> shape -> type;
    ShapeType tb = b -> shape -> type;

    if (ta == ShapeType::Circle && tb == ShapeType::Circle)
        return CircleVsCircle(m, a, b);
    
    return false;
}