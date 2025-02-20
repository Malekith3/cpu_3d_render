
#include "external/SDL2/include/SDL.h"
#include "external/fmt/include/fmt/core.h"
#include <iostream>
#include <bits/ranges_algobase.h>
#include "CommonDefines.h"
#include "Display.h"
#include "Vectors.h"

namespace
{
    constexpr size_t N_POINTS = 9u*9u*9u;
    std::array<vect3_t<float>,N_POINTS> awsomePoints;
}

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

void processInput(bool& isQuitEvent)
{
    SDL_Event e;
    auto getKeySymbol= [](const SDL_Event& event){ return event.key.keysym.sym;};
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            isQuitEvent = true;
            break;
        case SDL_KEYDOWN:
            isQuitEvent = (getKeySymbol(e) == SDLK_ESCAPE);
            break;
        default:
            break;
        }
    }
}

void update()
{
    static Uint64 prevFrameTime;
    auto currentFrameTime = SDL_GetTicks64();
    auto deltaTime = currentFrameTime - prevFrameTime;
    auto timeToWait = FRAME_TIME - deltaTime;

    if (timeToWait > 0 && timeToWait <=FRAME_TIME)
    {
        SDL_Delay(timeToWait);
    }

    prevFrameTime = SDL_GetTicks64();
}

void render(SDL_Renderer*& renderer, std::array<uint32_t, COLOR_BUFFER_SIZE>& colorBuffer, SDL_Texture*& colorBufferTexture)
{
    auto renderColorBuffer = [&]()
    {
      SDL_UpdateTexture(colorBufferTexture, nullptr, &colorBuffer, static_cast<int>(WINDOW_WIDTH * sizeof(uint32_t)));
      SDL_RenderCopy(renderer, colorBufferTexture, nullptr, nullptr);
    };

    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
    SDL_RenderClear(renderer);
    renderColorBuffer();
    colorBuffer.fill(0xFFFFFF00);
    Render::drawGrid(colorBuffer);
    Render::drawRect(colorBuffer,40,10,10,10,0xFFFF0000);
    SDL_RenderPresent(renderer);

}

void setup(SDL_Renderer*& renderer, std::array<uint32_t, COLOR_BUFFER_SIZE>& colorBuffer, SDL_Texture*& colorBufferTexture)
{
    std::ranges::fill(colorBuffer, ZERO_VALUE_COLOR_BUFFER);
    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);
    size_t point_count = 0u;
    for (float x = -1.0f; x <=1.0f; x+=0.25f)
    {
        for (float y = -1.0f; y <=1.0f; y+=0.25f)
        {
            for (float z = -1.0f; z <=1.0f; z+=0.25f)
            {
                vect3_t<float> newPoint = {x,y,z};
                awsomePoints[point_count++] = newPoint;
            }
        }
    }
}

void CleanUp(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Texture*& texture)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    SDL_Renderer* renderer{nullptr};
    SDL_Window* window{nullptr};

    if (const auto hasError = InitWindow(renderer, window); hasError != 0)
    {
        return hasError;
    }

    SDL_Texture* colorBufferTexture;
    std::array<uint32_t, COLOR_BUFFER_SIZE> colorBuffer{};
    setup(renderer,colorBuffer,colorBufferTexture);
    auto quit{false};

    while (!quit)
    {
        processInput(quit);
        update();
        render(renderer, colorBuffer, colorBufferTexture);
    }

    CleanUp(window, renderer, colorBufferTexture);

    return 0;
}