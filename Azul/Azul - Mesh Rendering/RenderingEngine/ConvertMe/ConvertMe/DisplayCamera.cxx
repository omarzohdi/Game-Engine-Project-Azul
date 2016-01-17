/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"

void DisplayCamera(KFbxCamera* pCamera, char* pName, KFbxNode* pTargetNode = NULL, KFbxNode* pTargetUpNode = NULL);
void DisplayDefaultAnimationValues(KFbxCamera* pCamera);
void DisplayRenderOptions(KFbxCamera* pCamera);
void DisplayCameraViewOptions(KFbxCamera* pCamera);
void DisplayBackgroundProperties(KFbxCamera* pCamera);
void DisplayApertureAndFilmControls(KFbxCamera* pCamera);
void DisplayViewingAreaControls(KFbxCamera* pCamera);
void DisplayCameraPositionAndOrientation(KFbxCamera* pCamera, KFbxNode* pTargetNode, KFbxNode* pUpTargetNode);


void DisplayCamera(KFbxNode* pNode)
{
    DisplayCamera((KFbxCamera*) pNode->GetNodeAttribute(), (char *) pNode->GetName(), pNode->GetTarget(), pNode->GetTargetUp());
}


void DisplayCamera(KFbxCamera* pCamera, char* pName, KFbxNode* pTargetNode, KFbxNode* pTargetUpNode)
{
    DisplayString("Camera Name: ", pName);
    if (!pCamera)
    {
        DisplayString("NOT FOUND");
        return;
    }
    DisplayMetaDataConnections(pCamera);

    DisplayCameraPositionAndOrientation(pCamera, pTargetNode, pTargetUpNode);

    char* lProjectionTypes[] = { "Perspective", "Orthogonal" };

    DisplayString("    Projection Type: ", lProjectionTypes[pCamera->ProjectionType.Get()]);

    DisplayViewingAreaControls(pCamera);

    // If camera projection type is set to KFbxCamera::eORTHOGONAL, the 
    // aperture and film controls are not relevant.
    if (pCamera->ProjectionType.Get() != KFbxCamera::eORTHOGONAL)
    {
        DisplayApertureAndFilmControls(pCamera);
    }

    DisplayBackgroundProperties(pCamera);
    DisplayCameraViewOptions(pCamera);
    DisplayRenderOptions(pCamera);
    DisplayDefaultAnimationValues(pCamera);
}


void DisplayCameraPositionAndOrientation(KFbxCamera* pCamera, KFbxNode* pTargetNode, KFbxNode* pTargetUpNode)
{
    DisplayString("    Camera Position and Orientation");
    Display3DVector("        Position: ", pCamera->Position.Get());

    if (pTargetNode)
    {
        DisplayString("        Camera Interest: ",(char *) pTargetNode->GetName());
    }
    else
    {
        Display3DVector("        Default Camera Interest Position: ", pCamera->InterestPosition.Get());
    }

    if (pTargetUpNode)
    {
        DisplayString("        Camera Up Target: ", (char *) pTargetUpNode->GetName());
    }
    else
    {
        Display3DVector("        Up Vector: ", pCamera->UpVector.Get());
    }

    DisplayDouble("        Roll: ", pCamera->Roll.Get());
}


void DisplayViewingAreaControls(KFbxCamera* pCamera)
{
    DisplayString("    Viewing Area Controls");

    char* lCameraFormat[] = { "Custom", "D1 NTSC", "NTSC", "PAL", "D1 PAL", 
        "HD", "640x480", "320x200", "320x240", "128x128",
        "Full Screen"  };

    DisplayString("        Format: ", lCameraFormat[pCamera->GetFormat()]);

    char* lAspectRatioModes[] = { "Window Size", "Fixed Ratio", "Fixed Resolution", 
        "Fixed Width", "Fixed Height" };

    DisplayString("        Aspect Ratio Mode: ", lAspectRatioModes[pCamera->GetAspectRatioMode()]);

    // If the ratio mode is eWINDOW_SIZE, both width and height values aren't relevant.
    if (pCamera->GetAspectRatioMode() != KFbxCamera::eWINDOW_SIZE)
    {
        DisplayDouble("        Aspect Width: ", pCamera->AspectWidth.Get());
        DisplayDouble("        Aspect Height: ", pCamera->AspectHeight.Get());
    }

    DisplayDouble("        Pixel Ratio: ", pCamera->PixelAspectRatio.Get());
    DisplayDouble("        Near Plane: ", pCamera->NearPlane.Get());
    DisplayDouble("        Far Plane: ", pCamera->FarPlane.Get());
    DisplayBool("        Mouse Lock: ", pCamera->LockMode.Get());
}


void DisplayApertureAndFilmControls(KFbxCamera* pCamera)
{
    DisplayString("    Aperture and Film Controls");

    char* lCameraApertureFormats[] = { "Custom", 
        "16mm Theatrical",
        "Super 16mm",
        "35mm Academy",
        "35mm TV Projection",
        "35mm Full Aperture",
        "35mm 1.85 Projection",
        "35mm Anamorphic",
        "70mm Projection",
        "VistaVision",
        "Dynavision",
        "Imax" };

    DisplayString("        Aperture Format: ", lCameraApertureFormats[pCamera->GetApertureFormat()]);

    char* lCameraApertureModes[] = { "Horizontal and Vertical", "Horizontal", "Vertical", "Focal Length" };

    DisplayString("        Aperture Mode: ", lCameraApertureModes[pCamera->GetApertureMode()]);

    DisplayDouble("        Aperture Width: ", pCamera->GetApertureWidth(), " inches");
    DisplayDouble("        Aperture Height: ", pCamera->GetApertureHeight(), " inches");
    DisplayDouble("        Squeeze Ratio: ", pCamera->GetSqueezeRatio());
    DisplayDouble("        Focal Length: ", pCamera->FocalLength.Get(), "mm");
    DisplayDouble("        Field of View: ", pCamera->FieldOfView.Get(), " degrees");
}


void DisplayBackgroundProperties(KFbxCamera* pCamera)
{
    DisplayString("    Background Properties");

    DisplayString("        Background File Name: \"", (char *) pCamera->GetBackgroundFileName(), "\"");

    char* lBackgroundDisplayModes[] = { "Disabled", "Always", "When Media" };

    DisplayString("        Background Display Mode: ", lBackgroundDisplayModes[pCamera->ViewFrustumBackPlaneMode.Get()]);

    DisplayBool("        Foreground Matte Threshold Enable: ", pCamera->ShowFrontplate.Get());

    // This option is only relevant if background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
    if (pCamera->ForegroundOpacity.Get())
    {
        DisplayDouble("        Foreground Matte Threshold: ", pCamera->BackgroundAlphaTreshold.Get());
    }

    KString lBackgroundPlacementOptions;

    if (pCamera->GetFitImage())
    {
        lBackgroundPlacementOptions += " Fit,";
    }
    if (pCamera->GetCenter())
    {
        lBackgroundPlacementOptions += " Center,";
    }
    if (pCamera->GetKeepRatio())
    {
        lBackgroundPlacementOptions += " Keep Ratio,";
    }
    if (pCamera->GetCrop())
    {
        lBackgroundPlacementOptions += " Crop,";
    }
    if (!lBackgroundPlacementOptions.IsEmpty())
    {
        KString lString =  lBackgroundPlacementOptions.Left(lBackgroundPlacementOptions.GetLen() - 1);
        DisplayString("        Background Placement Options: ",lString.Buffer());
    }

    DisplayDouble("        Background Distance: ", pCamera->BackPlaneDistance.Get());

    char* lCameraBackgroundDistanceModes[] = { "Relative to Interest", "Absolute from Camera" };

    DisplayString("        Background Distance Mode: ", lCameraBackgroundDistanceModes[pCamera->BackPlaneDistanceMode.Get()]);
}


void DisplayCameraViewOptions(KFbxCamera* pCamera)
{
    DisplayString("    Camera View Options");

    DisplayBool("        View Camera Interest: ", pCamera->ViewCameraToLookAt.Get());
    DisplayBool("        View Near Far Planes: ", pCamera->ViewFrustumNearFarPlane.Get());
    DisplayBool("        Show Grid: ", pCamera->ShowGrid.Get());
    DisplayBool("        Show Axis: ", pCamera->ShowAzimut.Get());
    DisplayBool("        Show Name: ", pCamera->ShowName.Get());
    DisplayBool("        Show Info on Moving: ", pCamera->ShowInfoOnMoving.Get());
    DisplayBool("        Show Time Code: ", pCamera->ShowTimeCode.Get());
    DisplayBool("        Display Safe Area: ", pCamera->DisplaySafeArea.Get());

    char* lSafeAreaStyles[] = { "Round", "Square" };
    KFbxColor color;
    fbxDouble3 c;

    DisplayString("        Safe Area Style: ", lSafeAreaStyles[pCamera->SafeAreaDisplayStyle.Get()]);
    DisplayBool("        Show Audio: ", pCamera->ShowAudio.Get());

    c = pCamera->BackgroundColor.Get();
    color = KFbxColor(c[0], c[1], c[2]);
    DisplayColor("        Background Color: ", color);

    c = pCamera->AudioColor.Get();
    color = KFbxColor(c[0], c[1], c[2]);
    DisplayColor("        Audio Color: ", color);

    DisplayBool("        Use Frame Color: ", pCamera->UseFrameColor.Get());

    c = pCamera->FrameColor.Get();
    color = KFbxColor(c[0], c[1], c[2]);
    DisplayColor("        Frame Color: ", color);
}


void DisplayRenderOptions(KFbxCamera* pCamera)
{
    DisplayString("    Render Options");

    char* lCameraRenderOptionsUsageTimes[] = { "Interactive", "At Render" };

    DisplayString("        Render Options Usage Time: ", lCameraRenderOptionsUsageTimes[pCamera->UseRealTimeDOFAndAA.Get()]);
    DisplayBool("        Use Antialiasing: ", pCamera->UseAntialiasing.Get());
    DisplayDouble("        Antialiasing Intensity: ", pCamera->AntialiasingIntensity.Get());

    char* lCameraAntialiasingMethods[] = { "Oversampling Antialiasing", "Hardware Antialiasing" };

    DisplayString("        Antialiasing Method: ", lCameraAntialiasingMethods[pCamera->AntialiasingMethod.Get()]);

    // This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
    if (pCamera->AntialiasingMethod.Get() == KFbxCamera::eOVERSAMPLING_ANTIALIASING)
    {
        DisplayInt("        Number of Samples: ", pCamera->FrameSamplingCount.Get());
    }

    char* lCameraSamplingTypes[] = { "Uniform", "Stochastic" };

    DisplayString("        Sampling Type: ", lCameraSamplingTypes[pCamera->FrameSamplingType.Get()]);
    DisplayBool("        Use Accumulation Buffer: ", pCamera->UseAccumulationBuffer.Get());
    DisplayBool("        Use Depth of Field: ", pCamera->UseDepthOfField.Get());

    char* lCameraFocusDistanceSources[] = { "Camera Interest", "Specific Distance" };

    DisplayString("        Focus Distance Source: ", lCameraFocusDistanceSources[pCamera->FocusSource.Get()]);

    // This parameter is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
    if (pCamera->FocusSource.Get() == KFbxCamera::eSPECIFIC_DISTANCE)
    {
        DisplayDouble("        Specific Distance: ", pCamera->FocusDistance.Get());
    }

    DisplayDouble("        Focus Angle: ", pCamera->FocusAngle.Get(), " degrees");
}


void DisplayDefaultAnimationValues(KFbxCamera* pCamera)
{
    DisplayString("    Default Animation Values");

    DisplayDouble("        Default Field of View: ", pCamera->FieldOfView.Get());
    DisplayDouble("        Default Field of View X: ", pCamera->FieldOfViewX.Get());
    DisplayDouble("        Default Field of View Y: ", pCamera->FieldOfViewY.Get());
    DisplayDouble("        Default Optical Center X: ", pCamera->OpticalCenterX.Get());
    DisplayDouble("        Default Optical Center Y: ", pCamera->OpticalCenterY.Get());
    DisplayDouble("        Default Roll: ", pCamera->Roll.Get());
}

