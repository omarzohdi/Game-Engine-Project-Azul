/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_COMMON_H
#define _DISPLAY_COMMON_H

#include <fbxfilesdk/fbxfilesdk_def.h>

void DisplayMetaDataConnections(FBXFILESDK_NAMESPACE::KFbxObject* pNode);
void DisplayString(const char* pHeader, const char* pValue  = "", const char* pSuffix  = "");
void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix  = "");
void DisplayInt(const char* pHeader, int pValue, const char* pSuffix  = "");
void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix  = "");
void Display2DVector(const char* pHeader, FBXFILESDK_NAMESPACE::KFbxVector2 pValue, const char* pSuffix  = "");
void Display3DVector(const char* pHeader, FBXFILESDK_NAMESPACE::KFbxVector4 pValue, const char* pSuffix  = "");
void DisplayColor(const char* pHeader, FBXFILESDK_NAMESPACE::KFbxColor pValue, const char* pSuffix  = "");
void Display4DVector(const char* pHeader, FBXFILESDK_NAMESPACE::KFbxVector4 pValue, const char* pSuffix  = "");


#endif // #ifndef _DISPLAY_COMMON_H


