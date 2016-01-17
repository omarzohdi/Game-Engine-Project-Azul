/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include <stdio.h>
#include "DisplayCommon.h"


void DisplayProperties(KFbxObject* pObject);
void DisplayGenericInfo(KFbxNode* pNode, int pDepth);
void DisplayGenericInfo(KFbxScene* pScene)
{
    int i;
    KFbxNode* lRootNode = pScene->GetRootNode();

    for( i = 0; i < lRootNode->GetChildCount(); i++)
    {
        DisplayGenericInfo(lRootNode->GetChild(i), 0);
    }

	//Other objects directly connected onto the scene
	for( i = 0; i < pScene->GetSrcObjectCount(FBX_TYPE(KFbxObject)); ++i )
	{
		DisplayProperties(pScene->GetSrcObject(FBX_TYPE(KFbxObject), i));
	}
}


void DisplayGenericInfo(KFbxNode* pNode, int pDepth)
{
    KString lString;
    int i;

    for(i = 0; i < pDepth; i++)
    {
        lString += "     ";
    }

    lString += pNode->GetName();
    lString += "\n";

    DisplayString(lString.Buffer());

    //Display generic info about that Node
    DisplayProperties(pNode);
    DisplayString("");
    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayGenericInfo(pNode->GetChild(i), pDepth + 1);
    }
}

void DisplayProperties(KFbxObject* pObject)
{

    DisplayString("Name: ", (char *)pObject->GetName());

    // Display all the properties
    int i,  lCount = 0;
    KFbxProperty lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid())
    {
        lCount++;
        lProperty = pObject->GetNextProperty(lProperty);
    }

    KString lTitleStr = "    Property Count: ";

    if (lCount == 0)
        return; // there are no properties to display

    DisplayInt(lTitleStr.Buffer(), lCount);

    i=0;
	lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid())
    {
        // exclude user properties

        KString lString;
        DisplayInt("        Property ", i);
        lString = lProperty.GetLabel();
        DisplayString("            Display Name: ", lString.Buffer());
        lString = lProperty.GetName();
        DisplayString("            Internal Name: ", lString.Buffer());
        lString = lProperty.GetPropertyDataType().GetName();
        DisplayString("            Type: ",lString.Buffer());
        if (lProperty.HasMinLimit()) DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
        if (lProperty.HasMaxLimit()) DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
        DisplayBool  ("            Is Animatable: ", lProperty.GetFlag(KFbxProperty::eANIMATABLE));
        DisplayBool  ("            Is Temporary: ", lProperty.GetFlag(KFbxProperty::eTEMPORARY));


        switch (lProperty.GetPropertyDataType().GetType())
        {
        case eBOOL1:
            DisplayBool("            Default Value: ", KFbxGet<bool>(lProperty));
            break;

        case eDOUBLE1:
            DisplayDouble("            Default Value: ",KFbxGet<double>(lProperty));
            break;

        case eDOUBLE4:
            {
                KFbxColor lDefault;
                char      lBuf[64];

                lDefault=KFbxGet <KFbxColor> (lProperty);
                sprintf(lBuf, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
                DisplayString("            Default Value: ", lBuf);
            }
            break;

        case eINTEGER1:
            DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
            break;

        case eDOUBLE3:
            {
                fbxDouble3 lDefault;
                char   lBuf[64];

                lDefault = KFbxGet <fbxDouble3> (lProperty);
                sprintf(lBuf, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
                DisplayString("            Default Value: ", lBuf);
            }
            break;

        //case DTEnum:
        //    DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
        //    break;

        case eFLOAT1:
            DisplayDouble("            Default Value: ", KFbxGet <float> (lProperty));
            break;
        case eSTRING:
            lString = (KFbxGet <KString> (lProperty));
            DisplayString("            Default Value: ", lString.Buffer());
            break;

        default:
            DisplayString("            Default Value: UNIDENTIFIED");
            break;
        }
        i++;
        lProperty = pObject->GetNextProperty(lProperty);
    }
}

