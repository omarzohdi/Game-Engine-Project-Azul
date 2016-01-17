/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplayCamera.h"


void DisplayGlobalLightSettings(KFbxGlobalSettings* pGlobalSettings)
{
    DisplayColor("Ambient Color: ", pGlobalSettings->GetAmbientColor());
    DisplayString("");
}


void DisplayGlobalCameraSettings(KFbxGlobalSettings* pGlobalSettings)
{
    DisplayString("Default Camera: ", pGlobalSettings->GetDefaultCamera());
    DisplayString("");
}


void DisplayGlobalTimeSettings(KFbxGlobalSettings* pGlobalSettings)
{
    char lTimeString[256];

    DisplayString("Time Mode : ", KTime_GetTimeModeName(pGlobalSettings->GetTimeMode()));

    KTimeSpan lTs;
    KTime     lStart, lEnd;
    pGlobalSettings->GetTimelineDefaultTimeSpan(lTs);
    lStart = lTs.GetStart();
    lEnd   = lTs.GetStop();
    DisplayString("Timeline default timespan: ");
    DisplayString("     Start: ", lStart.GetTimeString(lTimeString));
    DisplayString("     Stop : ", lEnd.GetTimeString(lTimeString));

    DisplayString("");
}


