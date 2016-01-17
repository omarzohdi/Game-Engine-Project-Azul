/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_GLOBAL_SETTINGS_H
#define _DISPLAY_GLOBAL_SETTINGS_H

#include <fbxfilesdk/fbxfilesdk_def.h>

// Forward declaration.
class FBXFILESDK_NAMESPACE::KFbxGlobalSettings;

void DisplayGlobalLightSettings(FBXFILESDK_NAMESPACE::KFbxGlobalSettings* pGlobalSettings);
void DisplayGlobalCameraSettings(FBXFILESDK_NAMESPACE::KFbxGlobalSettings* pGlobalSettings);
void DisplayGlobalTimeSettings(FBXFILESDK_NAMESPACE::KFbxGlobalSettings* pGlobalSettings);


#endif // #ifndef _DISPLAY_GLOBAL_SETTINGS_H


