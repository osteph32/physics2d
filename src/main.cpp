// main.cpp

#include <SDL2/SDL.h>
#include <cmath>
#include "Math/Vec2.h"
#include "Physics/World.h"

// Circle rendering
void DrawCircle(SDL_Renderer* r, Vec2 p, float radius, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);
    int cx = (int)p.x, cy = (int)p.y, rad = (int)radius;
    for (int dy = -rad; dy <= rad; dy++) {
        int dx = (int)std::sqrt((float)(rad*rad - dy*dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

// Polygon
void DrawPolygon(SDL_Renderer* r, Body& body, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);
    auto* poly = static_cast<Polygon*>(body.shape);
    int n = (int)poly->vertices.size();
    for (int i = 0; i < n; ++i) {
        Vec2 a = body.orient * poly->vertices[i] + body.position;
        Vec2 b = body.orient * poly->vertices[(i+1)%n] + body.position;
        SDL_RenderDrawLine(r, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
    }
}

// Body
void DrawBody(SDL_Renderer* r, Body* b) {
    SDL_Color circleCol = {100, 180, 255, 255};
    SDL_Color polyCol = {255, 160, 60, 255};
    SDL_Color staticCol = {180, 180, 180, 255};
    bool isStatic = (b->invMass == 0.f);
    if (b->shape->type == ShapeType::Circle) {
        auto* c = static_cast<Circle*>(b->shape);
        DrawCircle(r, b->position, c->radius, isStatic ? staticCol : circleCol);
    } else {
        DrawPolygon(r, *b, isStatic ? staticCol : polyCol);
    }
}

int main() {
    // SDL Window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "physics2d",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, 
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    const float dt = 1.7 / 60.f;

    // Shapes (World.h owns bodies)
    Circle ballA(25.f), ballB(20.f), ballC(30.f);
    Polygon floorShape; floorShape.SetBox(390.f, 10.f);
    Polygon wallLShape; wallLShape.SetBox(10.f, 300.f);
    Polygon wallRShape; wallRShape.SetBox(10.f, 300.f);

    World world;

    auto* b1 = world.AddDynamic({400.f, 50.f}, 1.f, &ballA);
    auto* b2 = world.AddDynamic({405.f, 150.f}, 1.f, &ballB);
    auto* b3 = world.AddDynamic({395.f, 270.f}, 2.f, &ballC);

    b1 -> restitution = 0.5f;
    b2 -> restitution = 0.5f;
    b3 -> restitution = 0.3f;


    world.AddStatic({400.f, 570.f}, &floorShape);
    world.AddStatic({10.f, 300.f}, &wallLShape);
    world.AddStatic({790.f, 300.f}, &wallRShape);

    Uint32 prevTime = SDL_GetTicks();
    float  accumulator = 0.f;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) {
                running = false;
            }
        
        Uint32 now = SDL_GetTicks();
        accumulator += (now - prevTime) / 1000.f;
        prevTime = now;

        while (accumulator >= dt) {
            world.Step(dt);
            accumulator -= dt;
        }
        
        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        for (auto* b : world.bodies)
            DrawBody(renderer, b);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
