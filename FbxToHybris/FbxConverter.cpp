#include <comdef.h>
#include <filesystem>
namespace fs = std::experimental::filesystem;


#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")

#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

#include <HybrisFile\Hybris.h>
#include <HybrisFile\Read.h>
#include <HybrisFile\Write.h>
#include <HybrisFile\ToString.h>
#pragma comment(lib, "Hybris.lib")


#pragma region Fbx Convert Functions

    void fbxcpy(Hybris::matrix4_t & dest, FbxAMatrix & src)
    {
        for (size_t x = 0; x < 4; x++)
        {
            for (size_t y = 0; y < 4; y++)
            {
                dest[x][y] = src[x][y];
            }
        }
    }

    void fbxcpy(Hybris::matrix4_t & dest, FbxMatrix & src)
    {
        for (size_t x = 0; x < 4; x++)
        {
            for (size_t y = 0; y < 4; y++)
            {
                dest[x][y] = src[x][y];
            }
        }
    }

    void fbxcpy(Hybris::vector4_t & dest, FbxQuaternion & src)
    {
        for (size_t i = 0; i < 4; i++)
        {
            dest[i] = src[i];
        }
    }

    void fbxcpy(Hybris::vector3_t & dest, FbxVector4 & src)
    {
        for (size_t i = 0; i < 3; i++)
        {
            dest[i] = src[i];
        }
    }

    void fbxcpy(Hybris::vector4_t & dest, FbxVector4 & src)
    {
        for (size_t i = 0; i < 4; i++)
        {
            dest[i] = src[i];
        }
    }

    void fbxcpy(Hybris::vector3_t & dest, FbxDouble3& src)
    {
        for (size_t i = 0; i < 3; i++)
        {
            dest[i] = src[i];
        }
    }

    void fbxcpy(Hybris::vector2_t & dest, FbxVector2 & src)
    {
        for (size_t i = 0; i < 2; i++)
        {
            dest[i] = src[i];
        }
    }

    void fbxcpy(Hybris::ivector4_t & dest, size_t src[4])
    {
        for (size_t i = 0; i < 4; i++)
        {
            dest[i] = src[i];
        }
    }

    void fbxcpy(Hybris::vector4_t & dest, float src[4])
    {
        for (size_t i = 0; i < 4; i++)
        {
            dest[i] = src[i];
        }
    }

    void cpy(Hybris::vector4_t & dest, float src[4])
    {
        for (size_t i = 0; i < 4; i++)
        {
            dest[i] = src[i];
        }
    }

    void cpy(Hybris::vector3_t & dest, float src[3])
    {
        for (size_t i = 0; i < 3; i++)
        {
            dest[i] = src[i];
        }
    }



   
#pragma endregion

#pragma region Help Functions

    template<typename T>
    size_t sizeofv(std::vector<T> & vector)
    {
        return sizeof(T) * vector.size();
    }

    template<typename T>
    std::vector<T*> getSrcObjects(FbxObject * object)
    {
        std::vector<T*> animationStacks;

        for (int i = 0, objectCount = object->GetSrcObjectCount<T>(); i < objectCount; i++)
        {
            animationStacks.push_back(object->GetSrcObject<T>(i));
        }

        return animationStacks;
    }

    template<typename T>
    std::vector<T*> getSrcObjects(FbxProperty * property)
    {
        std::vector<T*> objects;

        for (int i = 0, objectCount = property->GetSrcObjectCount<T>(); i < objectCount; i++)
        {
            objects.push_back(property->GetSrcObject<T>(i));
        }

        return objects;
    }

    template<typename T>
    std::vector<T*> getMembers(FbxCollection * collection)
    {
        std::vector<T*> members;

        for (int i = 0, memberCount = collection->GetMemberCount<T>(); i < memberCount; i++)
        {
            members.push_back(collection->GetMember<T>(i));
        }

        return members;
    }

    std::vector<FbxNode*> getNodes(FbxScene * scene)
    {
        std::vector<FbxNode*> nodes;

        for (int i = 0; i < scene->GetNodeCount(); i++)
        {
            nodes.push_back(scene->GetNode(i));
        }

        return nodes;
    }

#define PRINTFVEC2F(vec)\
    {\
        printf("%s%.3f,\t%s%.3f", \
            signbit(vec[0]) ? "" : " ", vec[0],\
            signbit(vec[1]) ? "" : " ", vec[1]\
       );\
    }

#define PRINTFVEC3F(vec)\
    {\
        printf("%s%.3f,\t%s%.3f,\t%s%.3f", \
            signbit(vec[0]) ? "" : " ", vec[0],\
            signbit(vec[1]) ? "" : " ", vec[1],\
            signbit(vec[2]) ? "" : " ", vec[2]\
       );\
    }

#define PRINTFVEC4F(vec)\
    {\
        printf("%s%.3f,\t%s%.3f,\t%s%.3f,\t%s%.3f", \
            signbit(vec[0]) ? "" : " ", vec[0],\
            signbit(vec[1]) ? "" : " ", vec[1],\
            signbit(vec[2]) ? "" : " ", vec[2],\
            signbit(vec[3]) ? "" : " ", vec[3]\
       );\
    }

#define PRINTFVEC4I(vec)\
    {\
        printf("%s%d,   \t%s%d,   \t%s%d,   \t%s%d", \
            vec[0] < 0 ? "" : " ", vec[0],\
            vec[1] < 0 ? "" : " ", vec[1],\
            vec[2] < 0 ? "" : " ", vec[2],\
            vec[3] < 0 ? "" : " ", vec[3]\
       );\
    }

#pragma endregion

#pragma region Fbx Scene Import
    FbxScene * importFbxScene(FbxManager * fbxManager, const wchar_t * inFilePath)
    {
        FbxImporter* importer = FbxImporter::Create(fbxManager, "");

        size_t len = 256;
        char* str = new char[len];
        wcstombs_s(&len, str, len, inFilePath, len);
        if (!importer->Initialize(str))
        {
            printf("Call to FbxImporter::Initialize failed.\n");
            printf("Error: %s\n\n", importer->GetStatus().GetErrorString());
            throw;
        }
        delete[] str;

        FbxScene * scene = FbxScene::Create(fbxManager, "Imported");
        importer->Import(scene);
        importer->Destroy();

        return scene;
    }
#pragma endregion



#pragma region Fbx Animation Import


typedef std::map<FbxNode*, size_t> SkeletonNodes_t;
typedef std::map<size_t, FbxAMatrix> JointTransforms_t;
struct KeyFrame_t
{
    float timeStamp;
    JointTransforms_t jointTransforms;
};
typedef std::map<size_t, KeyFrame_t> KeyFrames_t;
typedef std::map<std::string, KeyFrames_t> Animations_t;

struct OrderByJointId
{
    bool operator()(SkeletonNodes_t::value_type const& a, SkeletonNodes_t::value_type const& b) {
        return a.second < b.second;
    };
};
typedef std::set<SkeletonNodes_t::value_type, OrderByJointId> SortedSkeletonNodes_t;

std::vector<size_t> jointIds;
SkeletonNodes_t getSkeletonNodes(FbxScene * scene)
{
    SkeletonNodes_t skeletonNodes;

    uint32_t currentId = 0;
    for (int i = 0, nodeCount = scene->GetNodeCount(); i < nodeCount; i++)
    {
        FbxNode * node = scene->GetNode(i);
        if (node->GetSkeleton())
        {
            skeletonNodes[node] = currentId++;
        }
    }


    return skeletonNodes;
}

FbxMatrix transpose(FbxAMatrix matrix)
{
    std::swap(matrix.mData[0][3], matrix.mData[3][0]);
    std::swap(matrix.mData[1][3], matrix.mData[3][1]);
    std::swap(matrix.mData[2][3], matrix.mData[3][2]);
    FbxMatrix m(matrix);
    return m.Transpose();
}



KeyFrames_t     getKeyFrames(SkeletonNodes_t const& skeletonNodes, FbxAnimLayer * animLayer)
{
    KeyFrames_t keyFrames;

    SortedSkeletonNodes_t sortedSkeletonNodes(skeletonNodes.begin(), skeletonNodes.end());
    for (auto & node_jointId : sortedSkeletonNodes)
    {
        FbxNode * node = node_jointId.first;
        size_t    jointId = node_jointId.second;
        FbxNode * parent = node->GetParent();

        FbxAnimCurve* localTranslationX = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
        FbxAnimCurve* localTranslationY = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        FbxAnimCurve* localTranslationZ = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);

        FbxAnimCurve* localRotationX = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
        FbxAnimCurve* localRotationY = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        FbxAnimCurve* localRotationZ = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);

        FbxAnimCurve* localScaleX = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
        FbxAnimCurve* localScaleY = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        FbxAnimCurve* localScaleZ = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);


        for (size_t i = 0, keyCount = localTranslationX->KeyGetCount(); i < keyCount; i++)
        {
            FbxDouble3 translation(
                localTranslationX->KeyGetValue(i),
                localTranslationY->KeyGetValue(i),
                localTranslationZ->KeyGetValue(i)
            );
            FbxDouble3 rotation(
                localRotationX->KeyGetValue(i),
                localRotationY->KeyGetValue(i),
                localRotationZ->KeyGetValue(i)
            );
            FbxDouble3 scale(
                localScaleX->KeyGetValue(i),
                localScaleY->KeyGetValue(i),
                localScaleZ->KeyGetValue(i)
            );

            FbxAMatrix animTransform(translation, rotation, scale);

            FbxAMatrix parentTransform;
            if (parent->GetSkeleton())
            {
                size_t parentId = skeletonNodes.at(parent);
                parentTransform = keyFrames.at(i).jointTransforms.at(parentId);
            }

            FbxAMatrix localTransform;
            if (animLayer->BlendMode.Get() == FbxAnimLayer::EBlendMode::eBlendAdditive)
            {
                localTransform = FbxAMatrix(
                    node->LclTranslation.Get(), 
                    node->LclRotation.Get(), 
                    node->LclScaling.Get()
                );
            }
            

            keyFrames[i].timeStamp = localTranslationX->KeyGetTime(i).GetSecondDouble();
            keyFrames[i].jointTransforms[jointId] = parentTransform * localTransform * animTransform;
        }
    }

    return keyFrames;
}
Animations_t    getAnimations(FbxScene * scene, SkeletonNodes_t & skeletonNodes)
{
    Animations_t animations;

    for (FbxAnimStack * animStack : getSrcObjects<FbxAnimStack>(scene))
    {
        for (FbxAnimLayer * animLayer : getMembers<FbxAnimLayer>(animStack))
        {
            const char * name = animLayer->GetName();
            if (strcmp(name, "BaseLayer") != 0 && name[0] != '_')
            {
                animations.insert_or_assign(name, getKeyFrames(skeletonNodes, animLayer));
            }
        }
    }

    return animations;
}
#pragma endregion

#pragma region Fbx Mesh Import
typedef std::map<FbxNode*, size_t> MeshNodes_t;
typedef std::pair<size_t, float> JointWeight_t;
typedef std::vector<std::vector<JointWeight_t>> JointWeights_t;
struct Vertex
{
    FbxVector4 position;
    FbxVector4 normal;
    FbxVector4 binormal;
    FbxVector4 tangent;
    FbxVector2 uv;
    size_t jointIds[4];
    float jointWeights[4];
};
typedef std::vector<Vertex> MeshData_t;

MeshNodes_t getMeshNodes(FbxScene * scene)
{
    MeshNodes_t meshNodes;

    uint32_t currentId = 0;
    for (int i = 0, nodeCount = scene->GetNodeCount(); i < nodeCount; i++)
    {
        FbxNode * node = scene->GetNode(i);
        if (node->GetMesh() && !node->FindProperty("Hitbox").IsValid())
        {
            auto ptr_bool = meshNodes.insert({node, currentId});
            currentId += ptr_bool.second;
        }
    }

    return meshNodes;
}

FbxVector4 getNormal(FbxMesh * mesh, size_t controlPoint)
{
    FbxGeometryElementNormal * normals;
    size_t normalIndex;

    if (mesh->GetElementNormalCount() != 1)
    {
        throw std::runtime_error("Mesh needs to have exactly ONE Normals container.");
    }

    normals = mesh->GetElementNormal(0);

    if (normals->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
    {
        throw std::runtime_error("Normals needs to be mapped per vertex.");
    }

    switch (normals->GetReferenceMode())
    {
    default:
        throw std::runtime_error("Normals needs to be referenced by Direct or IndexToDirect.");
        break;

    case FbxGeometryElement::eIndexToDirect:
        normalIndex = normals->GetIndexArray().GetAt(controlPoint);
        break;

    case FbxGeometryElement::eDirect:
        normalIndex = controlPoint;
        break;
    }

    return normals->GetDirectArray().GetAt(normalIndex);
}



FbxVector4 getBinormal(FbxMesh * mesh, size_t controlPoint)
{
    if (mesh->GetElementBinormalCount() != 1)
    {
        throw std::runtime_error("Mesh needs to have exactly ONE Binormals container.");
    }
    if (mesh->GetElementBinormal(0)->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
    {
        throw std::runtime_error("Binormals needs to be mapped per vertex.");
    }

    FbxGeometryElementBinormal * binormals = mesh->GetElementBinormal(0);
    size_t binormalIndex;

    switch (binormals->GetReferenceMode())
    {
    default:
        throw std::runtime_error("Binormals needs to be referenced by Direct or IndexToDirect.");
        break;

    case FbxGeometryElement::eIndexToDirect:
        binormalIndex = binormals->GetIndexArray().GetAt(controlPoint);
        break;
    case FbxGeometryElement::eDirect:
        binormalIndex = controlPoint;
        break;
    }

    return binormals->GetDirectArray().GetAt(binormalIndex);
}

FbxVector4 getTangent(FbxMesh * mesh, size_t controlPoint)
{
    if (mesh->GetElementTangentCount() != 1)
    {
        throw std::runtime_error("Mesh needs to have exactly ONE Tangents container.");
    }
    if (mesh->GetElementTangent(0)->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
    {
        throw std::runtime_error("Tangents needs to be mapped per vertex.");
    }

    FbxGeometryElementTangent * tangents = mesh->GetElementTangent(0);
    size_t tangentIndex;

    switch (tangents->GetReferenceMode())
    {
    default:
        throw std::runtime_error("Tangents needs to be referenced by Direct or IndexToDirect.");
        break;

    case FbxGeometryElement::eIndexToDirect:
        tangentIndex = tangents->GetIndexArray().GetAt(controlPoint);
        break;
    case FbxGeometryElement::eDirect:
        tangentIndex = controlPoint;
        break;
    }

    return tangents->GetDirectArray().GetAt(tangentIndex);
}

FbxVector2 getUV(FbxMesh * mesh, size_t controlPoint)
{
    FbxGeometryElementUV * uvs;
    size_t uvIndex;

    if (mesh->GetElementUVCount() != 1)
    {
        throw std::runtime_error("Mesh needs to have exactly ONE UVs container.");
    }

    uvs = mesh->GetElementUV(0);

    if (uvs->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
    {
        throw std::runtime_error("UVs needs to be mapped per vertex.");
    }

    switch (uvs->GetReferenceMode())
    {
    default:
        throw std::runtime_error("UVs needs to be referenced by Direct or IndexToDirect.");
        break;

    case FbxGeometryElement::eIndexToDirect:
        uvIndex = uvs->GetIndexArray().GetAt(controlPoint);
        break;
    case FbxGeometryElement::eDirect:
        uvIndex = controlPoint;
        break;
    }

    return uvs->GetDirectArray().GetAt(uvIndex);
}

JointWeights_t getJointWeights(FbxMesh * mesh, SkeletonNodes_t & skeletonNodes)
{
    JointWeights_t jointWeights(mesh->GetControlPointsCount());

    FbxSkin * skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
    if (skin)
    {
        int boneCount = skin->GetClusterCount();
        for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
        {
            FbxCluster * cluster = skin->GetCluster(boneIndex);
            FbxNode * node = cluster->GetLink();

            for (size_t i = 0; i < cluster->GetControlPointIndicesCount(); i++)
            {
                float weight = cluster->GetControlPointWeights()[i];
                if (weight <= FLT_EPSILON) { weight = 0; }

                jointWeights[cluster->GetControlPointIndices()[i]].push_back({
                    skeletonNodes.at(node),
                    weight
                });
            }
        }
    }

    for (auto & jointWeight : jointWeights)
    {
        std::sort(jointWeight.begin(), jointWeight.end(), [](JointWeight_t & a, JointWeight_t & b) 
        {
            return a.second > b.second;
        });

        while (jointWeight.size() < 4) { jointWeight.push_back({}); }
        while (jointWeight.size() > 4) { jointWeight.pop_back(); }
    }

    return jointWeights;
}

MeshData_t getMeshData(FbxMesh * mesh, SkeletonNodes_t & skeletonNodes)
{
    JointWeights_t jointWeights = getJointWeights(mesh, skeletonNodes);
    std::vector<Vertex> vertexData;
    

    size_t vertexIndex = 0;
    for (size_t polyIndex = 0; polyIndex <  mesh->GetPolygonCount(); polyIndex++)
    {
        if (mesh->GetPolygonSize(polyIndex) != 3)
        {
            throw std::runtime_error("Mesh is not triangulated!");
        }

        for (size_t polyVert = 0; polyVert < 3; polyVert++)
        {
            bool unmapped;
            size_t cIndex = mesh->GetPolygonVertex(polyIndex, polyVert);

            Vertex vertex = {};
            vertex.position = mesh->GetControlPointAt(cIndex);
            mesh->GetPolygonVertexNormal(polyIndex, polyVert, vertex.normal);
            mesh->GetPolygonVertexUV(polyIndex, polyVert, "map1", vertex.uv, unmapped);
            vertex.uv[1] = 1 - vertex.uv[1]; // Flip Y
            
            vertex.binormal = getBinormal(mesh, vertexIndex);
            vertex.tangent  = getTangent(mesh, vertexIndex);

            float jointWeightsSum = 
                jointWeights[cIndex][0].second +
                jointWeights[cIndex][1].second +
                jointWeights[cIndex][2].second +
                jointWeights[cIndex][3].second;

            for (size_t i = 0; i < 4; i++)
            {
                vertex.jointIds[i]     = jointWeights[cIndex][i].first;
                vertex.jointWeights[i] = jointWeights[cIndex][i].second / jointWeightsSum;
            }

            vertexData.push_back(vertex);
            vertexIndex++;
        }
    }

    return vertexData;
}
#pragma endregion

#pragma region Create Hybris File
Hybris::JointTransform          makeHybrisJointTransform(size_t jointId, FbxAMatrix const& transform)
{
    Hybris::JointTransform hJointTransform = {};

    //glm::vec3 scale;
    //glm::quat rotation;
    //glm::vec3 translation;
    //glm::vec3 skew;
    //glm::vec4 perspective;

    //glm::decompose(transform, scale, rotation, translation, skew, perspective);

    hJointTransform.jointId = jointId;
    fbxcpy(hJointTransform.translation, transform.GetT());
    fbxcpy(hJointTransform.rotation,    transform.GetQ());
    fbxcpy(hJointTransform.scale,       transform.GetS());
    
    return hJointTransform;
}
Hybris::KeyFrame                makeHybrisKeyFrame(float timeStamp, JointTransforms_t const& jointTransforms)
{
    std::vector<Hybris::JointTransform> hJointTransforms;
    for (auto const& jointTransform : jointTransforms)
    {
        hJointTransforms.push_back(makeHybrisJointTransform(jointTransform.first, jointTransform.second));
    }

    Hybris::KeyFrame hKeyFrame = {};
    hKeyFrame.timeStamp = timeStamp;
    hKeyFrame.jointTransforms = hJointTransforms;
    return hKeyFrame;
}
Hybris::Animation               makeHybrisAnimation(std::string name, KeyFrames_t const& keyFrames)
{
    std::vector<Hybris::KeyFrame> hKeyFrames;
    for (auto const& keyFrame : keyFrames)
    {
        hKeyFrames.push_back(makeHybrisKeyFrame(keyFrame.second.timeStamp, keyFrame.second.jointTransforms));
    }

    Hybris::Animation hAnimation = {};
    strncpy_s(hAnimation.name, name.c_str(), 128);
    hAnimation.duration = (*--keyFrames.end()).second.timeStamp;
    hAnimation.keyFrames = hKeyFrames;
   
    return hAnimation;
}
Hybris::List<Hybris::Animation> makeHybrisAnimationList(Animations_t animations)
{
    std::vector<Hybris::Animation> hAnimations;
    for (auto const& animation : animations)
    {
        hAnimations.push_back(makeHybrisAnimation(animation.first, animation.second));
    }

    Hybris::List<Hybris::Animation> hAnimationList = {};
    hAnimationList = hAnimations;
    return hAnimationList;
}
Hybris::List<Hybris::Joint>     makeHybrisJointList(SkeletonNodes_t & skeletonNodes)
{
    std::map <FbxNode*, FbxAMatrix> globalTransforms;
    std::vector<Hybris::Joint> hJoints;

    for (auto & node_jointId : SortedSkeletonNodes_t(skeletonNodes.begin(), skeletonNodes.end()))
    {
        FbxNode * node = node_jointId.first;
        size_t jointId = node_jointId.second;

        Hybris::Joint hJoint = {};
        hJoint.id = jointId;

        FbxAMatrix localTransform(
            node->LclTranslation.Get(), 
            node->LclRotation.Get(), 
            node->LclScaling.Get()
        );

        FbxAMatrix parentGlobalTransform;
        FbxNode * parent = node->GetParent();
        if (parent->GetSkeleton())
        {
            parentGlobalTransform = globalTransforms.at(parent);
        }

        globalTransforms[node] = parentGlobalTransform * localTransform;
        fbxcpy(hJoint.invBindTransform, globalTransforms.at(node).Inverse());

        hJoints.push_back(hJoint);
    }

    Hybris::List<Hybris::Joint> hJointList = {};
    hJointList = hJoints;
    return hJointList;
}
Hybris::Mesh                    makeHybrisMesh(MeshData_t & meshData)
{
    std::vector<Hybris::Vertex> vertices;
    for (auto & vertexData : meshData)
    {
        Hybris::Vertex vertex = {};
        fbxcpy(vertex.position     , vertexData.position    );
        fbxcpy(vertex.normal       , vertexData.normal      );
        fbxcpy(vertex.binormal     , vertexData.binormal    );
        fbxcpy(vertex.tangent      , vertexData.tangent     );
        fbxcpy(vertex.uv           , vertexData.uv          );
        fbxcpy(vertex.jointIds     , vertexData.jointIds    );
        fbxcpy(vertex.jointWeights , vertexData.jointWeights);
        
        vertices.push_back(vertex);
    }

    Hybris::Mesh hMesh = {};
    hMesh.vertices = vertices;
    return hMesh;
}

Hybris::Skeleton makeHybrisSkeleton(SkeletonNodes_t & skeletonNodes, Animations_t & animations)
{
    Hybris::Skeleton skeleton = {};
    skeleton.joints = makeHybrisJointList(skeletonNodes);
    skeleton.animations = makeHybrisAnimationList(animations);
    return skeleton;
}

Hybris::Material makeHybrisMaterial(FbxMesh* mesh)
{
    if (mesh->GetNode()->GetMaterialCount() != 1)
        throw std::runtime_error("Model needs to have exactly one material.");
    FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(0);

    printf("Material %s\n", material->GetName());
    Hybris::Material hmaterial;


    FbxProperty prop = material->GetFirstProperty();
    while (prop.IsValid())
    {
        auto textures = getSrcObjects<FbxTexture>(&prop);
        if (textures.size() > 0)
        {
            std::string name = prop.GetName();


            printf("  %s\n",name.c_str());

            if (textures.size() > 1)
            {
                throw std::runtime_error("Materials can max have one texture per property!");
            }

            FbxFileTexture* file = (FbxFileTexture*)textures[0];
            fs::path filePath(file->GetFileName());

            size_t fileSize = fs::file_size(filePath);
            std::vector<Hybris::byte_t> fileData(fileSize);

            std::basic_ifstream<Hybris::byte_t> infile(filePath, std::ios::binary);

            printf("    %S is %d bytes, loading... ", filePath.filename().c_str(), fileSize);
            infile.read(fileData.data(), fileSize);
            infile.close();
            printf("done.\n");
            
            Hybris::Texture texture = {};
            texture.data = fileData;
            _bstr_t cstr(filePath.extension().c_str());
            strcpy_s(texture.extension, cstr);

            if (name == FbxSurfaceMaterial::sDiffuse) 
            {
                hmaterial.diffuseMap = texture;
            }
            else if (name == FbxSurfaceMaterial::sNormalMap)
            {
                hmaterial.normalMap = texture;
            }
            else if (name == FbxSurfaceMaterial::sSpecular)
            {
                hmaterial.specularMap = texture;
            }
            else if (name == FbxSurfaceMaterial::sEmissive)
            {
                hmaterial.glowMap = texture;
            }
        }

        prop = material->GetNextProperty(prop);
    }

    return hmaterial;
}

Hybris::File makeHybrisFile(FbxScene * scene)
{
    SkeletonNodes_t skeletonNodes = getSkeletonNodes(scene);
    MeshNodes_t meshNodes = getMeshNodes(scene);

    if (meshNodes.size() != 1) { throw "File has to have exactly 1 mesh."; }
    FbxMesh * mesh = meshNodes.begin()->first->GetMesh();

    Animations_t animations = getAnimations(scene, skeletonNodes);
    MeshData_t meshData = getMeshData(mesh, skeletonNodes);

    Hybris::File hFile = {};
    hFile.skeleton = makeHybrisSkeleton(skeletonNodes, animations);
    hFile.mesh = makeHybrisMesh(meshData);
    hFile.material = makeHybrisMaterial(mesh);
    return hFile;
}

Hybris::List<Hybris::Hitbox> getHitboxes(FbxScene * scene)
{
    auto toHitbox = [](FbxNode * node) -> Hybris::Hitbox
    {
        FbxVector4 position = node->LclTranslation.Get();
        FbxQuaternion rotation; rotation.ComposeSphericalXYZ(node->LclRotation.Get());
        FbxVector4 halfSize = node->LclScaling.Get(); 
        halfSize *= 0.5;

        Hybris::Hitbox hitbox;
        fbxcpy(hitbox.position, position);
        fbxcpy(hitbox.rotation, rotation);
        fbxcpy(hitbox.halfSize, halfSize);
        return hitbox;
    };
    std::vector<Hybris::Hitbox> hitboxes;

    for (FbxNode * node : getNodes(scene))
    {
        if (node->FindProperty("Hitbox").IsValid())
            hitboxes.push_back(toHitbox(node));
    }
    
    Hybris::List<Hybris::Hitbox> list; list = hitboxes;
    return list;
}

Hybris::FileWithHitbox makeHybrisFileWithHitbox(FbxScene * scene)
{
    SkeletonNodes_t skeletonNodes = getSkeletonNodes(scene);
    MeshNodes_t meshNodes = getMeshNodes(scene);

    if (meshNodes.size() != 1) { throw "File has to have exactly 1 mesh."; }
    FbxMesh * mesh = meshNodes.begin()->first->GetMesh();

    Animations_t animations = getAnimations(scene, skeletonNodes);
    MeshData_t meshData = getMeshData(mesh, skeletonNodes);

    Hybris::FileWithHitbox hFile = {};
    hFile.skeleton = makeHybrisSkeleton(skeletonNodes, animations);
    hFile.mesh = makeHybrisMesh(meshData);
    hFile.material = makeHybrisMaterial(mesh);
    hFile.hitboxes = getHitboxes(scene);

    return hFile;
}
#pragma endregion

int main(int argc, char ** argv)
{
    fs::path inFilePath;
    fs::path outFilePath;

    switch (argc)
    {
    case 2:
        inFilePath = argv[1];
        outFilePath = argv[1];
        outFilePath.replace_extension("hyb");
        break;

    case 3:
        inFilePath = argv[1];
        outFilePath = argv[2];
        break;

    default:
        inFilePath  = R"(C:\Users\h\source\repos\Hybris-Model-Files\Resources\HouseV4.fbx)";
        outFilePath = inFilePath;
        outFilePath.replace_extension("hyb");
        break;
    }

    printf("In Path: %S\nOut Path: %S\n", inFilePath.c_str(), outFilePath.c_str());

    FbxManager* fbxManager = FbxManager::Create();
    {
        FbxScene * scene = importFbxScene(fbxManager, inFilePath.c_str());

        try
        {
            Hybris::FileWithHitbox hybrisFile = makeHybrisFileWithHitbox(scene);

            std::ofstream ofile(outFilePath.c_str(), std::ios::binary | std::ios::trunc);
            Hybris::write(ofile, hybrisFile);
            ofile.close();

            Hybris::File loadedFile = {};
            std::ifstream ifile(outFilePath.c_str(), std::ios::binary);
            Hybris::read(ifile, loadedFile);
            ofile.close();

            fs::path jsonPath = outFilePath;
            jsonPath.replace_extension("json");

            std::ofstream strFile(jsonPath.c_str(), std::ios::trunc);
            strFile << Hybris::toString(loadedFile).c_str();
            strFile.close();
        }
        catch (std::runtime_error * e)
        {
            printf("Conversion failed: %s\n", e->what());
            system("pause");
        }
        catch (const char * e)
        {
            printf("Conversion failed: %s\n", e);
            system("pause");
        }
    }
    fbxManager->Destroy();
    fs::remove_all(inFilePath.replace_extension("fbm"));

    return 0;
}