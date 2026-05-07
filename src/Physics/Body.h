// Body.h

#pragma once
#include "../Math/Vec2.h"

// Body: object in physics simulation
struct Body {
    Vec2 position;
    float rotation = 0.f;

    Vec2 velocity;
    Vec2 force;

    float angularVelocity = 0.f;
    float torque = 0.f;

    float mass = 1.f;
    float invMass = 1.f;
    float inertia = 1.f;
    float invInertia = 1.f;

    float restitution = 0.3f;
    float friction = 0.5f;

    float radius = 20.f;

    // Constructors
    Body(Vec2 pos, float mass, float radius)
        : position(pos), radius(radius)
    {
        SetMass(mass);
        SetInertia(0.5f * mass * radius * radius);
    }

    static Body MakeStatic(Vec2 pos, float radius) {
        Body b(pos, 1.f, radius);
        b.invMass = 0.f;
        b.invInertia = 0.f;
        return b;
    }

    // Helpers
    void SetMass(float m) {
        mass = m;
        invMass = (m > 0.f) ? 1.f / m : 0.f;
    }

    void SetInertia(float i) {
        inertia = i;
        invInertia = (i > 0.f) ? 1.f / i : 0.f;
    }

    void ApplyForce(Vec2 f) {
        force += f;
    }

    void ApplyImpulse(Vec2 impulse, Vec2 contactVector) {
        velocity += invMass * impulse;
        angularVelocity += invInertia * contactVector.Cross(impulse);
    }

    void ClearForces() {
        force = {};
        torque = 0.f;
    }
};