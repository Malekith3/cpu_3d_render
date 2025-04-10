#ifndef MESH_H
#define MESH_H
#include <array>
#include <vector>

#include "Triangle.h"
#include "filesystem"
#include "Vectors.hpp"


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


constexpr std::array<face_t, N_CUBE_MESH_FACES> cubeMeshFaces = {{
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
    }};

struct Mesh
{
    std::vector<vect3_t<float>> vertices;
    std::vector<face_t> faces;
    vect3_t<float> rotation{0.0,0.0,0.0};
    vect3_t<float> scale{1.0f,1.0f,1.0f};
    vect3_t<float> translation{0.0f,0.0f,0.0f};
};

void LoadOBJFile(const std::filesystem::path& pathToOBJ,
                 std::vector<vect3_t<float>>& vertexArray,
                 std::vector<face_t>& facesArray);

void LoadOBJFileSimplified(const std::filesystem::path& pathToOBJ,
                           std::vector<vect3_t<float>>& vertexArray,
                           std::vector<face_t>& facesArray);

#endif //MESH_H
