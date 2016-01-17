/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"


void DisplayMaterial(KFbxGeometry* pGeometry)
{
    int lMaterialCount = 0;
    KFbxNode* lNode = NULL;
    if(pGeometry){
        lNode = pGeometry->GetNode();
        if(lNode)
            lMaterialCount = lNode->GetMaterialCount();    
    }

    if (lMaterialCount > 0)
    {
        KFbxPropertyDouble3 lKFbxDouble3;
        KFbxPropertyDouble1 lKFbxDouble1;
        KFbxColor theColor;

        for (int lCount = 0; lCount < lMaterialCount; lCount ++)
        {
            DisplayInt("        Material ", lCount);

            KFbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);

            DisplayString("            Name: \"", (char *) lMaterial->GetName(), "\""); 

            //Get the implementation to see if it's a hardware shader.
            const KFbxImplementation* lImplementation = GetImplementation(lMaterial, ImplementationHLSL);
            KString lImplemenationType = "HLSL";
            if(!lImplementation)
            {
                lImplementation = GetImplementation(lMaterial, ImplementationCGFX);
                lImplemenationType = "CGFX";
            }
            if(lImplementation)
            {
                //Now we have a hardware shader, let's read it
                printf("            Hardware Shader Type: %s\n", lImplemenationType.Buffer());
                KFbxBindingTable const* lRootTable = lImplementation->GetRootTable();
                fbxString lFileName = lRootTable->DescAbsoluteURL.Get();
                fbxString lTechniqueName = lRootTable->DescTAG.Get(); 


                KFbxBindingTable const* lTable = lImplementation->GetRootTable();
                size_t lEntryNum = lTable->GetEntryCount();

                for(int i=0;i <(int)lEntryNum; ++i)
                {
                    const KFbxBindingTableEntry& lEntry = lTable->GetEntry(i);
                    const char* lEntrySrcType = lEntry.GetEntryType(true); 
                    KFbxProperty lFbxProp;


                    KString lTest = lEntry.GetSource();
                    printf("            Entry: %s\n", lTest.Buffer());


                    if ( strcmp( KFbxPropertyEntryView::sEntryType, lEntrySrcType ) == 0 )
                    {   
                        lFbxProp = lMaterial->FindPropertyHierarchical(lEntry.GetSource()); 
                        if(!lFbxProp.IsValid())
                        {
                            lFbxProp = lMaterial->RootProperty.FindHierarchical(lEntry.GetSource());
                        }


                    }
                    else if( strcmp( KFbxConstantEntryView::sEntryType, lEntrySrcType ) == 0 )
                    {
                        lFbxProp = lImplementation->GetConstants().FindHierarchical(lEntry.GetSource());
                    }
                    if(lFbxProp.IsValid())
                    {
                        if( lFbxProp.GetSrcObjectCount( FBX_TYPE(KFbxTexture) ) > 0 )
                        {
                            //do what you want with the textures
                            for(int j=0; j<lFbxProp.GetSrcObjectCount(FBX_TYPE(KFbxFileTexture)); ++j)
                            {
                                KFbxFileTexture *lTex = lFbxProp.GetSrcObject(FBX_TYPE(KFbxFileTexture),j);
                                printf("           File Texture: %s\n", lTex->GetFileName());
                            }
                            for(int j=0; j<lFbxProp.GetSrcObjectCount(FBX_TYPE(KFbxLayeredTexture)); ++j)
                            {
                                KFbxLayeredTexture *lTex = lFbxProp.GetSrcObject(FBX_TYPE(KFbxLayeredTexture),j);
                                printf("        Layered Texture: %s\n", lTex->GetName());
                            }
                            for(int j=0; j<lFbxProp.GetSrcObjectCount(FBX_TYPE(KFbxProceduralTexture)); ++j)
                            {
                                KFbxProceduralTexture *lTex = lFbxProp.GetSrcObject(FBX_TYPE(KFbxProceduralTexture),j);
                                printf("     Procedural Texture: %s\n", lTex->GetName());
                            }
                        }
                        else
                        {
                            KFbxDataType lFbxType = lFbxProp.GetPropertyDataType();
                            KString blah = lFbxType.GetName();
                            if(DTBool == lFbxType)
                            {
                                DisplayBool("                Bool: ", KFbxGet <bool> (lFbxProp) );
                            }
                            else if ( DTInteger == lFbxType ||  DTEnum  == lFbxType )
                            {
                                DisplayInt("                Int: ", KFbxGet <int> (lFbxProp));
                            }
                            else if ( DTFloat == lFbxType)
                            {
                                DisplayDouble("                Float: ", KFbxGet <float> (lFbxProp));

                            }
                            else if ( DTDouble == lFbxType)
                            {
                                DisplayDouble("                Double: ", KFbxGet <double> (lFbxProp));
                            }
                            else if ( DTString == lFbxType
                                ||  DTUrl  == lFbxType
                                ||  DTXRefUrl  == lFbxType )
                            {
                                DisplayString("                String: ", (KFbxGet <fbxString> (lFbxProp)).Buffer());
                            }
                            else if ( DTDouble2 == lFbxType)
                            {
                                fbxDouble2 lDouble2=KFbxGet <fbxDouble2> (lFbxProp);
                                KFbxVector2 lVect;
                                lVect[0] = lDouble2[0];
                                lVect[1] = lDouble2[1];

                                Display2DVector("                2D vector: ", lVect);
                            }
                            else if ( DTVector3D == lFbxType||DTDouble3 == lFbxType || DTColor3 == lFbxType)
                            {
                                fbxDouble3 lDouble3 = KFbxGet <fbxDouble3> (lFbxProp);


                                KFbxVector4 lVect;
                                lVect[0] = lDouble3[0];
                                lVect[1] = lDouble3[1];
                                lVect[2] = lDouble3[2];
                                Display3DVector("                3D vector: ", lVect);
                            }

                            else if ( DTVector4D == lFbxType || DTDouble4 == lFbxType || DTColor4 == lFbxType)
                            {
                                fbxDouble4 lDouble4 = KFbxGet <fbxDouble4> (lFbxProp);
                                KFbxVector4 lVect;
                                lVect[0] = lDouble4[0];
                                lVect[1] = lDouble4[1];
                                lVect[2] = lDouble4[2];
                                lVect[3] = lDouble4[3];
                                Display4DVector("                4D vector: ", lVect);
                            }
                            else if ( DTDouble44 == lFbxType)
                            {
                                fbxDouble44 lDouble44 = KFbxGet <fbxDouble44> (lFbxProp);
                                for(int j=0; j<4; ++j)
                                {

                                    KFbxVector4 lVect;
                                    lVect[0] = lDouble44[j][0];
                                    lVect[1] = lDouble44[j][1];
                                    lVect[2] = lDouble44[j][2];
                                    lVect[3] = lDouble44[j][3];
                                    Display4DVector("                4x4D vector: ", lVect);
                                }

                            }
                        }

                    }   
                }
            }
            else if (lMaterial->GetClassId().Is(KFbxSurfacePhong::ClassId))
            {
                // We found a Phong material.  Display its properties.

                // Display the Ambient Color
                lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->Ambient;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Ambient: ", theColor);

                // Display the Diffuse Color
                lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->Diffuse;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Diffuse: ", theColor);

                // Display the Specular Color (unique to Phong materials)
                lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->Specular;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Specular: ", theColor);

                // Display the Emissive Color
                lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->Emissive;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Emissive: ", theColor);

                //Opacity is Transparency factor now
                lKFbxDouble1 =((KFbxSurfacePhong *) lMaterial)->TransparencyFactor;
                DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());

                // Display the Shininess
                lKFbxDouble1 =((KFbxSurfacePhong *) lMaterial)->Shininess;
                DisplayDouble("            Shininess: ", lKFbxDouble1.Get());

                // Display the Reflectivity
                lKFbxDouble1 =((KFbxSurfacePhong *) lMaterial)->ReflectionFactor;
                DisplayDouble("            Reflectivity: ", lKFbxDouble1.Get());
            }
            else if(lMaterial->GetClassId().Is(KFbxSurfaceLambert::ClassId) )
            {
                // We found a Lambert material. Display its properties.
                // Display the Ambient Color
                lKFbxDouble3=((KFbxSurfaceLambert *)lMaterial)->Ambient;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Ambient: ", theColor);

                // Display the Diffuse Color
                lKFbxDouble3 =((KFbxSurfaceLambert *)lMaterial)->Diffuse;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Diffuse: ", theColor);

                // Display the Emissive
                lKFbxDouble3 =((KFbxSurfaceLambert *)lMaterial)->Emissive;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Emissive: ", theColor);

                // Display the Opacity
                lKFbxDouble1 =((KFbxSurfaceLambert *)lMaterial)->TransparencyFactor;
                DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());
            }
            else
                DisplayString("Unknown type of Material");

            KFbxPropertyString lString;
            lString = lMaterial->ShadingModel;
            DisplayString("            Shading Model: ", lString.Get().Buffer());
            DisplayString("");
        }
    }
}

