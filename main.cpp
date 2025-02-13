#include "external/SDL2/include/SDL.h"
#include "external/fmt/include/fmt/core.h"
#include <iostream>

int InitWindow(SDL_Renderer*& renderer, SDL_Window*& window)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << fmt::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("SDL2 Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << fmt::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr)
    {
        std::cerr << fmt::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;

}


int main(int argc, char* argv[])
{

    SDL_Renderer* renderer{nullptr};
    SDL_Window* window{nullptr};

    if (const auto hasError = InitWindow(renderer, window); hasError != 0)
    {
        return hasError;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 100, 100, 700, 100);
        SDL_RenderDrawLine(renderer, 700, 100, 700, 500);
        SDL_RenderDrawLine(renderer, 700, 500, 100, 500);
        SDL_RenderDrawLine(renderer, 100, 500, 100, 100);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}