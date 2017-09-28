#pragma once
#include <memory>
#include <cstdint>

namespace Hybris
{
    struct Vertex
    {
        float x;
        float y;
        float z;
    };

    struct Mesh
    {
        uint32_t vertexCount;
        uint32_t uvCount;
        std::unique_ptr<Vertex[]> vertices;
        std::unique_ptr<Vertex[]> uvs;

    };

    struct Model
    {
        Mesh mesh;
    };
}
