#include "Write.h"



void Hybris::write(std::ofstream & ofile, File & file)
{
    write(ofile, file.mesh);
    write(ofile, file.material);
    write(ofile, file.skeleton);
}

void Hybris::write(std::ofstream & ofile, Material & material)
{
    write(ofile, material.diffuseMap);
    write(ofile, material.normalMap);
    write(ofile, material.specularMap);
    write(ofile, material.glowMap);
}

void Hybris::write(std::ofstream & ofile, Texture & texture)
{
    write(ofile, texture.extension);
    write(ofile, texture.data);
}

void Hybris::write(std::ofstream & ofile, Mesh & mesh)
{
    write(ofile, mesh.vertices);
}

void Hybris::write(std::ofstream & ofile, Skeleton & skeleton)
{
    write(ofile, skeleton.joints);
    write(ofile, skeleton.animations);
}

void Hybris::write(std::ofstream & ofile, Vertex & vertex)
{
    write(ofile, vertex.position);
    write(ofile, vertex.normal);
    write(ofile, vertex.binormal);
    write(ofile, vertex.tangent);
    write(ofile, vertex.uv);
    write(ofile, vertex.jointIds);
    write(ofile, vertex.jointWeights);
}

void Hybris::write(std::ofstream & ofile, Index & index)
{
    write(ofile, index.vertexId);
}

void Hybris::write(std::ofstream & ofile, Animation & animation)
{
    write(ofile, animation.name);
    write(ofile, animation.duration);
    write(ofile, animation.keyFrames);
}

void Hybris::write(std::ofstream & ofile, KeyFrame & keyFrame)
{
    write(ofile, keyFrame.timeStamp);
    write(ofile, keyFrame.jointTransforms);
}

void Hybris::write(std::ofstream & ofile, JointTransform & jointTransform)
{
    write(ofile, jointTransform.jointId);
    write(ofile, jointTransform.translation);
    write(ofile, jointTransform.rotation);
    write(ofile, jointTransform.scale);
}

void Hybris::write(std::ofstream & ofile, Joint & joint)
{
    write(ofile, joint.id);
    write(ofile, joint.invBindTransform);
}

void Hybris::write(std::ofstream & ofile, Hitbox & data)
{    
    write(ofile, data.position);
    write(ofile, data.rotation);
    write(ofile, data.halfSize);
}

void Hybris::write(std::ofstream & ofile, FileWithHitbox & data)
{
    write(ofile, (File)data);
    write(ofile, data.hitboxes);
}

//void Hybris::write(std::ofstream & ofile, size_t & size)
//{
//    ofile.write((char*)&size, sizeof(size));
//}

void Hybris::write(std::ofstream & ofile, time_t & time)
{
    ofile.write((char*)&time, sizeof(time));
}

void Hybris::write(std::ofstream & ofile, id_t & id)
{
    ofile.write((char*)&id, sizeof(id));
}

void Hybris::write(std::ofstream & ofile, count_t & count)
{
    ofile.write((char*)&count, sizeof(count));
}

void Hybris::write(std::ofstream & ofile, name_t & name)
{
    ofile.write((char*)name, sizeof(name));
}

void Hybris::write(std::ofstream & ofile, vector2_t & vector2)
{
    ofile.write((char*)vector2, sizeof(vector2));
}

void Hybris::write(std::ofstream & ofile, vector3_t & vector3)
{
    ofile.write((char*)vector3, sizeof(vector3));
}

void Hybris::write(std::ofstream & ofile, vector4_t & vector4)
{
    ofile.write((char*)vector4, sizeof(vector4));
}

//void Hybris::write(std::ofstream & ofile, quaternion_t & quaternion)
//{
//    ofile.write((char*)quaternion, sizeof(quaternion));
//}

void Hybris::write(std::ofstream & ofile, matrix4_t & matrix4)
{
    ofile.write((char*)matrix4, sizeof(matrix4));
}

void Hybris::write(std::ofstream & ofile, ivector4_t & ivector4)
{
    ofile.write((char*)ivector4, sizeof(ivector4));
}

void Hybris::write(std::ofstream & ofile, byte_t & byte)
{
    ofile.write((char*)&byte, sizeof(byte));
}
