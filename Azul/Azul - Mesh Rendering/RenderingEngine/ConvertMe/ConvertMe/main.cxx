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

#include <vector>
#include <algorithm>
#include <assert.h>
#include "File.h"
#include "ModelData.h"

std::vector<FBX_Vertex> FBX_vert;
std::vector<FBX_Vertex_vsn> FBX_vert_fat;
std::vector<FBX_Trilist> FBX_tri;
std::vector<VBO_Trilist> VBO_tri;
std::vector<FBX_Vertex_vsn> FBX_vert_fat_sort;
std::vector<FBX_Vertex_vsn> VBO_vert;
KFbxSdkManager* lSdkManager ;

char * texture;
char * inFile;
char * outFile;
char fileName[20];
bool incTexture = true;

// Local function prototypes.
void DisplayContent(KFbxScene* pScene);
void DisplayContent(KFbxNode* pNode);
void DisplayTarget(KFbxNode* pNode);
void DisplayTransformPropagation(KFbxNode* pNode);
void DisplayGeometricTransform(KFbxNode* pNode);
void DisplayMetaData(KFbxScene* pScene);


//////////////////////////////////CDM File Functions///////////////////////////////////////
void WriteCDMheader()
{
	CDM_Header * header = new CDM_Header();
	header->incTex = incTexture;

	FileHandle fh;
	FileError  ferror;

	ferror = File::open(fh, "hdr.bin", FILE_WRITE);

	assert(ferror == FILE_SUCCESS);

	ferror = File::write(fh,header, sizeof(CDM_Header));

	assert(ferror == FILE_SUCCESS);
	
	ferror = File::close(fh);
	delete header;

	system("vodka hdr.bin ahdr.blo -t VERTS_TYPE -n VBOVerts");

}
void WriteVBOstoFile()
{
	FileHandle fh;
	FileError  ferror;
	int size = (int)VBO_vert.size();

	ferror = File::open(fh, "ver.bin", FILE_WRITE);

	assert(ferror == FILE_SUCCESS);

	FBX_Vertex_vsn * tempWrite;

	for (int i=0; i<size; i++)
	{
		tempWrite = new FBX_Vertex_vsn(VBO_vert[i]);

		ferror = File::write(fh, tempWrite, sizeof(FBX_Vertex_vsn));
	}

	ferror = File::close(fh);
	delete tempWrite ;

	system("vodka ver.bin bver.blo -t VERTS_TYPE -n VBOVerts");

	
}
void WriteTristoFile()
{
	FileHandle fh;
	FileError  ferror;
	int size = (int)VBO_tri.size();

	ferror = File::open(fh, "tri.bin", FILE_WRITE);

	assert(ferror == FILE_SUCCESS);

	VBO_Trilist * tempWrite;

	for (int i=0; i<size; i++)
	{
		 tempWrite = new VBO_Trilist (VBO_tri[i]);
		 ferror = File::write(fh, tempWrite , sizeof(VBO_Trilist));
	}

	ferror = File::close(fh);
	delete tempWrite;

	system("vodka tri.bin ctri.blo -t VERTS_TYPE -n VBOTri");
}
void WriteTexturetoFile()
{
	FileHandle fh;
	FileError  ferror;
	int size = 0;
	char * buffer;
	TEX_Header * textureHeader = new TEX_Header();
	ferror = File::open(fh, texture, FILE_READ);

	if (ferror == FILE_SUCCESS)
	{
		ferror = File::seek(fh,FILE_SEEK_END,0);
		ferror = File::tell(fh,size);
		ferror = File::seek(fh,FILE_SEEK_BEGIN,0);
		buffer =  (char *)malloc(size);
		ferror = File::read(fh, buffer , size);

		ferror = File::seek(fh,FILE_SEEK_BEGIN,0);

		///////////////////////READ IMAGE PROPERTIES FROM TGA HEADER///////////////////
		short * sValue = new short();
		BYTE * bValue = new BYTE();

		ferror = File::seek(fh,FILE_SEEK_BEGIN,12); /////////Seek to Image Width,Height and bit depth
		
		ferror = File::read(fh,sValue,sizeof(short));
		textureHeader->width = *sValue;

		ferror = File::read(fh,sValue,sizeof(short));
		textureHeader->height= *sValue;

		ferror = File::read(fh,bValue,sizeof(BYTE));
		textureHeader->bitDepth= *bValue;

		delete sValue;
		delete bValue;
		///////////////////////////////////////////////////////////////////////////////
		

		if (ferror ==  FILE_SUCCESS)
		{	
			ferror = File::close(fh);
			ferror = File::open(fh, "tex.bin", FILE_WRITE);
			
			if (ferror ==  FILE_SUCCESS)
			{
				ferror = File::write(fh, textureHeader, sizeof(TEX_Header));
				ferror = File::write(fh, buffer, size);
			}

		}
	}

	ferror = File::close(fh);

	system("vodka tex.bin dtex.blo -t VERTS_TYPE -n TexRaw");


	free(buffer);
}
void WriteCDMFile()
{
	std::string CommandLineFull;
	
	CommandLineFull.append("liu ");
	CommandLineFull.append(outFile);
	CommandLineFull.append(" -n ");
	CommandLineFull.append(fileName);
	CommandLineFull.append(" -v 0.1_rev1");

	system(CommandLineFull.c_str());
}
void CleanUp ()
{
	system("del *.bin");
	system("del *.blo");
}

///////////////////////////////////////////Vector Lists Generators/////////////////////////
void SortVBOList(std::vector<FBX_Vertex_vsn> &FBX_vert_fat_sort)
{
	int ssize = FBX_vert_fat_sort.size();

	std::sort(FBX_vert_fat_sort.begin(),FBX_vert_fat_sort.end());

	FBX_Vertex_vsn vTmp;

	printf("\n\nFBX SORT\n\n");

	for (int i = 0; i< (int) FBX_vert_fat_sort.size(); i++)
	{
		vTmp = FBX_vert_fat_sort[i];
		printf("FBX_fat_sort[%d]: v: %f %f %f s: %f %f n: %f %f %f \n",i,vTmp.x,vTmp.y,vTmp.z,vTmp.s,vTmp.t,vTmp.nx,vTmp.ny,vTmp.nz);
	}
}
void ReducingProcess(std::vector<FBX_Vertex_vsn> &FBX_vert_fat_sort)
{
	FBX_Vertex_vsn vTmp;

	VBO_vert.reserve( FBX_vert_fat_sort.size() );

	FBX_Vertex_vsn currVert = FBX_vert_fat_sort[0];
	VBO_vert.push_back(currVert);

	for ( int i=1; i< (int)FBX_vert_fat_sort.size(); i++)
	{
			vTmp = FBX_vert_fat_sort[i];

			if ( !(vTmp == currVert) )
			{
				currVert = vTmp;
				VBO_vert.push_back(currVert);
			}
	}

	printf("\n\nFBX REDUCED\n\n");

	for (int i = 0; i< (int) VBO_vert.size(); i++)
	{
		vTmp = VBO_vert[i];
		printf("VBO_Vert[%d]: v: %f %f %f s: %f %f n: %f %f %f \n",i,vTmp.x,vTmp.y,vTmp.z,vTmp.s,vTmp.t,vTmp.nx,vTmp.ny,vTmp.nz);
	}

}
void GenerateTrilist()
{

	printf("SIZE %d", (int)VBO_vert.size());
	//assert( 65536 > (int)VBO_vert.size() );

	VBO_tri.reserve( FBX_tri.size() );
	std::vector< FBX_Vertex_vsn >::iterator it;

	int new_v0;
	int new_v1;
	int new_v2;

	VBO_Trilist tmpVBOTri;

	for (int i=0; i< (int) FBX_tri.size(); i++)
	{

		FBX_Trilist old_tri = FBX_tri[i];
		FBX_Vertex_vsn vert;

		vert = FBX_vert_fat[old_tri.v0];

		it = std::find(VBO_vert.begin(), VBO_vert.end(), vert );
		new_v0 = std::distance(VBO_vert.begin(), it);

		vert = FBX_vert_fat[old_tri.v1];

		it = std::find(VBO_vert.begin(), VBO_vert.end(), vert );
		new_v1 = std::distance(VBO_vert.begin(), it);

		vert = FBX_vert_fat[old_tri.v2];

		it = std::find(VBO_vert.begin(), VBO_vert.end(), vert );
		new_v2 = std::distance(VBO_vert.begin(), it);


		tmpVBOTri.v0 = new_v0;
		tmpVBOTri.v1 = new_v1;
		tmpVBOTri.v2 = new_v2;

		VBO_tri.push_back(tmpVBOTri);

	}

	printf("\n\n\nFBX_TRILIST\n\n");

	for (int i = 0; i< (int) VBO_tri.size(); i++)
	{
		tmpVBOTri = VBO_tri[i];
		printf("VBO_tri[%d] %d %d %d \n",i, tmpVBOTri.v0, tmpVBOTri.v1, tmpVBOTri.v2);
	}

}

/////////////////////////////////Get Command Line Arguments///////////////////////////////
void getArgumentFiles(int argc, char** argv)
{
	inFile = argv[1];
	printf("%s \n",inFile);
	outFile = argv[2];
	printf("%s \n",outFile);

	if (argc > 4)
	{
		int Cmp = strcmp(argv[3],"-t");
		if (0 == Cmp)
		{
			texture = argv[4];
		}
	}
	else
	{
		incTexture = false;
	}

	int length = strlen(outFile);
	strncpy ( fileName, outFile, length-4);
}

int main(int argc, char** argv)
{
    lSdkManager = 0;
    KFbxScene* lScene = NULL;
    bool lResult;
     
    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

	getArgumentFiles(argc,argv);


    // The example can take a FBX file as an argument.
    if(argc > 1)
    {
		printf("\n\nFile: %s\n\n",	inFile);
        lResult = LoadScene(lSdkManager, lScene, inFile);
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

        printf("\n\n------------\nNode Content\n------------\n\n");

        DisplayContent(lScene);

        printf("\n\n----\nPose\n----\n\n");

        DisplayPose(lScene);

        printf("\n\n---------\nAnimation\n---------\n\n");

        DisplayAnimation(lScene);

        //now display generic information

        printf("\n\n---------\nGeneric Information\n---------\n\n");
        DisplayGenericInfo(lScene);

		std::vector<FBX_Vertex_vsn> FBX_vert_fat_sort(FBX_vert_fat);

		SortVBOList(FBX_vert_fat_sort);
		ReducingProcess(FBX_vert_fat_sort);
		GenerateTrilist();
		WriteCDMheader();
		WriteTristoFile();
		WriteVBOstoFile();
		
		if (incTexture)
			WriteTexturetoFile();

		WriteCDMFile();
		
		CleanUp();
    }
	

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager);

	printf("Conversion Done! \n");
	printf("Press any Key to start next Conversion \n");
	system("pause");
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
            DisplayMarker(pNode);
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

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
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

