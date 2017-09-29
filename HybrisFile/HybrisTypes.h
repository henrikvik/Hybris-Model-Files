#pragma once
#include <memory>
#include <cstdint>
#include <vector>

namespace Hybris
{
    struct Vector2
    {
        float x;
        float y;
    };

    struct Vector3
    {
        float x;
        float y;
        float z;
    };

    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
    };

    struct Mesh
    {
        uint32_t vertexCount;
        std::vector<Vertex> vertices;
    };

    struct Model
    {
        Mesh mesh;
    };
}
