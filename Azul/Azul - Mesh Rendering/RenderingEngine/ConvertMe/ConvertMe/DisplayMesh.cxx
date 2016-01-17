/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplayMaterial.h"
#include "DisplayTexture.h"
#include "DisplayLink.h"
#include "DisplayShape.h"

#include <stdio.h>
#include <vector>
#include <assert.h>
#include "ModelData.h"


extern KFbxSdkManager* lSdkManager;
extern std::vector<FBX_Vertex> FBX_vert;
extern std::vector<FBX_Vertex_vsn> FBX_vert_fat;
extern std::vector<FBX_Trilist> FBX_tri;

#define MAT_HEADER_LENGTH 200

void DisplayControlsPoints(KFbxMesh* pMesh);
void DisplayPolygons(KFbxMesh* pMesh);
void DisplayMaterialMapping(KFbxMesh* pMesh);
void DisplayTextureMapping(KFbxMesh* pMesh);
void DisplayTextureNames( KFbxProperty &pProperty, KString& pConnectionString );
void DisplayMaterialConnections(KFbxMesh* pMesh);
void DisplayMaterialTextureConnections( KFbxSurfaceMaterial* pMaterial, 
                                       char * header, int pMatId, int l );

void TriangulateMesh(KFbxMesh* &mesh)
{

      if(mesh->IsTriangleMesh())
      {
            // The mesh is already triangulated
            return;
      }
      else
      {
            // get the parent node of the mesh
            KFbxNode* node = mesh->GetNode();
            // instantiate the geometry converter
            KFbxGeometryConverter converter(lSdkManager);
            // apply the converter on the parent node of the mesh
            bool status = converter.TriangulateInPlace(node);

            if(!status)
            {

                  printf("Failed to triangulate!\n");
                  exit(-1);
            }

            // the converter swaps the old (untriangulated mesh) with
            // a new triangulated mesh under the same parent node
            // (the old mesh is gone), that's why we need to return
            // a pointer to the new mesh by reference
            mesh = (KFbxMesh*) (node->GetNodeAttribute());
      }
}

void DisplayMesh(KFbxNode* pNode)
{
    KFbxMesh* lMesh = (KFbxMesh*) pNode->GetNodeAttribute ();

    DisplayString("Mesh Name: ", (char *) pNode->GetName());
	TriangulateMesh(lMesh);

    DisplayMetaDataConnections(lMesh);
    DisplayControlsPoints(lMesh);
    DisplayPolygons(lMesh);
    DisplayMaterialMapping(lMesh);
    DisplayMaterial(lMesh);
    DisplayTexture(lMesh);
    DisplayMaterialConnections(lMesh);
    DisplayLink(lMesh);
    DisplayShape(lMesh);
}


void DisplayControlsPoints(KFbxMesh* pMesh)
{
    int i, lControlPointsCount = pMesh->GetControlPointsCount();
    KFbxVector4* lControlPoints = pMesh->GetControlPoints();

    DisplayString("    Control Points");

    printf("k0:  num control pts: %d \n", lControlPointsCount);

	
	FBX_vert.reserve( lControlPointsCount );
	int ssize = FBX_vert.capacity();

	FBX_Vertex vTmp;
	 
    for (i = 0; i < lControlPointsCount; i++)
    {
        //DisplayInt("        Control Point ", i);
        //Display3DVector("            Coordinates: ", lControlPoints[i]);
       printf("  %d ---------\n",i);
       printf("     [%d]v: %f, %f, %f\n",i,lControlPoints[i][0], lControlPoints[i][1], lControlPoints[i][2]);

	   vTmp.x = (float)lControlPoints[i][0];
	   vTmp.y = (float)lControlPoints[i][1];
	   vTmp.z = (float)lControlPoints[i][2];
	   vTmp.nx = (float)lControlPoints[i][3];
	   vTmp.ny = (float)lControlPoints[i][4];
	   vTmp.nz = (float)lControlPoints[i][5];

        for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
        {
            KFbxGeometryElementNormal* leNormals = pMesh->GetElementNormal( j);

			float f = (float)pMesh->GetElementNormalCount();

			if (leNormals->GetMappingMode() == KFbxGeometryElement::eBY_CONTROL_POINT)
			{
				char header[100];
				sprintf(header, "            Normal Vector: "); 

				if (leNormals->GetReferenceMode() == KFbxGeometryElement::eDIRECT)
				{
					//	Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
				printf("     [%d]n: %f, %f, %f\n",i,leNormals->GetDirectArray().GetAt(i)[0], leNormals->GetDirectArray().GetAt(i)[1], leNormals->GetDirectArray().GetAt(i)[2]);
			
				vTmp.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
				vTmp.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
				vTmp.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];

				printf("FBX_V[i]:v: %f %f %f    n: %f %f %f \n", vTmp.x,vTmp.y,vTmp.z,vTmp.nx,vTmp.ny,vTmp.nz);
               
				}
			}
			else
			{
				vTmp.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
				vTmp.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
				vTmp.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
			}
        }

		FBX_vert.push_back( vTmp );
    }

    DisplayString("");


	printf("\n\n\nFBX_VERTEX\n\n");

	for(int i=0; i<(int)FBX_vert.size(); i++)
	{
		vTmp = FBX_vert[i];
		printf("FBX_V[%d]:v: %f %f %f    n: %f %f %f \n",i,vTmp.x,vTmp.y,vTmp.z,vTmp.nx,vTmp.ny,vTmp.nz);
	}

	printf("\n\n\n");

}


void DisplayPolygons(KFbxMesh* pMesh)
{
    int i, j, lPolygonCount = pMesh->GetPolygonCount();
    KFbxVector4* lControlPoints = pMesh->GetControlPoints(); 
    char header[100];

    DisplayString("    Polygons");
    
    printf(" num of Triangles: %d\n",lPolygonCount);

	FBX_vert_fat.reserve(lPolygonCount * 3);
	FBX_tri.reserve(lPolygonCount);


	FBX_Vertex_vsn vTmp;
	FBX_Trilist tmpTrilist;

	int trilist_count = 0;

    int vertexId = 0;
    for (i = 0; i < lPolygonCount; i++)
    {
    //    DisplayInt("        Polygon ", i);
        int l;

        for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
        {
            KFbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case KFbxGeometryElement::eBY_POLYGON:
				if (lePolgrp->GetReferenceMode() == KFbxGeometryElement::eINDEX)
				{
					sprintf(header, "        Assigned to group: "); 
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					DisplayInt(header, polyGroupId);
					break;
				}
			default:
				// any other mapping modes don't make sense
				DisplayString("        \"unsupported group assignment\"");
				break;
			}
        }

        int lPolygonSize = pMesh->GetPolygonSize(i);
        printf("poly[%d] \n",i);
		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

         //printf("poly[%d] \n",i);
			//Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);
         printf("    [%d]v: %f %f %f\n",
            lControlPointIndex,
            lControlPoints[lControlPointIndex][0],
            lControlPoints[lControlPointIndex][1],
            lControlPoints[lControlPointIndex][2]);


			vTmp.x = FBX_vert[lControlPointIndex].x;
			vTmp.y = FBX_vert[lControlPointIndex].y;
			vTmp.z = FBX_vert[lControlPointIndex].z;

			vTmp.nx = FBX_vert[lControlPointIndex].nx;
			vTmp.ny = FBX_vert[lControlPointIndex].ny;
			vTmp.nz = FBX_vert[lControlPointIndex].nz;

			if (j == 0)
			{
				tmpTrilist.v0 = trilist_count++;
			}
			else if (j == 1)
			{
				tmpTrilist.v1 = trilist_count++;
			}
			else if(j == 2)
			{
				tmpTrilist.v2 = trilist_count++;
			}
			else
			{
				assert (0 == 1);
			}
			
         for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			{
				KFbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor( l);
				sprintf(header, "            Color vertex: "); 

				switch (leVtxc->GetMappingMode())
				{
				case KFbxGeometryElement::eBY_CONTROL_POINT:
					switch (leVtxc->GetReferenceMode())
					{
					case KFbxGeometryElement::eDIRECT:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case KFbxGeometryElement::eINDEX_TO_DIRECT:
						{
							int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case KFbxGeometryElement::eBY_POLYGON_VERTEX:
					{
						switch (leVtxc->GetReferenceMode())
						{
						case KFbxGeometryElement::eDIRECT:
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
							break;
						case KFbxGeometryElement::eINDEX_TO_DIRECT:
							{
								int id = leVtxc->GetIndexArray().GetAt(vertexId);
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case KFbxGeometryElement::eBY_POLYGON: // doesn't make much sense for UVs
				case KFbxGeometryElement::eALL_SAME:   // doesn't make much sense for UVs
				case KFbxGeometryElement::eNONE:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				KFbxGeometryElementUV* leUV = pMesh->GetElementUV( l);
				sprintf(header, "            Texture UV: "); 

				switch (leUV->GetMappingMode())
				{
				case KFbxGeometryElement::eBY_CONTROL_POINT:
					switch (leUV->GetReferenceMode())
					{
					case KFbxGeometryElement::eDIRECT:
						Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case KFbxGeometryElement::eINDEX_TO_DIRECT:
						{
							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
							Display2DVector(header, leUV->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case KFbxGeometryElement::eBY_POLYGON_VERTEX:
					{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case KFbxGeometryElement::eDIRECT:
						case KFbxGeometryElement::eINDEX_TO_DIRECT:
							{
                        
							//	Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
                       printf("   [???]uv: %f %f\n",
                           leUV->GetDirectArray().GetAt(lTextureUVIndex)[0], 
                           leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);

						vTmp.s = (float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						vTmp.t = (float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];

						             printf("FBX_fat: v: %f %f %f s: %f %f n: %f %f %f \n",
										 vTmp.x,vTmp.y,vTmp.z,vTmp.s,vTmp.t,vTmp.nx,vTmp.ny,vTmp.nz);

							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case KFbxGeometryElement::eBY_POLYGON: // doesn't make much sense for UVs
				case KFbxGeometryElement::eALL_SAME:   // doesn't make much sense for UVs
				case KFbxGeometryElement::eNONE:       // doesn't make much sense for UVs
					break;
				}
			}
			for( l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				KFbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
				sprintf(header, "            Normal: "); 

				if(leNormal->GetMappingMode() == KFbxGeometryElement::eBY_POLYGON_VERTEX)
				{
					float f;
					switch (leNormal->GetReferenceMode())
					{
					case KFbxGeometryElement::eDIRECT:
						vTmp.nx = leNormal->GetDirectArray().GetAt(vertexId)[0];
						vTmp.ny = leNormal->GetDirectArray().GetAt(vertexId)[1];
						vTmp.nz = leNormal->GetDirectArray().GetAt(vertexId)[2];
						Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						break;
					case KFbxGeometryElement::eINDEX_TO_DIRECT:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for( l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				KFbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l);
				sprintf(header, "            Tangent: "); 

				if(leTangent->GetMappingMode() == KFbxGeometryElement::eBY_POLYGON_VERTEX)
				{
					switch (leTangent->GetReferenceMode())
					{
					case KFbxGeometryElement::eDIRECT:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						break;
					case KFbxGeometryElement::eINDEX_TO_DIRECT:
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for( l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				KFbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal( l);

				sprintf(header, "            Binormal: "); 
				if(leBinormal->GetMappingMode() == KFbxGeometryElement::eBY_POLYGON_VERTEX)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case KFbxGeometryElement::eDIRECT:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case KFbxGeometryElement::eINDEX_TO_DIRECT:
						{
							int id = leBinormal->GetIndexArray().GetAt(vertexId);
							Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;

			FBX_vert_fat.push_back(vTmp);

		} // for polygonSize

		FBX_tri.push_back(tmpTrilist);

		printf("FBX_tri %d %d %d \n", tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);
    } // for polygonCount


    //check visibility for the edges of the mesh
	for(int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
	{
		KFbxGeometryElementVisibility* leVisibility=pMesh->GetElementVisibility(l);
		sprintf(header, "    Edge Visibility : ");
		DisplayString(header);
		switch(leVisibility->GetMappingMode())
		{
			//should be eBY_EDGE
		case KFbxGeometryElement::eBY_EDGE:
			//should be eDIRECT
			for(int j=0; j!=pMesh->GetMeshEdgeCount();++j)
			{
				DisplayInt("        Edge ", j);
				DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
			}

			break;
		}
	}
    DisplayString("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");


	printf("\n\n\nFBX_VERTEX_FAT\n\n");

	for (int i = 0; i< (int) FBX_vert_fat.size(); i++)
	{
		vTmp = FBX_vert_fat[i];
		printf("FBX_fat[%d]: v: %f %f %f s: %f %f n: %f %f %f \n",i,vTmp.x,vTmp.y,vTmp.z,vTmp.s,vTmp.t,vTmp.nx,vTmp.ny,vTmp.nz);
	}

	printf("\n\n\nFBX_TRILIST\n\n");

	for (int i = 0; i< (int) FBX_tri.size(); i++)
	{
		tmpTrilist = FBX_tri[i];
		printf("FBX_tri %d %d %d \n", tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);
	}

	printf("\n\n\n");
}

void DisplayTextureNames( KFbxProperty &pProperty, KString& pConnectionString )
{
    int lLayeredTextureCount = pProperty.GetSrcObjectCount(KFbxLayeredTexture::ClassId);
    if(lLayeredTextureCount > 0)
    {
        for(int j=0; j<lLayeredTextureCount; ++j)
        {
            KFbxLayeredTexture *lLayeredTexture = KFbxCast <KFbxLayeredTexture>(pProperty.GetSrcObject(KFbxLayeredTexture::ClassId, j));
            int lNbTextures = lLayeredTexture->GetSrcObjectCount(KFbxTexture::ClassId);
            pConnectionString += " Texture ";

            for(int k =0; k<lNbTextures; ++k)
            {
                //lConnectionString += k;
                pConnectionString += "\"";
                pConnectionString += (char*)lLayeredTexture->GetName();
                pConnectionString += "\"";
                pConnectionString += " ";
            }
            pConnectionString += "of ";
            pConnectionString += pProperty.GetName();
            pConnectionString += " on layer ";
            pConnectionString += j;
        }
        pConnectionString += " |";
    }
    else
    {
        //no layered texture simply get on the property
        int lNbTextures = pProperty.GetSrcObjectCount(KFbxTexture::ClassId);

        if(lNbTextures > 0)
        {
            pConnectionString += " Texture ";
            pConnectionString += " ";

            for(int j =0; j<lNbTextures; ++j)
            {
                KFbxTexture* lTexture = KFbxCast <KFbxTexture> (pProperty.GetSrcObject(KFbxTexture::ClassId,j));
                if(lTexture)
                {
                    pConnectionString += "\"";
                    pConnectionString += (char*)lTexture->GetName();
                    pConnectionString += "\"";
                    pConnectionString += " ";
                }
            }
            pConnectionString += "of ";
            pConnectionString += pProperty.GetName();
            pConnectionString += " |";
        }
    }
}

void DisplayMaterialTextureConnections( KFbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l )
{
    if(!pMaterial)
        return;

    KString lConnectionString = "            Material %d -- ";
    //Show all the textures

    KFbxProperty lProperty;
    //Diffuse Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuse);
    DisplayTextureNames(lProperty, lConnectionString);

    //DiffuseFactor Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuseFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Emissive Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sEmissive);
    DisplayTextureNames(lProperty, lConnectionString);

    //EmissiveFactor Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sEmissiveFactor);
    DisplayTextureNames(lProperty, lConnectionString);


    //Ambient Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sAmbient);
    DisplayTextureNames(lProperty, lConnectionString); 

    //AmbientFactor Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sAmbientFactor);
    DisplayTextureNames(lProperty, lConnectionString);          

    //Specular Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sSpecular);
    DisplayTextureNames(lProperty, lConnectionString);  

    //SpecularFactor Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sSpecularFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Shininess Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sShininess);
    DisplayTextureNames(lProperty, lConnectionString);

    //Bump Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sBump);
    DisplayTextureNames(lProperty, lConnectionString);

    //Normal Map Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sNormalMap);
    DisplayTextureNames(lProperty, lConnectionString);

    //Transparent Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sTransparentColor);
    DisplayTextureNames(lProperty, lConnectionString);

    //TransparencyFactor Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sTransparencyFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Reflection Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sReflection);
    DisplayTextureNames(lProperty, lConnectionString);

    //ReflectionFactor Textures
    lProperty = pMaterial->FindProperty(KFbxSurfaceMaterial::sReflectionFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Update header with material info
	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (lStringOverflow)
	{
		// Truncate string!
		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
		lConnectionString = lConnectionString + "...";
	}
	sprintf(header, lConnectionString.Buffer(), pMatId, l);
	DisplayString(header);
}

void DisplayMaterialConnections(KFbxMesh* pMesh)
{
    int i, l, lPolygonCount = pMesh->GetPolygonCount();

    char header[MAT_HEADER_LENGTH];

    DisplayString("    Polygons Material Connections");

    //check whether the material maps with only one mesh
    bool lIsAllSame = true;
    for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
    {

        KFbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
		KFbxSurfaceMaterial* lMaterial = NULL;
		int lMatId = -1;
		if( lMaterialElement->GetMappingMode() == KFbxGeometryElement::eBY_POLYGON) 
		{
			lIsAllSame = false;
			break;
		}
    }

    //For eALL_SAME mapping type, just out the material and texture mapping info once
    if(lIsAllSame)
    {
        for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
        {

            KFbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
			if( lMaterialElement->GetMappingMode() == KFbxGeometryElement::eALL_SAME) 
			{
				KFbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));    
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				if(lMatId >= 0)
				{
					DisplayInt("        All polygons share the same material in mesh ", l);
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
        }

		//no material
		if(l == 0)
			DisplayString("        no material applied");
    }

    //For eBY_POLYGON mapping type, just out the material and texture mapping info once
    else
    {
        for (i = 0; i < lPolygonCount; i++)
        {
            DisplayInt("        Polygon ", i);

            for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
            {

                KFbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
				KFbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

				if(lMatId >= 0)
				{
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
            }
        }
    }
}


void DisplayMaterialMapping(KFbxMesh* pMesh)
{
    char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
    char* lReferenceMode[] = { "Direct", "Index", "Index to Direct"};

    int lMtrlCount = 0;
    KFbxNode* lNode = NULL;
    if(pMesh){
        lNode = pMesh->GetNode();
        if(lNode)
            lMtrlCount = lNode->GetMaterialCount();    
    }

    for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
    {
        KFbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial( l);
        if (leMat)
        {
            char header[100];
            sprintf(header, "    Material Element %d: ", l); 
            DisplayString(header);


            DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
            DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

            int lMaterialCount = 0;
            KString lString;

            if (leMat->GetReferenceMode() == KFbxGeometryElement::eDIRECT ||
                leMat->GetReferenceMode() == KFbxGeometryElement::eINDEX_TO_DIRECT)
            {
                lMaterialCount = lMtrlCount;
            }

            if (leMat->GetReferenceMode() == KFbxGeometryElement::eINDEX ||
                leMat->GetReferenceMode() == KFbxGeometryElement::eINDEX_TO_DIRECT)
            {
                int i;

                lString = "           Indices: ";

                int lIndexArrayCount = leMat->GetIndexArray().GetCount(); 
                for (i = 0; i < lIndexArrayCount; i++)
                {
                    lString += leMat->GetIndexArray().GetAt(i);

                    if (i < lIndexArrayCount - 1)
                    {
                        lString += ", ";
                    }
                }

                lString += "\n";

                printf(lString);
            }
        }
    }

    DisplayString("");
}
