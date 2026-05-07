// main.cpp

#include <SDL2/SDL.h>
#include <cstdio>
#include "Math/Vec2.h"
#include "Physics/Body.h"

// Filled circle
void DrawCircle(SDL_Renderer* r, Vec2 p, float radius, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);

    int cx = (int)p.x, cy = (int)p.y, rad = (int)radius;

    for (int dy = -rad; dy <= rad; dy++) {
        int dx = (int)std::sqrt((float)(rad*rad - dy*dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void DrawRect(SDL_Renderer* r, Vec2 pos, float hw, float hh, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);
    SDL_Rect rect {
        (int)(pos.x - hw), (int)(pos.y - hh),
        (int)(hw * 2), (int)(hh * 2)
    };
    SDL_RenderDrawRect(r, &rect);
}

int main() {

    // Body test
    Body ball({400, 100}, 1.f, 20.f);
    printf("mass=%.1f  invMass=%.4f\n", ball.mass, ball.invMass);  // 1.0  1.0
    printf("inertia=%.1f\n", ball.inertia);                        // 200.0

    Body ground = Body::MakeStatic({400, 580}, 400.f);
    printf("static invMass=%.1f\n", ground.invMass);

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
    
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) {
                running = false;
            }
        
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        DrawCircle(renderer, ball.position, ball.radius, {100, 180, 255, 255});
        DrawCircle(renderer, ground.position, ground.radius, {180, 180, 180, 255});

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
