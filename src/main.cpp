// main.cpp

#include <SDL2/SDL.h>
#include <cstdio>
#include "Math/Vec2.h"
#include "Physics/Body.h"

// Circle
void DrawCircle(SDL_Renderer* r, Vec2 p, float radius, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);
    int cx = (int)p.x, cy = (int)p.y, rad = (int)radius;
    for (int dy = -rad; dy <= rad; dy++) {
        int dx = (int)std::sqrt((float)(rad*rad - dy*dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
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

    Body ball({400.f, 50.f}, 1.f, 20.f);
    ball.restitution = 0.6f;

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
        float elapsed = (now - prevTime) / 1000.f;
        prevTime = now;
        accumulator += elapsed;

        while (accumulator >= dt) {
            ball.Integrate(dt, gravity);
            const float floorY = 580.f;
            if (ball.position.y + ball.radius > floorY) {
                ball.position.y = floorY - ball.radius;
                ball.velocity.y *= -ball.restitution;
            }

            accumulator -= dt;
        }
        
        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        DrawCircle(renderer, ball.position, ball.radius, {100, 180, 255, 255});

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderDrawLine(renderer, 0, 580, 800, 580);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
