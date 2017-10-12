#include "ToString.h"


#define SS_SET(ss)\
ss.precision(2);\
ss.width(5);\
ss.setf(std::ios::fixed);

std::string Hybris::toString(File & file)
{
    std::string str;
    str += "Mesh:\n" + toString(file.mesh, PAD) + "\n";
    str += "Skeleton:\n" + toString(file.skeleton, PAD);
    return str;
}

std::string Hybris::toString(Mesh & mesh, std::string pad)
{
    std::string str;
    str += pad + "Indecies: " + toString(mesh.indices.size) + "\n";
    str += pad + "Vertices: " + toString(mesh.vertices, pad + PAD);
    return str;
}

std::string Hybris::toString(Skeleton & skeleton, std::string pad)
{
    std::string str;
    str += pad + "Joints: " + toString(skeleton.joints, pad + PAD) + "\n";
    str += pad + "Animations: " + toString(skeleton.animations, pad + PAD) + "\n";
    return str;
}

std::string Hybris::toString(Vertex & vertex, std::string pad)
{
    std::string str;
    str += pad + "Position: " + toString(vertex.position) + "\n";
    str += pad + "Normal:   " + toString(vertex.normal) + "\n";
    str += pad + "Binormal: " + toString(vertex.binormal) + "\n";
    str += pad + "Tangent:  " + toString(vertex.tangent) + "\n";
    str += pad + "UV:       " + toString(vertex.uv) + "\n";
    str += pad + "JointIds: " + toString(vertex.jointIds) + "\n";
    str += pad + "Weights:  " + toString(vertex.jointWeights);
    return str;
}

std::string Hybris::toString(Index & index, std::string pad)
{
    std::string str;
    str += pad + "Vertex: " + toString(index.vertexId);
    return str;
}

std::string Hybris::toString(Animation & animation, std::string pad)
{
    std::string str;
    str += pad + "Name: " + toString(animation.name) + "\n";
    str += pad + "KeyFrames:" + toString(animation.keyFrames, pad + PAD);
    return str;
}

std::string Hybris::toString(KeyFrame & keyFrame, std::string pad)
{
    std::string str;
    str += pad + "TimeStamp: " + toString(keyFrame.timeStamp) + "\n";
    str += pad + "JointTransforms: " + toString(keyFrame.jointTransforms, pad + PAD);
    return str;
}

std::string Hybris::toString(JointTransform & jointTransform, std::string pad)
{
    std::string str;
    str += pad + "JointId:     " + toString(jointTransform.jointId) + "\n";
    str += pad + "Translation: " + toString(jointTransform.translation) + "\n";
    str += pad + "Rotation:    " + toString(jointTransform.rotation) + "\n";
    str += pad + "Scale:       " + toString(jointTransform.scale);
    return str;
}

std::string Hybris::toString(Joint & joint, std::string pad)
{
    std::string str;
    str += pad + "Id: " + toString(joint.id) + "\n";
    str += pad + "InvBindTransform:\n" + toString(joint.invBindTransform, pad + PAD);
    return str;
}

std::string Hybris::toString(size_t & size)
{
    std::stringstream ss; SS_SET(ss);
    ss << size;
    return ss.str();
}

std::string Hybris::toString(time_t & time)
{
    std::stringstream ss; SS_SET(ss);
    ss << time;
    return ss.str();
}

std::string Hybris::toString(id_t & id)
{
    std::stringstream ss; SS_SET(ss);
    ss << id;
    return ss.str();
}

std::string Hybris::toString(count_t & count)
{
    std::stringstream ss; SS_SET(ss);
    ss << count;
    return ss.str();
}

std::string Hybris::toString(name_t & name)
{
    std::stringstream ss; SS_SET(ss);
    ss << name;
    return ss.str();
}

std::string Hybris::toString(vector2_t & vector2)
{
    std::stringstream ss; SS_SET(ss);
    ss << toString(vector2[0]) << ", " 
       << toString(vector2[1]);
    return ss.str();
}

std::string Hybris::toString(vector3_t & vector3)
{
    std::stringstream ss; SS_SET(ss);
    ss << toString(vector3[0]) << ", " 
       << toString(vector3[1]) << ", " 
       << toString(vector3[2]);
    return ss.str();
}

std::string Hybris::toString(vector4_t & vector4)
{
    std::stringstream ss; SS_SET(ss);
    ss << toString(vector4[0]) << ", " 
       << toString(vector4[1]) << ", " 
       << toString(vector4[2]) << ", " 
       << toString(vector4[3]);
    return ss.str();
}

std::string Hybris::toString(ivector4_t & ivector4)
{
    std::stringstream ss; SS_SET(ss);
    ss << toString(ivector4[0]) << ", " 
       << toString(ivector4[1]) << ", " 
       << toString(ivector4[2]) << ", " 
       << toString(ivector4[3]);
    return ss.str();
}

std::string Hybris::toString(matrix4_t & matrix4, std::string pad)
{
    std::stringstream ss; SS_SET(ss);
    ss << pad << toString(matrix4[0]) << std::endl 
       << pad << toString(matrix4[1]) << std::endl 
       << pad << toString(matrix4[2]) << std::endl 
       << pad << toString(matrix4[3]);
    return ss.str();
}
