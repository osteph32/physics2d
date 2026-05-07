// Body.h

#pragma once
#include "../Math/Vec2.h"
#include "../Math/Mat2.h"
#include "Shape.h"

// Body: object in physics simulation
struct Body {
    Vec2 position;
    float rotation = 0.f;
    Mat2  orient;

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

    Shape* shape = nullptr;

    // Constructors
    Body(Vec2 pos, float mass, Shape* shape)
        : position(pos), shape(shape)
    {
        SetMass(mass);
        SetInertia(shape->ComputeInertia(mass));
    }

    static Body MakeStatic(Vec2 pos, Shape* shape) {
        Body b(pos, 1.f, shape);
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

    void Integrate(float dt, Vec2 gravity) {
        if (invMass == 0.f) return;
        velocity += (force * invMass + gravity) * dt;
        position += velocity * dt;
        angularVelocity += torque * invInertia * dt;
        rotation += angularVelocity * dt;
        rotation += angularVelocity * dt;
        orient = Mat2(rotation);

        ClearForces();
    }
};