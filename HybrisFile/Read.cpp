#include "Read.h"

void Hybris::read(std::ifstream & ifile, File & file)
{
    read(ifile, file.mesh);
    read(ifile, file.material);
    read(ifile, file.skeleton);
}

void Hybris::read(std::ifstream & ifile, Material & material)
{
    read(ifile, material.diffuseMap);
    read(ifile, material.normalMap);
    read(ifile, material.specularMap);
    read(ifile, material.glowMap);
}

void Hybris::read(std::ifstream & ifile, Texture & texture)
{
    read(ifile, texture.extension);
    read(ifile, texture.data);
}

void Hybris::read(std::ifstream & ifile, Mesh & mesh)
{
    read(ifile, mesh.vertices);
}

void Hybris::read(std::ifstream & ifile, Skeleton & skeleton)
{
    read(ifile, skeleton.joints);
    read(ifile, skeleton.animations);
}

void Hybris::read(std::ifstream & ifile, Vertex & vertex)
{
    read(ifile, vertex.position);
    read(ifile, vertex.normal);
    read(ifile, vertex.binormal);
    read(ifile, vertex.tangent);
    read(ifile, vertex.uv);
    read(ifile, vertex.jointIds);
    read(ifile, vertex.jointWeights);
}

void Hybris::read(std::ifstream & ifile, Index & index)
{
    read(ifile, index.vertexId);
}

void Hybris::read(std::ifstream & ifile, Animation & animation)
{
    read(ifile, animation.name);
    read(ifile, animation.duration);
    read(ifile, animation.keyFrames);
}

void Hybris::read(std::ifstream & ifile, KeyFrame & keyFrame)
{
    read(ifile, keyFrame.timeStamp);
    read(ifile, keyFrame.jointTransforms);
}

void Hybris::read(std::ifstream & ifile, JointTransform & jointTransform)
{
    read(ifile, jointTransform.jointId);
    read(ifile, jointTransform.translation);
    read(ifile, jointTransform.rotation);
    read(ifile, jointTransform.scale);
}

void Hybris::read(std::ifstream & ifile, Joint & joint)
{
    read(ifile, joint.id);
    read(ifile, joint.invBindTransform);
}

void Hybris::read(std::ifstream & ifile, size_t & size)
{
    ifile.read((char*)&size, sizeof(size));
}

void Hybris::read(std::ifstream & ifile, time_t & time)
{
    ifile.read((char*)&time, sizeof(time));
}

void Hybris::read(std::ifstream & ifile, id_t & id)
{
    ifile.read((char*)&id, sizeof(id));
}

void Hybris::read(std::ifstream & ifile, count_t & count)
{
    ifile.read((char*)&count, sizeof(count));
}

void Hybris::read(std::ifstream & ifile, name_t & name)
{
    ifile.read((char*)name, sizeof(name));
}

void Hybris::read(std::ifstream & ifile, vector2_t & vector2)
{
    ifile.read((char*)vector2, sizeof(vector2));
}

void Hybris::read(std::ifstream & ifile, vector3_t & vector3)
{
    ifile.read((char*)vector3, sizeof(vector3));
}

void Hybris::read(std::ifstream & ifile, vector4_t & vector4)
{
    ifile.read((char*)vector4, sizeof(vector4));
}

//void Hybris::read(std::ifstream & ifile, quaternion_t & quaternion)
//{
//    ifile.read((char*)quaternion, sizeof(quaternion));
//}

void Hybris::read(std::ifstream & ifile, matrix4_t & matrix4)
{
    ifile.read((char*)matrix4, sizeof(matrix4));
}

void Hybris::read(std::ifstream & ifile, ivector4_t & ivector4)
{
    ifile.read((char*)ivector4, sizeof(ivector4));
}

void Hybris::read(std::ifstream & ifile, byte_t & byte)
{
    ifile.read((char*)&byte, sizeof(byte));
}
