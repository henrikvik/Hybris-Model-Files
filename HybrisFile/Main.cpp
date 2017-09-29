#include <fbxsdk.h>



/**
* Print a node, its attributes, and all its children recursively.
*/

size_t numTabs = 0;
void PrintTabs()
{
    for (size_t i = 0; i < numTabs; i++)
    {
        printf("\t");
    }
}

void Display3DVector(const char * prefix, FbxVector4 v)
{
    printf("%s%f,%f,%f\n", prefix, v.mData[0], v.mData[1], v.mData[3]);
}

void DisplayControlsPoints(FbxMesh* pMesh)
{
    int i, lControlPointsCount = pMesh->GetControlPointsCount();
    FbxVector4* lControlPoints = pMesh->GetControlPoints();

    printf("    Control Points");

    for (i = 0; i < lControlPointsCount; i++)
    {
        printf("        Control Point %d\n", i);
        Display3DVector("            Coordinates: ", lControlPoints[i]);

        for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
        {
            FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal( j);
            if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
            {
                char header[100];
                FBXSDK_sprintf(header, 100, "            Normal Vector: "); 
                if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
                    Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
            }
        }
    }

    printf("\n");
}

void DisplayMesh(FbxNode * node)
{
    FbxMesh * mesh = (FbxMesh*) node->GetNodeAttribute();
//    DisplayControlsPoints(mesh);
}

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) { 
    switch(type) { 
    case FbxNodeAttribute::eUnknown: return "unidentified"; 
    case FbxNodeAttribute::eNull: return "null"; 
    case FbxNodeAttribute::eMarker: return "marker"; 
    case FbxNodeAttribute::eSkeleton: return "skeleton"; 
    case FbxNodeAttribute::eMesh: return "mesh"; 
    case FbxNodeAttribute::eNurbs: return "nurbs"; 
    case FbxNodeAttribute::ePatch: return "patch"; 
    case FbxNodeAttribute::eCamera: return "camera"; 
    case FbxNodeAttribute::eCameraStereo: return "stereo"; 
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
    case FbxNodeAttribute::eLight: return "light"; 
    case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
    case FbxNodeAttribute::eOpticalMarker: return "marker"; 
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
    case FbxNodeAttribute::eBoundary: return "boundary"; 
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
    case FbxNodeAttribute::eShape: return "shape"; 
    case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
    case FbxNodeAttribute::eSubDiv: return "subdiv"; 
    default: return "unknown"; 
    } 
}

void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if(!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get(); 
    FbxDouble3 rotation = pNode->LclRotation.Get(); 
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n", 
        nodeName, 
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    DisplayMesh(pNode);

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}


int main(int argc, char ** argv)
{
    const char * filePath = "../Resources/AnimationSetCubeNo.fbx";

    FbxManager* fbxManager = FbxManager::Create();
    FbxIOSettings * ios = FbxIOSettings::Create(fbxManager, IOSROOT);

    ios->SetBoolProp( IMP_FBX_MODEL_COUNT              , true );
    ios->SetBoolProp( IMP_FBX_DEVICE_COUNT             , true );
    ios->SetBoolProp( IMP_FBX_CHARACTER_COUNT          , true );
    ios->SetBoolProp( IMP_FBX_ACTOR_COUNT              , true );
    ios->SetBoolProp( IMP_FBX_CONSTRAINT_COUNT         , true );
    ios->SetBoolProp( IMP_FBX_MEDIA_COUNT              , true );

    ios->SetBoolProp( IMP_FBX_TEMPLATE                 , true );
    ios->SetBoolProp( IMP_FBX_PIVOT                    , true );
    ios->SetBoolProp( IMP_FBX_GLOBAL_SETTINGS          , true );
    ios->SetBoolProp( IMP_FBX_CHARACTER                , true );
    ios->SetBoolProp( IMP_FBX_CONSTRAINT               , true );
    ios->SetBoolProp( IMP_FBX_MERGE_LAYER_AND_TIMEWARP , true );
    ios->SetBoolProp( IMP_FBX_GOBO                     , true );
    ios->SetBoolProp( IMP_FBX_SHAPE                    , true );
    ios->SetBoolProp( IMP_FBX_LINK                     , true );
    ios->SetBoolProp( IMP_FBX_MATERIAL                 , true );
    ios->SetBoolProp( IMP_FBX_TEXTURE                  , true );
    ios->SetBoolProp( IMP_FBX_MODEL                    , true );
    ios->SetBoolProp( IMP_FBX_ANIMATION                , true );
    ios->SetBoolProp( IMP_FBX_PASSWORD                 , true );
    ios->SetBoolProp( IMP_FBX_PASSWORD_ENABLE          , true );
    ios->SetBoolProp( IMP_FBX_CURRENT_TAKE_NAME        , true );
    ios->SetBoolProp( IMP_FBX_EXTRACT_EMBEDDED_DATA    , true );

    fbxManager->SetIOSettings(ios);

    FbxImporter* importer = FbxImporter::Create(fbxManager, "");

    if (!importer->Initialize(filePath, -1, fbxManager->GetIOSettings()))
    {
        printf("Call to FbxImporter::Initialize failed.\n");
        printf("Error: %s\n\n", importer->GetStatus().GetErrorString());
        throw;
    }

    FbxScene * scene = FbxScene::Create(fbxManager, "Imported");
    importer->Import(scene);
    importer->Destroy();

    FbxNode* lRootNode = scene->GetRootNode();
    if(lRootNode) 
    {
        for(int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintNode(lRootNode->GetChild(i));
    }



    int animactionStackCount = scene->GetSrcObjectCount<FbxAnimStack>();
    printf("%d amimations\n", animactionStackCount);


    for (int i = 0; i < animactionStackCount; i++)
    {
        FbxAnimStack * a = scene->GetSrcObject<FbxAnimStack>(i);
        printf("\t%s\n", a->GetName());

        int animationLayerCount = a->GetMemberCount<FbxAnimLayer>();
        printf("\tanimation layers %d\n", animationLayerCount);




        
    }



    fbxManager->Destroy();
    system("pause");
    return 0;
}