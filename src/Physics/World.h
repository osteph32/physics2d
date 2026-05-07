// World.h

#pragma once
#include "Body.h"
#include <vector>

// World
struct World {
    std::vector<Body*> bodies;
    Vec2  gravity = {0.f, 500.f};

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
            b->Integrate(dt, gravity);
    }
};