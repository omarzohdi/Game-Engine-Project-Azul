/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"


void DisplayShape(KFbxGeometry* pGeometry)
{
    int lBlendShapeCount, lBlendShapeChannelCount, lTargetShapeCount;
    KFbxBlendShape* lBlendShape;
	KFbxBlendShapeChannel* lBlendShapeChannel;
	KFbxShape* lShape;

    lBlendShapeCount = pGeometry->GetDeformerCount(KFbxDeformer::eBLENDSHAPE);

    for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeCount; ++lBlendShapeIndex)
    {
		lBlendShape = (KFbxBlendShape*) pGeometry->GetDeformer(lBlendShapeIndex, KFbxDeformer::eBLENDSHAPE);   
        DisplayString("    BlendShape ", (char *) lBlendShape->GetName());
        
		lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
		for(int lBlendShapeChannelIndex = 0;  lBlendShapeChannelIndex < lBlendShapeChannelCount; ++lBlendShapeChannelIndex)
		{
			lBlendShapeChannel = lBlendShape->GetBlendShapeChannel(lBlendShapeChannelIndex);
			DisplayString("    BlendShapeChannel ", (char *) lBlendShapeChannel->GetName());
			DisplayDouble("    Default Deform Value: ", lBlendShapeChannel->DeformPercent.Get());

			lTargetShapeCount = lBlendShapeChannel->GetTargetShapeCount();
			for (int lTargetShapeIndex = 0; lTargetShapeIndex < lTargetShapeCount; ++lTargetShapeIndex)
			{
				lShape = lBlendShapeChannel->GetTargetShape(lTargetShapeIndex);
				DisplayString("    TargetShape ", (char *) lShape->GetName());

				int j, lControlPointsCount = lShape->GetControlPointsCount();
				KFbxVector4* lControlPoints = lShape->GetControlPoints();
				KFbxLayerElementArrayTemplate<KFbxVector4>* lNormals = NULL;    
				bool lStatus = lShape->GetNormals(&lNormals); 

				for(j = 0; j < lControlPointsCount; j++)
				{
					DisplayInt("        Control Point ", j);
					Display3DVector("            Coordinates: ", lControlPoints[j]);

					if (lStatus && lNormals && lNormals->GetCount() == lControlPointsCount)
					{
						Display3DVector("            Normal Vector: ", lNormals->GetAt(j));
					}
				}

				DisplayString("");
			}
		}
    }
}


