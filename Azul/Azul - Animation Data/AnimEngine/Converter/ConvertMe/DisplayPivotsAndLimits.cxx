/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include <stdio.h>


void DisplayPivotsAndLimits(KFbxNode* pNode)
{
    KFbxVector4 lTmpVector;

    //
    // Pivots
    //
    printf("    Pivot Information\n");

    KFbxNode::EPivotState lPivotState;
    pNode->GetPivotState(KFbxNode::eSOURCE_SET, lPivotState);
    printf("        Pivot State: %s\n", lPivotState == KFbxNode::ePIVOT_STATE_ACTIVE ? "Active" : "Reference");

    lTmpVector = pNode->GetPreRotation(KFbxNode::eSOURCE_SET);
    printf("        Pre-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    lTmpVector = pNode->GetPostRotation(KFbxNode::eSOURCE_SET);
    printf("        Post-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    lTmpVector = pNode->GetRotationPivot(KFbxNode::eSOURCE_SET);
    printf("        Rotation Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    lTmpVector = pNode->GetRotationOffset(KFbxNode::eSOURCE_SET);
    printf("        Rotation Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    lTmpVector = pNode->GetScalingPivot(KFbxNode::eSOURCE_SET);
    printf("        Scaling Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    lTmpVector = pNode->GetScalingOffset(KFbxNode::eSOURCE_SET);
    printf("        Scaling Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Limits
    //
    KFbxNodeLimits lLimits = pNode->GetLimits();
    bool           lIsActive, lMinXActive, lMinYActive, lMinZActive;
    bool           lMaxXActive, lMaxYActive, lMaxZActive;
    KFbxVector4    lMinValues, lMaxValues;

    printf("Keenan    Limits Information\n");
    return;

    lIsActive = lLimits.GetTranslationLimitActive();
    lLimits.mTranslationLimits.GetLimitMinActive(lMinXActive, lMinYActive, lMinZActive);
    lLimits.mTranslationLimits.GetLimitMaxActive(lMaxXActive, lMaxYActive, lMaxZActive);
    lMinValues = lLimits.mTranslationLimits.GetLimitMin();
    lMaxValues = lLimits.mTranslationLimits.GetLimitMax();

    printf("        Translation limits: %s\n", lIsActive ? "Active" : "Inactive");
    printf("            X\n");
    printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[0]);
    printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[0]);
    printf("            Y\n");
    printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[1]);
    printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[1]);
    printf("            Z\n");
    printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[2]);
    printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[2]);

    lIsActive = lLimits.GetRotationLimitActive();
    lLimits.mRotationLimits.GetLimitMinActive(lMinXActive, lMinYActive, lMinZActive);
    lLimits.mRotationLimits.GetLimitMaxActive(lMaxXActive, lMaxYActive, lMaxZActive);
    lMinValues = lLimits.mRotationLimits.GetLimitMin();
    lMaxValues = lLimits.mRotationLimits.GetLimitMax();

    printf("        Rotation limits: %s\n", lIsActive ? "Active" : "Inactive");
    printf("            X\n");
    printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[0]);
    printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[0]);
    printf("            Y\n");
    printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[1]);
    printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[1]);
    printf("            Z\n");
    printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[2]);
    printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[2]);

    lIsActive = lLimits.GetScalingLimitActive();
    lLimits.mScalingLimits.GetLimitMinActive(lMinXActive, lMinYActive, lMinZActive);
    lLimits.mScalingLimits.GetLimitMaxActive(lMaxXActive, lMaxYActive, lMaxZActive);
    lMinValues = lLimits.mScalingLimits.GetLimitMin();
    lMaxValues = lLimits.mScalingLimits.GetLimitMax();

    printf("        Scaling limits: %s\n", lIsActive ? "Active" : "Inactive");
    printf("            X\n");
    printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[0]);
    printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[0]);
    printf("            Y\n");
    printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[1]);
    printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[1]);
    printf("            Z\n");
    printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
    printf("                Min Limit Value: %f\n", lMinValues[2]);
    printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
    printf("                Max Limit Value: %f\n", lMaxValues[2]);
}

