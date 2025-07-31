#include "graphics/shapes/inc/Triangle.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <vector>


//I tried to do fancy way with monads ... srsly why still cpp can beat c way to do with string...
void LoadOBJFile(const std::filesystem::path& pathToOBJ, std::vector<vect3_t<float>>& vertexArray, std::vector<Face>& facesArray)
{
    if(!exists(pathToOBJ))
    {
        std::cerr << std::format("File in file path {} does not exist\n", pathToOBJ.string());
        return;
    }

    std::ifstream objFIle(pathToOBJ);

    if (!objFIle.is_open())
    {
        std::cerr << std::format("cant open obj file \n", pathToOBJ.string());
        return;
    }

    std::string line;
    std::regex checkRelStarting(R"(^\s*(v|vt|f|vn)\s)");
    while (std::getline(objFIle, line))
    {
        if (std::regex_search(line, checkRelStarting))
        {
            //Delete start with token
            if (line.starts_with("v "))
            {
                line.erase(0,2);
                auto numbers = line | std::views::split(' ')
                                                    | std::ranges::views::transform([](auto&& range)
                                                      { return std::string(range.begin(), range.end());});


                std::vector<float> vertexBuffer;
                for (auto number : numbers)
                {
                    float stringFloatValue = std::stof(number);
                    vertexBuffer.emplace_back(stringFloatValue);
                }
                vertexArray.emplace_back(vertexBuffer[0], vertexBuffer[1], vertexBuffer[2]);
            }
            else if (line.starts_with("f "))
            {
                //Delete start with token
                line.erase(0,2);
                auto numbers = line
                     | std::views::split(' ')
                     | std::views::transform([](auto&& part)
                     {
                         // Convert the part into a string
                       std::string s(part.begin(), part.end());

                       std::istringstream ss(s);
                       std::string token;

                       std::getline(ss, token, '/');

                       return std::stoi(token);
                     });

                std::vector<float> buffer;
                for (auto number : numbers)
                {
                    buffer.emplace_back(number);
                }
                facesArray.emplace_back(buffer[0], buffer[1], buffer[2]);
            }
            else if (line.starts_with("vt "))
            {
                //Preparation for texture coord
                continue;
            }
            else if (line.starts_with("vn "))
            {
                //Preparation for normals
                continue;
            }
        }
    }
}

void LoadOBJFileSimplified(const std::filesystem::path& pathToOBJ, std::vector<vect3_t<float>>& vertexArray, std::vector<Face>& facesArray)
{
    if (!exists(pathToOBJ))
    {
        std::cerr << std::format("File at path {} does not exist\n", pathToOBJ.string());
        return;
    }

    std::ifstream objFile(pathToOBJ);
    if (!objFile.is_open())
    {
        std::cerr << std::format("Can't open obj file: {}\n", pathToOBJ.string());
        return;
    }

    std::string line;
    std::regex checkRelStarting(R"(^\s*(v|vt|f|vn)\s)");

    std::vector<Texture2d> texturesCoord;

    while (std::getline(objFile, line))
    {
        if (std::regex_search(line, checkRelStarting))
        {
            if (line.starts_with("v "))
            {
                float x, y, z;
                sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                vertexArray.emplace_back(x, y, z);
            }
            else if (line.starts_with("f "))
            {
                std::array<int,3> v{}, t{}, n{};
                sscanf(line.c_str(),
                    "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &v[0], &t[0], &n[0],
                    &v[1], &t[1], &n[1],
                    &v[2], &t[2], &n[2]);

                auto aTextCoord = texturesCoord[t[0] - 1];
                auto bTextCoord = texturesCoord[t[1] - 1];
                auto cTextCoord = texturesCoord[t[2] - 1];
                Face newFace{
                    .a=v[0],
                    .b=v[1],
                    .c=v[2],
                    .a_uv=aTextCoord,
                    .b_uv=bTextCoord,
                    .c_uv=cTextCoord
                };
                facesArray.push_back(std::move(newFace));
            }
            else if (line.starts_with("vt "))
            {
                float uCoord, vCoord;
                sscanf(line.c_str(), "vt %f %f", &uCoord, &vCoord);
                //We need to flip
                vCoord = 1 - vCoord;
                texturesCoord.emplace_back(uCoord, vCoord);
            }
            else if (line.starts_with("vn "))
            {
                // Process normal data (if needed)
                continue;
            }
        }
    }

}


