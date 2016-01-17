/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"


void DisplayLodGroup(KFbxNode* pNode)
{
    char* lDisplayLevels[] = { "UseLOD", "Show", "Hide" }; 

    DisplayString("LodGroup Name: ", (char *) pNode->GetName());

    DisplayInt("    ", pNode->GetChildCount(), " Geometries");
    for (int i = 0; i < pNode->GetChildCount(); i++)
    {
        KFbxNode* lChildNode = pNode->GetChild(i);
        DisplayString("        ", lChildNode->GetName());
    }

    KFbxLodGroup *lLodGroupAttr = (KFbxLodGroup*)pNode->GetNodeAttribute();
    DisplayBool("    MinMaxDistance Enabled: ", lLodGroupAttr->MinMaxDistance.Get());
    if (lLodGroupAttr->MinMaxDistance.Get())
    {
        DisplayDouble("        Min Distance: ", lLodGroupAttr->MinDistance.Get());
        DisplayDouble("        Max Distance: ", lLodGroupAttr->MaxDistance.Get());
    }
    DisplayBool("    Is World Space: ", lLodGroupAttr->WorldSpace.Get());

    DisplayString("    Thresholds ");
    for (int i = 0; i < lLodGroupAttr->GetNumThresholds(); i++)
    {
        fbxDistance lThreshVal;
        if (lLodGroupAttr->GetThreshold(i, lThreshVal))
            DisplayDouble("        ", lThreshVal.value());
    }

    DisplayString("    DisplayLevels");
    for (int i = 0; i < lLodGroupAttr->GetNumDisplayLevels(); i++)
    {
        KFbxLodGroup::EDisplayLevel lLevel;
        if (lLodGroupAttr->GetDisplayLevel(i, lLevel))
            DisplayString("        ", lDisplayLevels[lLevel]);
    }
}
