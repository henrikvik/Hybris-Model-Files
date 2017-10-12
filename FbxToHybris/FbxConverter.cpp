#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")

#include <vector>
#include <algorithm>
#include <map>
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

#pragma endregion

#pragma region Help Functions

    template<typename T>
    size_t sizeofv(std::vector<T> vector)
    {
        return sizeof(T) * vector.size();
    }

    template<typename T>
    std::vector<T*> getSrcObjects(FbxScene * scene)
    {
        std::vector<T*> animationStacks;

        for (int i = 0, objectCount = scene->GetSrcObjectCount<T>(); i < objectCount; i++)
        {
            animationStacks.push_back(scene->GetSrcObject<T>(i));
        }

        return animationStacks;
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
    FbxScene * importFbxScene(FbxManager * fbxManager, const char * inFilePath)
    {
        FbxImporter* importer = FbxImporter::Create(fbxManager, "");

        if (!importer->Initialize(inFilePath))
        {
            printf("Call to FbxImporter::Initialize failed.\n");
            printf("Error: %s\n\n", importer->GetStatus().GetErrorString());
            throw;
        }

        FbxScene * scene = FbxScene::Create(fbxManager, "Imported");
        importer->Import(scene);
        importer->Destroy();

        return scene;
    }
#pragma endregion



#pragma region Fbx Animation Import
typedef std::map<FbxNode*, size_t> SkeletonNodes_t;
typedef std::map<size_t, FbxAMatrix> JointTransforms_t;
typedef std::map<size_t, JointTransforms_t> KeyFrames_t;
typedef std::map<std::string, KeyFrames_t> Animations_t;

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
KeyFrames_t     getKeyFrames(SkeletonNodes_t const& skeletonNodes, FbxAnimLayer * animLayer)
{
    KeyFrames_t keyFrames;

    for (auto & node_jointId : skeletonNodes)
    {
        FbxNode * node = node_jointId.first;

        FbxAnimCurve* localTranslationX = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
        FbxAnimCurve* localTranslationY = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
        FbxAnimCurve* localTranslationZ = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

        FbxAnimCurve* localRotationX = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
        FbxAnimCurve* localRotationY = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
        FbxAnimCurve* localRotationZ = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

        FbxAnimCurve* localScaleX = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
        FbxAnimCurve* localScaleY = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
        FbxAnimCurve* localScaleZ = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

        for (size_t i = 0, keyCount = localTranslationX->KeyGetCount(); i < keyCount; i++)
        {
            FbxVector4 localTranslation = { 
                localTranslationX->KeyGetValue(i), 
                localTranslationY->KeyGetValue(i), 
                localTranslationZ->KeyGetValue(i)
            };
            FbxVector4 localRotation = { 
                localRotationX->KeyGetValue(i), 
                localRotationY->KeyGetValue(i), 
                localRotationZ->KeyGetValue(i)
            };
            FbxVector4 localScale = {
                localScaleX->KeyGetValue(i),
                localScaleY->KeyGetValue(i),
                localScaleZ->KeyGetValue(i)
            };

            
            FbxAMatrix localTransform; localTransform.SetTRS(localTranslation, localRotation, localScale);

            FbxAMatrix parentGlobalTransform;

            FbxNode * parent = node->GetParent();
            if (parent->GetSkeleton())
            {
                size_t parentId = skeletonNodes.at(parent);
                parentGlobalTransform = keyFrames[i][parentId];
            }

            size_t jointId = node_jointId.second;
            keyFrames[i][jointId] = parentGlobalTransform * localTransform;
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
            if (strcmp(name, "BaseLayer") != 0)
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
typedef std::map<size_t, std::vector<JointWeight_t>> JointWeights_t;
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
typedef std::vector<Vertex> VertexData_t;
typedef std::vector<size_t> IndexData_t;
typedef std::pair<VertexData_t, IndexData_t> MeshData_t;

MeshNodes_t getMeshNodes(FbxScene * scene)
{
    MeshNodes_t meshNodes;

    uint32_t currentId = 0;
    for (int i = 0, nodeCount = scene->GetNodeCount(); i < nodeCount; i++)
    {
        FbxNode * node = scene->GetNode(i);
        if (node->GetMesh())
        {
            auto ptr_bool = meshNodes.insert({node, currentId});
            currentId += ptr_bool.second;

        }
    }

    return meshNodes;
}

JointWeights_t getJointWeights(FbxMesh * mesh, SkeletonNodes_t & skeletonNodes)
{
    JointWeights_t jointWeights;

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
        std::sort(jointWeight.second.begin(), jointWeight.second.end(), [](JointWeight_t & a, JointWeight_t & b) 
        {
            return a.second > b.second;
        });

        while (jointWeight.second.size() < 4) { jointWeight.second.push_back({}); }
        while (jointWeight.second.size() > 4) { jointWeight.second.pop_back(); }
    }

    return jointWeights;
}

MeshData_t getMeshData(FbxMesh * mesh, SkeletonNodes_t & skeletonNodes)
{
    JointWeights_t jointWeights = getJointWeights(mesh, skeletonNodes);

    std::vector<Vertex> vertexData;
    for (size_t cIndex = 0; cIndex < mesh->GetControlPointsCount(); cIndex++)
    {
        Vertex vertex = {};
        vertex.position = mesh->GetControlPointAt(cIndex);
        vertex.normal   = mesh->GetElementNormal()->GetDirectArray().GetAt(cIndex);
        vertex.binormal = mesh->GetElementBinormal()->GetDirectArray().GetAt(cIndex);
        vertex.tangent  = mesh->GetElementTangent()->GetDirectArray().GetAt(cIndex);
        vertex.uv       = mesh->GetElementUV("map1")->GetDirectArray().GetAt(cIndex);

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
    }

    std::vector<size_t> indexData;
    size_t polyCount = mesh->GetPolygonCount();
    for (size_t polyIndex = 0; polyIndex < polyCount; polyIndex++)
    {
        size_t polySize = mesh->GetPolygonSize(polyIndex);
        if (polySize != 3)
        {
            printf("The mesh is not triangulated!\n");
            system("pause");
            exit(1);
        }

        for (size_t vertexIndex = 0; vertexIndex < polySize; vertexIndex++)
        {
            size_t index = mesh->GetPolygonVertex(polyIndex, vertexIndex);
            indexData.push_back(index);
        }
    }

    return {vertexData, indexData};
}
#pragma endregion



#pragma region Create Hybris File
Hybris::JointTransform          makeHybrisJointTransform(size_t jointId, FbxAMatrix const& transform)
{
    Hybris::JointTransform hJointTransform = {};

    hJointTransform.jointId = jointId;
    fbxcpy(hJointTransform.translation, transform.GetT());
    fbxcpy(hJointTransform.rotation,    transform.GetQ());
    fbxcpy(hJointTransform.scale,       transform.GetS());

    return hJointTransform;
}
Hybris::KeyFrame                makeHybrisKeyFrame(size_t keyFrameId, JointTransforms_t const& jointTransforms)
{
    std::vector<Hybris::JointTransform> hJointTransforms;
    for (auto const& jointTransform : jointTransforms)
    {
        hJointTransforms.push_back(makeHybrisJointTransform(jointTransform.first, jointTransform.second));
    }

    Hybris::KeyFrame hKeyFrame = {};
    hKeyFrame.timeStamp = keyFrameId;
    hKeyFrame.jointTransforms = hJointTransforms;
    return hKeyFrame;
}
Hybris::Animation               makeHybrisAnimation(std::string name, KeyFrames_t const& keyFrames)
{
    std::vector<Hybris::KeyFrame> hKeyFrames;
    for (auto const& keyFrame : keyFrames)
    {
        hKeyFrames.push_back(makeHybrisKeyFrame(keyFrame.first, keyFrame.second));
    }

    Hybris::Animation hAnimation = {};
    strncpy_s(hAnimation.name, name.c_str(), 128);
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
    std::vector<Hybris::Joint> hJoints;
    for (auto & node_jointId : skeletonNodes)
    {
        FbxNode * node = node_jointId.first;
        size_t jointId = node_jointId.second;

        Hybris::Joint hJoint = {};
        hJoint.id = jointId;

        FbxAMatrix invBindTransform = node->EvaluateGlobalTransform().Inverse();
        fbxcpy(hJoint.invBindTransform, invBindTransform);

        hJoints.push_back(hJoint);
    }

    Hybris::List<Hybris::Joint> hJointList = {};
    hJointList = hJoints;
    return hJointList;
}
Hybris::Mesh                    makeHybrisMesh(MeshData_t & meshData)
{
    std::vector<Hybris::Vertex> vertices;
    for (auto & vertexData : meshData.first)
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

    std::vector<Hybris::Index> indices;
    for (auto & indexData : meshData.second)
    {
        Hybris::Index index = {};
        index.vertexId = indexData;

        indices.push_back(index);
    }
    

    Hybris::Mesh hMesh = {};
    hMesh.vertices = vertices;
    hMesh.indices = indices;
    return hMesh;
}

Hybris::Skeleton makeHybrisSkeleton(SkeletonNodes_t & skeletonNodes, Animations_t & animations)
{
    Hybris::Skeleton skeleton = {};
    skeleton.joints = makeHybrisJointList(skeletonNodes);
    skeleton.animations = makeHybrisAnimationList(animations);
    return skeleton;
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
    return hFile;
}
#pragma endregion

int main(int argc, char ** argv)
{
    std::string inFilePath;
    std::string outFilePath;

    switch (argc)
    {
    case 2:
        inFilePath = argv[1];
        outFilePath = argv[1];
        outFilePath.replace(outFilePath.length() - 4, 4, ".hyb");
        break;
    case 3:
        inFilePath = argv[1];
        outFilePath = argv[2];
        break;

    default:
        //printf("Invalid amout of arguments.\n");
        //exit(1);
        inFilePath = "../Resources/BakedTest.fbx";
        outFilePath = "../Resources/BakedTest.hyb";
        break;
    }

    printf("In Path: %s\nOut Path: %s\n", inFilePath.c_str(), outFilePath.c_str());

    FbxManager* fbxManager = FbxManager::Create();
    {
        FbxScene * scene = importFbxScene(fbxManager, inFilePath.c_str());

        Hybris::File hybrisFile = makeHybrisFile(scene);

        std::ofstream ofile(outFilePath.c_str(), std::ios::binary | std::ios::trunc);
        Hybris::write(ofile, hybrisFile);
        ofile.close();

        Hybris::File loadedFile = {};
        std::ifstream ifile(outFilePath.c_str(), std::ios::binary);
        Hybris::read(ifile, loadedFile);
        ofile.close();


        std::string txtFilePath = outFilePath.replace(outFilePath.length() - 4, 4, ".txt");
        std::ofstream strFile(txtFilePath.c_str(), std::ios::trunc);
        strFile << Hybris::toString(loadedFile).c_str();
        strFile.close();

    }
    fbxManager->Destroy();
    return 0;
}