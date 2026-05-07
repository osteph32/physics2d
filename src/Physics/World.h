// World.h

#pragma once
#include "Body.h"
#include "Manifold.h"
#include "Collision.h"
#include <vector>

// World
struct World {
    std::vector<Body*> bodies;
    std::vector<Manifold> manifolds;
    Vec2 gravity = {0.f, 500.f};

    ~World() {
        for (auto* b : bodies) delete b;
    }

    Body* Add(Body* b) {
        bodies.push_back(b);
        return b;
    }

    Body* AddDynamic(Vec2 pos, float mass, Shape* shape) {
        return Add(new Body(pos, mass, shape));
    }

    Body* AddStatic(Vec2 pos, Shape* shape) {
        return Add(new Body(Body::MakeStatic(pos, shape)));
    }

    void Step(float dt) {
        for (auto* b : bodies)
            b -> Integrate(dt, gravity);

        // Temporary hard boundaries
        for (auto* b : bodies) {
            if (b->invMass == 0.f) continue;
            if (b->shape->type != ShapeType::Circle) continue;
            auto* c = static_cast<Circle*>(b->shape);
            if (b->position.y + c->radius > 560.f) {
                b->position.y  = 560.f - c->radius;
                b->velocity.y *= -b->restitution;
            }
            if (b->position.x - c->radius < 20.f) {
                b->position.x  = 20.f + c->radius;
                b->velocity.x *= -b->restitution;
            }
            if  (b->position.x + c->radius > 780.f) {
                b->position.x  = 780.f - c->radius;
                b->velocity.x *= -b->restitution;
            }
        }

        manifolds.clear();
        for (int i = 0; i < (int)bodies.size(); ++i) {
            for (int j = i+1; j < (int)bodies.size(); ++j) {
                Manifold m;
                if (Collide(m, bodies[i], bodies[j]))
                    manifolds.push_back(m);
            }
        }

        for (auto& m : manifolds)
            Resolve(m);

        for (auto& m : manifolds)
            PositionalCorrection(m);
    }

    void Resolve (Manifold& m) {
        Vec2 rv = m.B -> velocity - m.A -> velocity;
        float velN = rv.Dot(m.normal);
        
        if (velN > 0) return;
        
        float e = std::min(m.A -> restitution, m.B -> restitution);
        float invM = m.A -> invMass + m.B -> invMass;
        if (invM == 0.f) return;
        
        float j  = -(1.f + e) * velN / invM;
        Vec2 impulse = m.normal * j;
        m.A -> velocity -= m.A -> invMass * impulse;
        m.B -> velocity += m.B -> invMass * impulse;
    }

    void PositionalCorrection(Manifold& m) {
        const float percent = 0.4f;
        const float slop = 0.05f;

        float invM = m.A -> invMass + m.B -> invMass;
        if (invM == 0.f) return;

        Vec2 correction = m.normal *
            (std::max(m.penetration - slop, 0.f) / invM * percent);

        m.A -> position -= m.A -> invMass * correction;
        m.B -> position += m.B -> invMass * correction;
    }
};