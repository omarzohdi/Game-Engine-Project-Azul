/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include <stdio.h>


// Local functions prototype.
void DisplayHierarchy(KFbxNode *pParent, KFbxNode* pNode, int pDepth);

static int displayHierarchyIndex; 

void DisplayHierarchy(KFbxScene* pScene)
{
    int i;
    KFbxNode* lRootNode = pScene->GetRootNode();

    for( i = 0; i < lRootNode->GetChildCount(); i++)
    {
       displayHierarchyIndex = 0;
        DisplayHierarchy(lRootNode, lRootNode->GetChild(i), 0);
    }
}



void DisplayHierarchy(KFbxNode *pParent, KFbxNode* pNode, int pDepth)
{
    KString lString;
    int i;


    // Big Hack to remove "Humanoid:" from each name
    char buff[64];
    char buff2[64];
    char origBuff[64];

    // Get original
    strcpy( origBuff, pNode->GetName() );


	for( int j = 0; j < (int)(strlen(origBuff)+1); j++)
	{
		if ( origBuff[j] == 0x20 )
		{
			buff[j] = 0x5f;
		}
		else
		{
			buff[j] = origBuff[j];
		}
	}
    


    // Get original
    strcpy( origBuff, pParent->GetName() );


	for( int j = 0; j < (int)(strlen(origBuff)+1); j++)
	{
		if ( origBuff[j] == 0x20 )
		{
			buff2[j] = 0x5f;
		}
		else
		{
			buff2[j] = origBuff[j];
		}
	}
    
	// Keenan HACK
	if(displayHierarchyIndex > 19)
		return;
	if( !strncmp( buff, "Bip01_Ctrl:",11) )
		return;


    lString += "PyramidObject *p";
    lString += buff;
    lString += " = new PyramidObject( \"xxx\", pPyramid );\n";

    lString += "p";
    lString += buff;
    lString += "->setIndex(";
    lString += displayHierarchyIndex;
    lString += ");\n";

    lString += "p";
    lString += buff;
    lString += "->setName(\"";
    lString += buff;
    lString += "\");\n";

    lString += "tree->insert( p";
    lString += buff;
    lString += ", p";
    lString += buff2;
    lString += " );\n\n";



    printf(lString.Buffer());

   displayHierarchyIndex++;

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayHierarchy(pNode, pNode->GetChild(i), pDepth + 1);
    }
}


