/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"

#include <stdio.h>


void DisplayLink(KFbxGeometry* pGeometry)
{
    //Display cluster now

    //int i, lLinkCount;
    //KFbxLink* lLink;

    int i, j;
    int lSkinCount=0;
    int lClusterCount=0;
    KFbxCluster* lCluster;

    lSkinCount=pGeometry->GetDeformerCount(KFbxDeformer::eSKIN);



    //lLinkCount = pGeometry->GetLinkCount();
    for(i=0; i!=lSkinCount; ++i)
    {
        lClusterCount = ((KFbxSkin *) pGeometry->GetDeformer(i, KFbxDeformer::eSKIN))->GetClusterCount();
        for (j = 0; j != lClusterCount; ++j)
        {
            DisplayInt("    Cluster ", i);

            lCluster=((KFbxSkin *) pGeometry->GetDeformer(i, KFbxDeformer::eSKIN))->GetCluster(j);
            //lLink = pGeometry->GetLink(i);    

            char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

            DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

            if(lCluster->GetLink() != NULL)
            {
                DisplayString("        Name: ", (char *) lCluster->GetLink()->GetName());
            }

            KString lString1 = "        Link Indices: ";
            KString lString2 = "        Weight Values: ";

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();

            for(k = 0; k < lIndexCount; k++)
            {
                lString1 += lIndices[k];
                lString2 += (float) lWeights[k];

                if (k < lIndexCount - 1)
                {
                    lString1 += ", ";
                    lString2 += ", ";
                }
            }

            lString1 += "\n";
            lString2 += "\n";

            printf(lString1);
            printf(lString2);

            DisplayString("");

            KFbxXMatrix lMatrix;

            lMatrix = lCluster->GetTransformMatrix(lMatrix);
            Display3DVector("        Transform Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Scaling: ", lMatrix.GetS());

            lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
            Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

            if (lCluster->GetAssociateModel() != NULL)
            {
                lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                DisplayString("        Associate Model: ", (char *) lCluster->GetAssociateModel()->GetName());
                Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
                Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
                Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
            }

            DisplayString("");
        }
    }
}


