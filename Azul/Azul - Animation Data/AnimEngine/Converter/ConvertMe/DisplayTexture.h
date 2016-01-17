/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_TEXTURE_H
#define _DISPLAY_TEXTURE_H

#include <fbxfilesdk/fbxfilesdk_def.h>
#include <fbxfilesdk/fbxfilesdk_nsuse.h>

// Forward declaration.
class FBXFILESDK_NAMESPACE::KFbxGeometry;


void FindAndDisplayTextureInfoByProperty(KFbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
void DisplayTexture(FBXFILESDK_NAMESPACE::KFbxGeometry* pGeometry);
void DisplayTextureInfo(KFbxTexture* pTexture, int pBlendMode);

#endif // #ifndef _DISPLAY_TEXTURE_H


