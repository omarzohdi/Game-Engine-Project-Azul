/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_CAMERA_H
#define _DISPLAY_CAMERA_H

#include <fbxfilesdk/fbxfilesdk_def.h>

// Forward declaration.
class FBXFILESDK_NAMESPACE::KFbxNode;
class FBXFILESDK_NAMESPACE::KFbxCamera;

void DisplayCamera(FBXFILESDK_NAMESPACE::KFbxCamera* pCamera, char* pName, FBXFILESDK_NAMESPACE::KFbxNode* pTargetNode = NULL, FBXFILESDK_NAMESPACE::KFbxNode* pTargetUpNode = NULL);
void DisplayCamera(FBXFILESDK_NAMESPACE::KFbxNode* pNode);


#endif // #ifndef _DISPLAY_CAMERA_H


