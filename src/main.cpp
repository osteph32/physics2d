// main.cpp

#include <SDL2/SDL.h>
#include <cstdio>
#include <cmath>
#include "Math/Vec2.h"
#include "Math/Mat2.h"
#include "Physics/Body.h"
#include "Physics/Shape.h"

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
        Vec2 a = body.orient * poly->vertices[i]        + body.position;
        Vec2 b = body.orient * poly->vertices[(i+1)%n]  + body.position;
        SDL_RenderDrawLine(r, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
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
    
    // Simulation Setup
    const Vec2 gravity = {0.f, 500.f};
    const float dt = 1.7 / 60.f;

    Circle  ballShape(20.f);
    Polygon boxShape;  boxShape.SetBox(40.f, 40.f);
    Polygon floorShape; floorShape.SetBox(400.f, 10.f);

    Body ball({400.f, 50.f}, 1.f, &ballShape);
    Body box ({400.f, 200.f}, 2.f, &boxShape);
    Body floor = Body::MakeStatic({400.f, 570.f}, &floorShape);

    ball.restitution = 0.6f;
    box.restitution = 0.4f;
    box.rotation = 0.3f;

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
            ball.Integrate(dt, gravity);
            box .Integrate(dt, gravity);
            accumulator -= dt;
        }
        
        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        DrawCircle(renderer, ball.position, ballShape.radius, {100, 180, 255, 255});
        DrawPolygon(renderer, box, {255, 160, 60, 255});
        DrawPolygon(renderer, floor, {180, 180, 180, 255});

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
