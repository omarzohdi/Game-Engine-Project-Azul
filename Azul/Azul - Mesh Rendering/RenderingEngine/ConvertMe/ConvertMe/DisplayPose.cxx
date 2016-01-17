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

void DisplayPose(KFbxScene* pScene)
{
    int      i,j,k,lPoseCount;
    KString  lName;

    lPoseCount = pScene->GetPoseCount();

    for (i = 0; i < lPoseCount; i++)
    {
        KFbxPose* lPose = pScene->GetPose(i);

        lName = lPose->GetName();
        DisplayString("Pose Name: ", lName.Buffer());

        DisplayBool("    Is a bind pose: ", lPose->IsBindPose());

        DisplayInt("    Number of items in the pose: ", lPose->GetCount());

        DisplayString("","");

        for (j=0; j<lPose->GetCount(); j++)
        {
            lName = lPose->GetNodeName(j).GetCurrentName();
            DisplayString("    Item name: ", lName.Buffer());

            if (!lPose->IsBindPose())
            {
                // Rest pose can have local matrix
                DisplayBool("    Is local space matrix: ", lPose->IsLocalMatrix(j));
            }

            DisplayString("    Matrix value: ","");

            KString lMatrixValue;

            for (k=0; k<4; k++)
            {
                KFbxMatrix  lMatrix = lPose->GetMatrix(j);
                KFbxVector4 lRow = lMatrix.GetRow(k);
                char        lRowValue[1024];

                sprintf(lRowValue, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
                lMatrixValue += KString("        ") + KString(lRowValue);
            }

            DisplayString("", lMatrixValue.Buffer());
        }
    }

    lPoseCount = pScene->GetCharacterPoseCount();

    for (i = 0; i < lPoseCount; i++)
    {
        KFbxCharacterPose* lPose = pScene->GetCharacterPose(i);
        KFbxCharacter*     lCharacter = lPose->GetCharacter();

        if (!lCharacter) break;

        DisplayString("Character Pose Name: ", lCharacter->mName.Buffer());

        KFbxCharacterLink lCharacterLink;
        ECharacterNodeId  lNodeId = eCharacterHips;

        while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
        {
            KFbxXMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(KTIME_ZERO);

            DisplayString("    Matrix value: ","");

            KString lMatrixValue;

            for (k=0; k<4; k++)
            {
                KFbxVector4 lRow = lGlobalPosition.GetRow(k);
                char        lRowValue[1024];

                sprintf(lRowValue, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
                lMatrixValue += KString("        ") + KString(lRowValue);
            }

            DisplayString("", lMatrixValue.Buffer());

            lNodeId = ECharacterNodeId(int(lNodeId) + 1);
        }
    }
}


