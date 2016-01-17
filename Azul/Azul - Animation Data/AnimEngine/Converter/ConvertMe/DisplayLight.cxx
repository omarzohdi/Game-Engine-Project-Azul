/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"


void DisplayDefaultAnimationValues(KFbxLight* pLight);


void DisplayLight(KFbxNode* pNode)
{
    KFbxLight* lLight = (KFbxLight*) pNode->GetNodeAttribute();

    DisplayString("Light Name: ", (char *) pNode->GetName());
    DisplayMetaDataConnections(lLight);

    char* lLightTypes[] = { "Point", "Directional", "Spot" };

    DisplayString("    Type: ", lLightTypes[lLight->LightType.Get()]);
    DisplayBool("    Cast Light: ", lLight->CastLight.Get());

    if (!(lLight->FileName.Get().IsEmpty()))
    {
        DisplayString("    Gobo");

        DisplayString("        File Name: \"", lLight->FileName.Get().Buffer(), "\"");
        DisplayBool("        Ground Projection: ", lLight->DrawGroundProjection.Get());
        DisplayBool("        Volumetric Projection: ", lLight->DrawVolumetricLight.Get());
        DisplayBool("        Front Volumetric Projection: ", lLight->DrawFrontFacingVolumetricLight.Get());
    }

    DisplayDefaultAnimationValues(lLight);
}


void DisplayDefaultAnimationValues(KFbxLight* pLight)
{
    DisplayString("    Default Animation Values");

    fbxDouble3 c = pLight->Color.Get();
    KFbxColor lColor(c[0], c[1], c[2]);
    DisplayColor("        Default Color: ", lColor);
    DisplayDouble("        Default Intensity: ", pLight->Intensity.Get());
    DisplayDouble("        Default Cone Angle: ", pLight->ConeAngle.Get());
    DisplayDouble("        Default Fog: ", pLight->Fog.Get());
}


