/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include <fbxsdk.h>

#include "Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;
KString outFile;
KString fileName;

// Local function prototypes.
void DisplayContent(KFbxScene* pScene);
void DisplayContent(KFbxNode* pNode);
void DisplayTarget(KFbxNode* pNode);
void DisplayTransformPropagation(KFbxNode* pNode);
void DisplayGeometricTransform(KFbxNode* pNode);
void DisplayMetaData(KFbxScene* pScene);

int main(int argc, char** argv)
{
	fileName = argv[1];
	outFile = argv[2];

    KFbxSdkManager* lSdkManager = NULL;
    KFbxScene* lScene = NULL;
    bool lResult;
     
    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

    // The example can take a FBX file as an argument.
    if(argc > 1)
    {
        printf("\n\nFile: %s\n\n", argv[1]);
        lResult = LoadScene(lSdkManager, lScene, argv[1]);
    }
    else
    {
        lResult = false;
        printf("\n\nUsage: ImportScene <FBX file name>\n\n");
    }

    if(lResult == false)
    {
        printf("\n\nAn error occurred while loading the scene...");
    }
    else 
    {
        // Display the scene.
        DisplayMetaData(lScene);

        printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

        DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

        printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

        DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

        printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

        DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

        printf("\n\n---------\nHierarchy\n---------\n\n");

        DisplayHierarchy(lScene);

  //      printf("\n\n------------\nNode Content\n------------\n\n");

  //      DisplayContent(lScene);

  //      printf("\n\n----\nPose\n----\n\n");

//        DisplayPose(lScene);

        printf("\n\n---------\nAnimation\n---------\n\n");

        DisplayAnimation(lScene);

        //now display generic information

      //  printf("\n\n---------\nGeneric Information\n---------\n\n");
       // Keenan - kill
        //DisplayGenericInfo(lScene);
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager);

    return 0;
}

void DisplayContent(KFbxScene* pScene)
{
    int i;
    KFbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

void DisplayContent(KFbxNode* pNode)
{
    KFbxNodeAttribute::EAttributeType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        case KFbxNodeAttribute::eMARKER:  
   //         DisplayMarker(pNode);
            break;

        case KFbxNodeAttribute::eSKELETON:  
            DisplaySkeleton(pNode);
            break;

        case KFbxNodeAttribute::eMESH:      
            DisplayMesh(pNode);
            break;

        case KFbxNodeAttribute::eNURB:      
            DisplayNurb(pNode);
            break;

        case KFbxNodeAttribute::ePATCH:     
            DisplayPatch(pNode);
            break;

        case KFbxNodeAttribute::eCAMERA:    
            DisplayCamera(pNode);
            break;

        case KFbxNodeAttribute::eLIGHT:     
            DisplayLight(pNode);
            break;

        case KFbxNodeAttribute::eLODGROUP:
            DisplayLodGroup(pNode);
            break;
        }   
    }

//    DisplayUserProperties(pNode);
  //  DisplayTarget(pNode);
   // DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}


void DisplayTarget(KFbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(KFbxNode* pNode)
{
    printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    ERotationOrder lRotationOrder;
    pNode->GetRotationOrder(KFbxNode::eSOURCE_SET, lRotationOrder);

    printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEULER_XYZ: 
        printf("Euler XYZ\n");
        break;
    case eEULER_XZY:
        printf("Euler XZY\n");
        break;
    case eEULER_YZX:
        printf("Euler YZX\n");
        break;
    case eEULER_YXZ:
        printf("Euler YXZ\n");
        break;
    case eEULER_ZXY:
        printf("Euler ZXY\n");
        break;
    case eEULER_ZYX:
        printf("Euler ZYX\n");
        break;
    case eSPHERIC_XYZ:
        printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEULER_XYZ for the rest)
    //
    printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(KFbxNode::eSOURCE_SET) ? "Yes" : "No");


    //
    // Inherit Type
    //
    ETransformInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case eINHERIT_RrSs:
        printf("RrSs\n");
        break;
    case eINHERIT_RSrs:
        printf("RSrs\n");
        break;
    case eINHERIT_Rrs:
        printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(KFbxNode* pNode)
{
    KFbxVector4 lTmpVector;
    //keenan;
    return;
    printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
    printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(KFbxNode::eSOURCE_SET);
    printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(KFbxNode::eSOURCE_SET);
    printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(KFbxScene* pScene)
{
    KFbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        KFbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case KFbxThumbnail::eRGB_24:
                printf("        Format: RGB\n");
                break;
            case KFbxThumbnail::eRGBA_32:
                printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
            case KFbxThumbnail::eNOT_SET:
                printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case KFbxThumbnail::e64x64:
                printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case KFbxThumbnail::e128x128:
                printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

