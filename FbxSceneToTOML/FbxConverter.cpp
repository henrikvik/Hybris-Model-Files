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
#include <iostream>

#include <HybrisFile\Hybris.h>
#include <HybrisFile\Read.h>
#include <HybrisFile\Write.h>
#include <HybrisFile\ToString.h>
#pragma comment(lib, "Hybris.lib")

#include <toml\toml.h>


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

struct Instance
{
    std::string name;
    std::string modelName;
    std::string modelType;
    FbxVector4 translation;
    FbxVector4 rotation;
    FbxVector4 scale;

    toml::Table toToml()
    {
        static FbxVector4 translationDefault(0,0,0,0);
        static FbxVector4 rotationDefault(0,0,0,1);
        static FbxVector4 scaleDefault(1,1,1,0);
        toml::Table table;

        table["name"] = toml::Value(name);
        table["model"] = toml::Value(modelName);
        //table["type"] = toml::Value(modelType);
        if (translation != translationDefault) table["translation"] = toml::Value(toml::Array({translation[0],translation[1],translation[2]}));
        if (rotation != rotationDefault) table["rotation"] = toml::Value(toml::Array({rotation[0],rotation[1],rotation[2],rotation[3]}));
        if (scale != scaleDefault) table["scale"] = toml::Value(toml::Array({scale[0],scale[1],scale[2]}));
        
        return table;
    }

    friend std::ostream & operator<<(std::ostream & stream, Instance const& instance)
    {
        stream << instance.modelName << ", " 
               << instance.modelType << ", ";

        stream << "(" 
            << instance.translation[0] << ", "
            << instance.translation[1] << ", "
            << instance.translation[2] 
        << "), ";

        stream << "(" 
            << instance.rotation[0] << ", "
            << instance.rotation[1] << ", "
            << instance.rotation[2] << ", " 
            << instance.rotation[3]
        << "), ";

        stream << "(" 
            << instance.scale[0] << ", "
            << instance.scale[1] << ", "
            << instance.scale[2]
        << ")";

        return stream;
    }
};

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
        //printf("Invalid amout of arguments.\n");
        //exit(1);
        inFilePath  = R"(C:\Users\h\source\repos\Hybris-Model-Files\Resources\AnimatedSummonUnit.fbx)";
        outFilePath = inFilePath;
        outFilePath.replace_extension("hyb");
        break;
    }

    printf("In Path: %S\nOut Path: %S\n", inFilePath.c_str(), outFilePath.c_str());

    try
    {
        FbxManager* fbxManager = FbxManager::Create();
        {
            FbxScene * scene = importFbxScene(fbxManager, inFilePath.c_str());
            toml::Value root((toml::Table()));
            for (int i = 0; i < scene->GetNodeCount(); i++)
            {
                FbxNode * node = scene->GetNode(i);
                FbxProperty modelNameProp = node->FindProperty("ModelName");
                FbxProperty modelTypeProp = node->FindProperty("ModelType");
                
                if (modelNameProp.IsValid() && modelTypeProp.IsValid())
                {
                    Instance instance;
                    instance.name = node->GetName();
                    instance.modelName = modelNameProp.Get<FbxString>();
                    instance.modelType = modelTypeProp.Get<FbxString>();

                    //for (char & c : instance.modelName) { c = std::tolower(c); }
                    //for (char & c : instance.modelType) { c = std::tolower(c); }

                    FbxQuaternion rotation;rotation.ComposeSphericalXYZ(node->LclRotation.Get());

                    instance.translation = node->LclTranslation.Get();
                    instance.rotation = rotation.mData;
                    instance.scale = node->LclScaling.Get();

                    root[instance.modelType].push(instance.toToml());                    
                }
            } 


            std::ofstream outFile(outFilePath.c_str());
            root.write(&outFile);
            outFile.close();
        }
        fbxManager->Destroy();
    }
    catch (std::runtime_error * e)
    {
        printf("Conversion failed: %s\n", e->what());
        system("pause");
    }

    return 0;
}