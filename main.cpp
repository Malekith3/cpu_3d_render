
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <format>
#include <ranges>

#include "common/inc/CommonDefines.h"
#include "graphics/rendering/inc/Display.h"
#include "common/inc/Vectors.hpp"
#include "graphics/shapes/inc/Mesh.h"
#include "common/inc/Colors.h"
#include "graphics/light/inc/light.h"
#include <glm/glm.hpp>
#include "utils/inc/GlmAdapter.h"
#include "utils/inc/ProjectionMat.h"

#include <glm/gtc/matrix_transform.hpp>

namespace
{
    vect3_t<float> cameraPosition{0.0f,0.0f,0.0f};
    std::vector<Triangle> trianglesToRender;
    Mesh globalMesh;
    std::vector<uint32_t> textureMesh;
    glm::mat4x4 projectionMat{0};

    enum class VertexPoint : uint8_t
    {
        A,
        B,
        C
    };

    enum class RenderingStates : uint8_t
    {
        WIREFRAME_WITH_VERTICES = 1U,           // Displays a wireframe with small red dots at each triangle vertex
        WIREFRAME_ONLY,                        // Displays only the wireframe lines
        FILLED_TRIANGLES,                     // Displays filled triangles with a solid color
        FILLED_TRIANGLES_WITH_WIREFRAME,     // Displays both filled triangles and wireframe lines
        TEXTURED_TRIANGLES,                 // Displays textured triangles
        TEXTURED_TRIANGLES_WITH_WIREFRAME, // Displays both textured triangles and wireframe lines
    };

    bool isBackFaceCullingEnabled{true};
    RenderingStates renderingState{RenderingStates::WIREFRAME_ONLY};

}

int InitWindow(SDL_Renderer*& renderer, SDL_Window*& window)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << std::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("SDL2 Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << std::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr)
    {
        std::cerr << std::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;

}

void processKeyInput(const SDL_Keycode keycode, bool& isQuitEvent)
{
    switch (keycode)
    {
        case SDLK_1:
            //Pressing “1” displays the wireframe and a small red dot for each triangle vertex
            renderingState = RenderingStates::WIREFRAME_WITH_VERTICES;
            break;
        case SDLK_2:
            //Pressing “2” displays only the wireframe lines
            renderingState = RenderingStates::WIREFRAME_ONLY;
            break;
        case SDLK_3:
            //Pressing “3” displays filled triangles with a solid color
            renderingState = RenderingStates::FILLED_TRIANGLES;
            break;
        case SDLK_4:
            //Pressing “4” displays both filled triangles and wireframe lines
            renderingState = RenderingStates::FILLED_TRIANGLES_WITH_WIREFRAME;
            break;
        case SDLK_5:
            renderingState = RenderingStates::TEXTURED_TRIANGLES;
            break;
        case SDLK_6:
            renderingState = RenderingStates::TEXTURED_TRIANGLES_WITH_WIREFRAME;
            break;
        case SDLK_c:
            //Pressing “c” we should enable back-face culling
            isBackFaceCullingEnabled = true;
            break;
        case SDLK_d:
            //Pressing “d” we should disable the back-face cullin
            isBackFaceCullingEnabled = false;
            break;
        case SDLK_ESCAPE:
            isQuitEvent = true;
        default:
            break;
    }

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
                processKeyInput(getKeySymbol(e), isQuitEvent);
                break;
            default:
                break;
        }
    }
}

void update()
{
    static Uint64 prevFrameTime;
    const auto currentFrameTime = SDL_GetTicks64();
    const auto deltaTime = currentFrameTime - prevFrameTime;
    const auto timeToWait = FRAME_TIME - deltaTime;

    trianglesToRender.clear();

    if (timeToWait > 0 && timeToWait <=FRAME_TIME)
    {
        SDL_Delay(timeToWait);
    }

    prevFrameTime = SDL_GetTicks64();
    globalMesh.rotation.x += +0.3;
    globalMesh.rotation.y += +0.3;
    globalMesh.rotation.z += +0.2;

    // globalMesh.translation.x += 0.01;
    // globalMesh.scale.x += 0.001;

    static auto offsetIndex = [](const int index){return index - 1;};
    for (const auto& [aFaceVert, bFaceVert, cFaceVert, meshColor, a_uv,b_uv,c_uv] : globalMesh.faces)
    {
        std::array<vect3_t<float>,3> faceVert{{
                globalMesh.vertices[offsetIndex(aFaceVert)],
                globalMesh.vertices[offsetIndex(bFaceVert)],
                globalMesh.vertices[offsetIndex(cFaceVert)]
            }};

        std::array<vect3_t<float>,3> transformedVertices{};

        //Transform
        std::ranges::transform(faceVert, transformedVertices.begin(), [](const auto& vert)
            {
                const glm::vec4 glmVert(vert.x,vert.y,vert.z, 1.0f);
                const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(globalMesh.rotation.x), glm::vec3(1, 0, 0)) *
                                                 glm::rotate(glm::mat4(1.0f), glm::radians(globalMesh.rotation.y), glm::vec3(0, 1, 0)) *
                                                 glm::rotate(glm::mat4(1.0f), glm::radians(globalMesh.rotation.z), glm::vec3(0, 0, 1));


                const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(globalMesh.translation.x, globalMesh.translation.y, 5.0f));  // Translate Z by 5.0f
                const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(globalMesh.scale.x, globalMesh.scale.y, globalMesh.scale.z));
                const glm::mat4 transformMatrix =  translationMatrix * rotationMatrix * scaleMatrix;
                const glm::vec4 transformedVert = transformMatrix * glmVert;

                return vect3_t(transformedVert.x, transformedVert.y, transformedVert.z);
            });

        auto isRenderTriangle{true};

        //Culling
        auto vectorAB = transformedVertices[static_cast<size_t>(VertexPoint::B)] -
                                    transformedVertices[static_cast<size_t>(VertexPoint::A)];
        auto vectorAC =  transformedVertices[static_cast<size_t>(VertexPoint::C)] -
                                     transformedVertices[static_cast<size_t>(VertexPoint::A)];
        auto cameraVector =     cameraPosition -
                                            transformedVertices[static_cast<size_t>(VertexPoint::A)];
        auto faceNormal = vectorAB.cross(vectorAC).normalize();
        auto projectionNormal = faceNormal.dot(cameraVector);

        if (isBackFaceCullingEnabled)
        {
            isRenderTriangle = projectionNormal >= -std::numeric_limits<float>::epsilon();
        }


        if (isRenderTriangle)
        {
            Triangle projectedTriangle;
            const auto& globalLight {getGlobalLight()};
            const float lightIntensity = -faceNormal.dot(globalLight._direction);
            projectedTriangle._color = applyIntensityToColor(meshColor, lightIntensity);
            projectedTriangle.textCoord = std::array<Texture2d,3>{{{a_uv},{b_uv},{c_uv}}};
            projectedTriangle.setAvgDepth((transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3.0f);
            std::ranges::transform(transformedVertices, projectedTriangle._points.begin(), [](auto& vert)
                {
                    auto res = Utils::projectWithMat(projectionMat,{vert.x,vert.y,vert.z,1});

                    res.x *= WINDOW_WIDTH / 2.0f;
                    res.y *= WINDOW_HEIGHT / 2.0f;

                    //Flip the Y axis because the model is loaded with y up
                    res.y *= -1.0f;

                    res.x += WINDOW_WIDTH / 2.0f;
                    res.y += WINDOW_HEIGHT / 2.0f;
                    return vect2_t<float>{res.x,res.y};
                });
            trianglesToRender.push_back(projectedTriangle);
        }



    }

    std::ranges::sort(trianglesToRender, [](auto& firstTriangle, auto& secondTriangle)
    {
        return firstTriangle.getAvgDepth() > secondTriangle.getAvgDepth();
    });

}

void render(SDL_Renderer*& renderer, std::array<uint32_t, COLOR_BUFFER_SIZE>& colorBuffer, SDL_Texture*& colorBufferTexture)
{
    auto renderColorBuffer = [&]()
    {
      SDL_UpdateTexture(colorBufferTexture, nullptr, &colorBuffer, static_cast<int>(WINDOW_WIDTH * sizeof(uint32_t)));
      SDL_RenderCopy(renderer, colorBufferTexture, nullptr, nullptr);
    };

    for (auto& triangle : trianglesToRender)
    {
        auto points{triangle._points};
        auto [point0,point1,point2] = points;


        if ( renderingState == RenderingStates::WIREFRAME_WITH_VERTICES )
        {
            // For each point in the triangle, draw a small red dot (wireframe with vertices)
            std::ranges::for_each(points, [&colorBuffer](const auto& point)
            {
                Render::drawRect(colorBuffer, point.x, point.y, 3, 3, toColorValue(Colors::RED));
            });
        }

        if (
               renderingState == RenderingStates::FILLED_TRIANGLES
            || renderingState == RenderingStates::FILLED_TRIANGLES_WITH_WIREFRAME ) {
            // Draw a filled triangle with a flat bottom in green color (solid color rendering)
            Render::drawFilledTriangleFlatBottom(colorBuffer, Triangle(points), triangle._color);

            // Draw the triangle's outline (wireframe) in green color
            Render::drawTriangle(colorBuffer, {point0.x, point0.y}, {point1.x, point1.y}, {point2.x, point2.y}, triangle._color);
        }

        if (
               renderingState == RenderingStates::WIREFRAME_ONLY
            || renderingState == RenderingStates::WIREFRAME_WITH_VERTICES
            || renderingState == RenderingStates::FILLED_TRIANGLES_WITH_WIREFRAME
            || renderingState == RenderingStates::TEXTURED_TRIANGLES_WITH_WIREFRAME)
        {
            // Draw the triangle's outline (wireframe) in blue color
            Render::drawTriangle(colorBuffer, {point0.x, point0.y}, {point1.x, point1.y}, {point2.x, point2.y}, toColorValue(Colors::BLUE));
        }

        if (
            renderingState == RenderingStates::TEXTURED_TRIANGLES
            || renderingState == RenderingStates::TEXTURED_TRIANGLES_WITH_WIREFRAME)
        {
          // TODO add draw texture
        }


    }
    renderColorBuffer();
    colorBuffer.fill(0xFF000000);
    SDL_RenderPresent(renderer);

}

void setup(SDL_Renderer*& renderer, std::array<uint32_t, COLOR_BUFFER_SIZE>& colorBuffer, SDL_Texture*& colorBufferTexture)
{
    std::ranges::fill(colorBuffer, ZERO_VALUE_COLOR_BUFFER);
    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);

    constexpr auto fov = glm::radians(60.0f);
    constexpr float aspect =  WINDOW_WIDTH/ static_cast<float>(WINDOW_HEIGHT);
    constexpr float zNear = 0.1f;
    constexpr float zFar = 100.0f;

    projectionMat = Utils::makePerspectiveMat4(fov, aspect, zNear, zFar);

    std::vector<vect3_t<float>> loadedVertex;
    std::vector<Face> loadedFaces;
    // LoadOBJFileSimplified("./assets/f22.obj", loadedVertex, loadedFaces);
    std::ranges::copy(cubeMeshFaces, std::back_inserter(loadedFaces));
    std::ranges::copy(cubeMeshVert, std::back_inserter(loadedVertex));

    // //good old unsafe c way :D
    // textureMesh.assign(
    // reinterpret_cast<const uint32_t*>(REDBRICK_TEXTURE.data()),
    // reinterpret_cast<const uint32_t*>(REDBRICK_TEXTURE.data() + REDBRICK_TEXTURE.size()));

    //Fancy modern cpp way with chunks
    textureMesh.reserve(REDBRICK_TEXTURE.size() / 4);
    for (const auto chunk : REDBRICK_TEXTURE | std::views::chunk(4))
    {
        uint32_t textureColor = chunk[3] << 24 | chunk[0] << 16 | chunk[1] << 8 | chunk[2];
        textureMesh.emplace_back(textureColor);
    }


    std::ranges::copy(loadedVertex, std::back_inserter(globalMesh.vertices));
    std::ranges::copy(loadedFaces, std::back_inserter(globalMesh.faces));
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