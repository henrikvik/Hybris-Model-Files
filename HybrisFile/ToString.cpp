#include "ToString.h"

#define SS_SET(ss)\
ss.precision(2);\
ss.setf(std::ios::fixed);

std::string Hybris::toString(File & file)
{
    std::string str;
    str += pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Mesh\":" + toString(file.mesh) + ",\n";
    str += pad.str() + "\"Skeleton\":" + toString(file.skeleton) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(Mesh & mesh)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Vertices\": " + toString(mesh.vertices) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(Skeleton & skeleton)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Joints\": " + toString(skeleton.joints) + ",\n";
    str += pad.str() + "\"Animations\": " + toString(skeleton.animations) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(Vertex & vertex)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Position\": " + toString(vertex.position) + ",\n";
    str += pad.str() + "\"Normal\":   " + toString(vertex.normal) + ",\n";
    str += pad.str() + "\"Binormal\": " + toString(vertex.binormal) + ",\n";
    str += pad.str() + "\"Tangent\":  " + toString(vertex.tangent) + ",\n";
    str += pad.str() + "\"UV\":       " + toString(vertex.uv) + ",\n";
    str += pad.str() + "\"JointIds\": " + toString(vertex.jointIds) + ",\n";
    str += pad.str() + "\"Weights\":  " + toString(vertex.jointWeights) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(Index & index)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Vertex\": " + toString(index.vertexId);
    pad.dec();
    str += pad.str() + "}\n";
    return str;
}

std::string Hybris::toString(Animation & animation)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Name\": " + toString(animation.name) + ",\n";
    str += pad.str() + "\"Duration\": " + toString(animation.duration) + "\n";
    str += pad.str() + "\"KeyFrames\":" + toString(animation.keyFrames) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(KeyFrame & keyFrame)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"TimeStamp\": " + toString(keyFrame.timeStamp) + ",\n";
    str += pad.str() + "\"JointTransforms\": " + toString(keyFrame.jointTransforms) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(JointTransform & jointTransform)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"JointId\":     " + toString(jointTransform.jointId) + ",\n";
    str += pad.str() + "\"Translation\": " + toString(jointTransform.translation) + ",\n";
    str += pad.str() + "\"Rotation\":    " + toString(jointTransform.rotation) + ",\n";
    str += pad.str() + "\"Scale\":       " + toString(jointTransform.scale) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

std::string Hybris::toString(Joint & joint)
{
    std::string str;
    str += "\n" + pad.str() + "{\n";
    pad.inc();
    str += pad.str() + "\"Id\": " + toString(joint.id) + ",\n";
    str += pad.str() + "\"InvBindTransform\":" + toString(joint.invBindTransform) + "\n";
    pad.dec();
    str += pad.str() + "}";
    return str;
}

//std::string Hybris::toString(size_t & size)
//{
//    std::stringstream ss; SS_SET(ss);
//    ss << size;
//    return ss.str();
//}

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
    ss << '"' << name << '"';
    return ss.str();
}

std::string Hybris::toString(vector2_t & vector2)
{
    std::stringstream ss; SS_SET(ss);
    ss << "[" 
       << toString(vector2[0]) << ", " 
       << toString(vector2[1]) << "]";
    return ss.str();
}

std::string Hybris::toString(vector3_t & vector3)
{
    std::stringstream ss; SS_SET(ss);
    ss << "[" 
        << toString(vector3[0]) << ", "
        << toString(vector3[1]) << ", "
        << toString(vector3[2]) << "]";
    return ss.str();
}

std::string Hybris::toString(vector4_t & vector4)
{
    std::stringstream ss; SS_SET(ss);
    ss << "["
       << toString(vector4[0]) << ", " 
       << toString(vector4[1]) << ", " 
       << toString(vector4[2]) << ", " 
       << toString(vector4[3]) << "]";
    return ss.str();
}

std::string Hybris::toString(ivector4_t & ivector4)
{
    std::stringstream ss; SS_SET(ss);
    ss << "["
       << toString(ivector4[0]) << ", " 
       << toString(ivector4[1]) << ", " 
       << toString(ivector4[2]) << ", " 
       << toString(ivector4[3]) << "]";
    return ss.str();
}

std::string Hybris::toString(matrix4_t & matrix4)
{
    std::stringstream ss; SS_SET(ss);
    ss << std::endl;
    pad.inc();
    ss << pad.str() << "[" << toString(matrix4[0]) << "," << std::endl
       << pad.str() << toString(matrix4[1]) << "," << std::endl
       << pad.str() << toString(matrix4[2]) << "," << std::endl 
       << pad.str() << toString(matrix4[3]) << "]";
    pad.dec();
    return ss.str();
}
