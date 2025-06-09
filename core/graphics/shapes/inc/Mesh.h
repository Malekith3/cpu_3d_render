#ifndef MESH_H
#define MESH_H

#include "graphics/shapes/inc/Triangle.h"

#include "common/inc/Vectors.hpp"

#include <array>
#include <filesystem>
#include <vector>


constexpr size_t N_CUBE_MESH_VERT = 8u;
constexpr size_t N_CUBE_MESH_FACES = 6u*2u;

constexpr std::array<vect3_t<float>, N_CUBE_MESH_VERT> cubeMeshVert = {{
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
}};

constexpr uint32_t CUBE_MESH_COLOR= 0xFFFFFFFF;
constexpr std::array<Face, N_CUBE_MESH_FACES> cubeMeshFaces = {{
    // front
    { .a = 1, .b = 2, .c = 3, .color = CUBE_MESH_COLOR, 
      .a_uv = {0,0}, .b_uv = {0,1}, .c_uv = {1,1}},
    { .a = 1, .b = 3, .c = 4, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {1,1}, .c_uv = {1,0}},
    // right
    { .a = 4, .b = 3, .c = 5, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {0,1}, .c_uv = {1,1}},
    { .a = 4, .b = 5, .c = 6, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {1,1}, .c_uv = {1,0}},
    // back
    { .a = 6, .b = 5, .c = 7, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {0,1}, .c_uv = {1,1}},
    { .a = 6, .b = 7, .c = 8, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {1,1}, .c_uv = {1,0}},
    // left
    { .a = 8, .b = 7, .c = 2, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {0,1}, .c_uv = {1,1}},
    { .a = 8, .b = 2, .c = 1, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {1,1}, .c_uv = {1,0}},
    // top
    { .a = 2, .b = 7, .c = 5, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {0,1}, .c_uv = {1,1}},
    { .a = 2, .b = 5, .c = 3, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {1,1}, .c_uv = {1,0}},
    // bottom
    { .a = 6, .b = 8, .c = 1, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {0,1}, .c_uv = {1,1}},
    { .a = 6, .b = 1, .c = 4, .color = CUBE_MESH_COLOR,
      .a_uv = {0,0}, .b_uv = {1,1}, .c_uv = {1,0}}
}};

struct Mesh
{
    std::vector<vect3_t<float>> vertices;
    std::vector<Face> faces;
    vect3_t<float> rotation{0.0,0.0,0.0};
    vect3_t<float> scale{1.0f,1.0f,1.0f};
    vect3_t<float> translation{0.0f,0.0f,0.0f};
};

void LoadOBJFile(const std::filesystem::path& pathToOBJ,
                 std::vector<vect3_t<float>>& vertexArray,
                 std::vector<Face>& facesArray);

void LoadOBJFileSimplified(const std::filesystem::path& pathToOBJ,
                           std::vector<vect3_t<float>>& vertexArray,
                           std::vector<Face>& facesArray);

#endif //MESH_H