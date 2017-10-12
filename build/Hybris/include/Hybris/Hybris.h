#pragma once
#include "Typedefs.h"
#include "List.h"
#include "IO.h"

namespace Hybris
{
    struct Joint
    {
        id_t      id;
        matrix4_t invBindTransform;
    };

    struct JointTransform
    {
        id_t         jointId;
        vector3_t    translation;
        vector4_t    rotation;
        vector3_t    scale;
    };

    struct KeyFrame
    {
        time_t timeStamp;
        List<JointTransform> jointTransforms;
    };

    struct Animation
    {
        name_t name;
        List<KeyFrame> keyFrames;
    };

    struct Index
    {
        id_t vertexId;
    };

    struct Vertex
    {
        vector3_t  position;
        vector3_t  normal;
        vector3_t  binormal;
        vector3_t  tangent;
        vector2_t  uv;
        ivector4_t jointIds;
        vector4_t  jointWeights;
    };

    struct Mesh
    {
        List<Index>  indices;
        List<Vertex> vertices;
    };

    struct Skeleton
    {
        List<Joint> joints;
        List<Animation> animations;
    };

    struct File
    {
        Mesh mesh;
        Skeleton skeleton;
    };
}
