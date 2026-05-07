// Collision.h

#pragma once
#include "Manifold.h"
#include "Body.h"
#include "Shape.h"
#include <cmath>
#include <algorithm>

inline bool CircleVsCircle(Manifold& m, Body* a, Body* b) {
    auto* ca = static_cast<Circle*>(a -> shape);
    auto* cb = static_cast<Circle*>(b -> shape);

    Vec2 diff = b -> position - a -> position;
    float dist = diff.Len();
    float radii = ca -> radius + cb -> radius;

    if (dist >= radii) return false;

    m.A = a;
    m.B = b;

    if (dist < 1e-6f) {
        m.penetration = ca -> radius;
        m.normal = {1.f, 0.f};
        m.contacts[0] = a -> position;
    } else {
        m.penetration = radii - dist;
        m.normal = diff / dist;
        m.contacts[0] = m.normal * ca -> radius + a -> position;
    }

    m.contactCount = 1;
    return true;
}

inline bool CircleVsPolygon(Manifold& m, Body* a, Body* b) {
    auto* circle = static_cast<Circle*>(a -> shape);
    auto* poly = static_cast<Polygon*>(b -> shape);
    Vec2 center = b -> orient.Transpose() * (a -> position - b -> position);

    float separation = -1e30f;
    int bestFace = 0;
    int n = (int)poly -> vertices.size();

    for (int i = 0; i < n; ++i) {
        float s = poly -> normals[i].Dot(center - poly -> vertices[i]);
        if (s > circle -> radius) return false;
        if (s > separation) { separation = s; bestFace = i; }
    }

    if (separation < 1e-6f) {
        m.A = a; m.B = b;
        m.normal = -(b -> orient * poly -> normals[bestFace]);
        m.penetration = circle -> radius - separation;
        m.contacts[0] = m.normal * circle -> radius + a -> position;
        m.contactCount = 1;
        return true;
    }

    Vec2 v1 = poly -> vertices[bestFace];
    Vec2 v2 = poly -> vertices[(bestFace + 1) % n];

    float dot1 = (center - v1).Dot(v2 - v1);
    float dot2 = (center - v2).Dot(v1 - v2);

    m.A = a; 
    m.B = b;

    m.penetration  = circle -> radius - separation;
    m.contactCount = 1;

    if (dot1 <= 0.f) {
        if ((center - v1).LenSq() > circle -> radius * circle -> radius) return false;
        m.normal = (b->orient * (v1 - center)).Norm();
        m.contacts[0] = b->orient * v1 + b->position;

    } else if (dot2 <= 0.f) {
        if ((center - v2).LenSq() > circle -> radius * circle -> radius) return false;
        m.normal = (b->orient * (v2 - center)).Norm();
        m.contacts[0] = b->orient * v2 + b -> position;

    } else {
        Vec2 faceN = poly -> normals[bestFace];
        if ((center - v1).Dot(faceN) > circle -> radius) return false;
        m.normal = -(b -> orient * faceN);
        m.contacts[0] = m.normal * circle -> radius + a->position;
    }
    return true;
}

inline bool Collide(Manifold& m, Body* a, Body* b) {
    ShapeType ta = a -> shape -> type;
    ShapeType tb = b -> shape -> type;

    if (ta == ShapeType::Circle && tb == ShapeType::Circle)
        return CircleVsCircle(m, a, b);
    
    if (ta == ShapeType::Circle && tb == ShapeType::Polygon)
        return CircleVsPolygon(m, a, b);

    if (ta == ShapeType::Polygon && tb == ShapeType::Circle)
        return CircleVsPolygon(m, b, a);
    
    return false;
}