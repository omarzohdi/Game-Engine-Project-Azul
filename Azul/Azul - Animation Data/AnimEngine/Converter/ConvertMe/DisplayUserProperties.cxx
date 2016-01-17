/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"

#include <stdio.h>


void DisplayUserProperties(KFbxObject* pObject)
{
    int lCount = 0;
    KString lTitleStr = "    Property Count: ";

    KFbxProperty lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(KFbxProperty::eUSER))
            lCount++;

        lProperty = pObject->GetNextProperty(lProperty);
    }

    if (lCount == 0)
        return; // there are no user properties to display

    DisplayInt(lTitleStr.Buffer(), lCount);

    lProperty = pObject->GetFirstProperty();
    int i = 0;
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(KFbxProperty::eUSER))
        {
            DisplayInt("        Property ", i);
            KString lString = lProperty.GetLabel();
            DisplayString("            Display Name: ", lString.Buffer());
            lString = lProperty.GetName();
            DisplayString("            Internal Name: ", lString.Buffer());
            DisplayString("            Type: ", lProperty.GetPropertyDataType().GetName());
            if (lProperty.HasMinLimit()) DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
            if (lProperty.HasMaxLimit()) DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
            DisplayBool  ("            Is Animatable: ", lProperty.GetFlag(KFbxProperty::eANIMATABLE));
            DisplayBool  ("            Is Temporary: ", lProperty.GetFlag(KFbxProperty::eTEMPORARY));
            
			KFbxDataType lPropertyDataType=lProperty.GetPropertyDataType();

			// BOOL
			if (lPropertyDataType.GetType() == eBOOL1)
            {
                DisplayBool("            Default Value: ", KFbxGet<bool>(lProperty));
			}
			// REAL
			else if (lPropertyDataType.GetType() == eDOUBLE1 || lPropertyDataType.GetType() == eFLOAT1)
			{
                DisplayDouble("            Default Value: ",KFbxGet<double>(lProperty));
			}
			// COLOR
			else if (lPropertyDataType.Is(DTColor3) || lPropertyDataType.Is(DTColor4))
            {
				KFbxColor lDefault;
                char      lBuf[64];

                lDefault=KFbxGet <KFbxColor> (lProperty);
                sprintf(lBuf, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
                DisplayString("            Default Value: ", lBuf);
            }
			// INTEGER
			else if (lPropertyDataType.GetType() == eINTEGER1)
			{
                DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
			}
			// VECTOR
			else if(lPropertyDataType.GetType() == eDOUBLE3 || lPropertyDataType.GetType() == eDOUBLE4)
			{
				fbxDouble3 lDefault;
                char   lBuf[64];

                lDefault = KFbxGet <fbxDouble3> (lProperty);
                sprintf(lBuf, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
                DisplayString("            Default Value: ", lBuf);
            }
			// LIST
			else if (lPropertyDataType.GetType() == eENUM)
			{
                DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
			}
			// UNIDENTIFIED
            else
			{
                DisplayString("            Default Value: UNIDENTIFIED");
            }
            i++;
        }

        lProperty = pObject->GetNextProperty(lProperty);
    }
}

