// main.cpp

#include <SDL2/SDL.h>
#include <cstdio>
#include "Math/Vec2.h"

int main() {
    
    // Vec2 Test
    Vec2 a{3.f, 0.f};
    Vec2 b{0.f, 4.f};

    printf("a + b      = (%.1f, %.1f)\n", (a+b).x, (a+b).y);   // (3, 4)
    printf("a.Dot(b)   = %.1f\n",  a.Dot(b));                   // 0
    printf("a.Cross(b) = %.1f\n",  a.Cross(b));                  // 12
    printf("b.Len()    = %.1f\n",  b.Len());                     // 4
    printf("b.Norm()   = (%.1f, %.1f)\n", b.Norm().x, b.Norm().y); // (0, 1)
    printf("a.Perp()   = (%.1f, %.1f)\n", a.Perp().x, a.Perp().y); // (0, 3)

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
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
