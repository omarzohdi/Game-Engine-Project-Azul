/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"


void DisplayMarker(KFbxNode* pNode)
{
    KFbxMarker* lMarker = (KFbxMarker*)pNode->GetNodeAttribute();
    KString     lString;

    DisplayString("Marker Name: ", (char *) pNode->GetName());
    DisplayMetaDataConnections(lMarker);

    // Type
    lString = "    Marker Type: ";
    switch (lMarker->GetType())
    {
    case KFbxMarker::eSTANDARD:    lString += "Standard";    break;
    case KFbxMarker::eOPTICAL:     lString += "Optical";     break;
    case KFbxMarker::eIK_EFFECTOR: lString += "IK Effector"; break;
    case KFbxMarker::eFK_EFFECTOR: lString += "FK Effector"; break;
    }
    DisplayString(lString.Buffer());

    // Look
    lString = "    Marker Look: ";
    switch (lMarker->Look.Get())
    {
    case KFbxMarker::eCUBE:         lString += "Cube";        break;
    case KFbxMarker::eHARD_CROSS:   lString += "Hard Cross";  break;
    case KFbxMarker::eLIGHT_CROSS:  lString += "Light Cross"; break;
    case KFbxMarker::eSPHERE:       lString += "Sphere";      break;
    }
    DisplayString(lString.Buffer());

    // Size
    lString = KString("    Size: ") + KString(lMarker->Size.Get());
    DisplayString(lString.Buffer());

    // Color
    fbxDouble3 c = lMarker->Color.Get();
    KFbxColor color(c[0], c[1], c[2]);
    DisplayColor("    Color: ", color);

    // IKPivot
    Display3DVector("    IKPivot: ", lMarker->IKPivot.Get());
}

