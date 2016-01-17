/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>
#include <vector>
#include "DisplayCommon.h"

extern std::string texture;

void DisplayTextureInfo(KFbxTexture* pTexture, int pBlendMode)
{
	KFbxFileTexture *lFileTexture = KFbxCast<KFbxFileTexture>(pTexture);
	KFbxProceduralTexture *lProceduralTexture = KFbxCast<KFbxProceduralTexture>(pTexture);

    DisplayString("            Name: \"", (char *) pTexture->GetName(), "\"");
	
	///////////////////////Save texture Name////////////////////////
	//texture = pTexture->GetName();
	/////////////////////////////////////////////////////////////////


	if (lFileTexture)
	{
		DisplayString("            Type: File Texture");
		DisplayString("            File Name: \"", (char *) lFileTexture->GetFileName(), "\"");
	}
	else if (lProceduralTexture)
	{
		DisplayString("            Type: Procedural Texture");
	}
    DisplayDouble("            Scale U: ", pTexture->GetScaleU());
    DisplayDouble("            Scale V: ", pTexture->GetScaleV());
    DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
    DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
    DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
    DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
    DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
    DisplayDouble("            Rotation W: ", pTexture->GetRotationW());

    char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

    DisplayString("            Alpha Source: ", lAlphaSources[pTexture->GetAlphaSource()]);
    DisplayDouble("            Cropping Left: ", pTexture->GetCroppingLeft());
    DisplayDouble("            Cropping Top: ", pTexture->GetCroppingTop());
    DisplayDouble("            Cropping Right: ", pTexture->GetCroppingRight());
    DisplayDouble("            Cropping Bottom: ", pTexture->GetCroppingBottom());

    char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
        "Box", "Face", "UV", "Environment" };

    DisplayString("            Mapping Type: ", lMappingTypes[pTexture->GetMappingType()]);

    if (pTexture->GetMappingType() == KFbxTexture::ePLANAR)
    {
        char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

        DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
    }

    char* lBlendModes[]   = { "Translucent", "Add", "Modulate", "Modulate2" };   
    if(pBlendMode >= 0)
        DisplayString("            Blend Mode: ", lBlendModes[pBlendMode]);
    DisplayDouble("            Alpha: ", pTexture->GetDefaultAlpha());

	if (lFileTexture)
	{
		char* lMaterialUses[] = { "Model Material", "Default Material" };
	    DisplayString("            Material Use: ", lMaterialUses[lFileTexture->GetMaterialUse()]);
	}

    char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
        "Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

    DisplayString("            Texture Use: ", pTextureUses[pTexture->GetTextureUse()]);
    DisplayString("");                

}

void FindAndDisplayTextureInfoByProperty(KFbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex){

    if( pProperty.IsValid() )
    {
		int lTextureCount = pProperty.GetSrcObjectCount(KFbxTexture::ClassId);

		for (int j = 0; j < lTextureCount; ++j)
		{
			//Here we have to check if it's layeredtextures, or just textures:
			KFbxLayeredTexture *lLayeredTexture = KFbxCast <KFbxLayeredTexture>(pProperty.GetSrcObject(KFbxLayeredTexture::ClassId, j));
			if (lLayeredTexture)
			{
                DisplayInt("    Layered Texture: ", j);
                KFbxLayeredTexture *lLayeredTexture = KFbxCast <KFbxLayeredTexture>(pProperty.GetSrcObject(KFbxLayeredTexture::ClassId, j));
                int lNbTextures = lLayeredTexture->GetSrcObjectCount(KFbxTexture::ClassId);
                for(int k =0; k<lNbTextures; ++k)
                {
                    KFbxTexture* lTexture = KFbxCast <KFbxTexture> (lLayeredTexture->GetSrcObject(KFbxTexture::ClassId,k));
                    if(lTexture)
                    {

                        if(pDisplayHeader){                    
                            DisplayInt("    Textures connected to Material ", pMaterialIndex);
                            pDisplayHeader = false;
                        }

                        //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                        //Why is that?  because one texture can be shared on different layered textures and might
                        //have different blend modes.

                        KFbxLayeredTexture::EBlendMode lBlendMode;
                        lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
                        DisplayString("    Textures for ", pProperty.GetName());
                        DisplayInt("        Texture ", k);  
                        DisplayTextureInfo(lTexture, (int) lBlendMode);   
                    }

                }
            }
			else
			{
				//no layered texture simply get on the property
                KFbxTexture* lTexture = KFbxCast <KFbxTexture> (pProperty.GetSrcObject(KFbxTexture::ClassId,j));
                if(lTexture)
                {
                    //display connected Material header only at the first time
                    if(pDisplayHeader){                    
                        DisplayInt("    Textures connected to Material ", pMaterialIndex);
                        pDisplayHeader = false;
                    }             

                    DisplayString("    Textures for ", pProperty.GetName());
                    DisplayInt("        Texture ", j);  
                    DisplayTextureInfo(lTexture, -1);
                }
            }
        }
    }//end if pProperty

}


void DisplayTexture(KFbxGeometry* pGeometry)
{
    int lMaterialIndex;
    KFbxProperty lProperty;
    if(pGeometry->GetNode()==NULL)
        return;
    int lNbMat = pGeometry->GetNode()->GetSrcObjectCount(KFbxSurfaceMaterial::ClassId);
    for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
        KFbxSurfaceMaterial *lMaterial = (KFbxSurfaceMaterial *)pGeometry->GetNode()->GetSrcObject(KFbxSurfaceMaterial::ClassId, lMaterialIndex);
        bool lDisplayHeader = true;

        //go through all the possible textures
        if(lMaterial){

            int lTextureIndex;
            FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = lMaterial->FindProperty(KFbxLayerElement::TEXTURE_CHANNEL_NAMES[lTextureIndex]);
                FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex); 
            }

        }//end if(lMaterial)

    }// end for lMaterialIndex     
}
