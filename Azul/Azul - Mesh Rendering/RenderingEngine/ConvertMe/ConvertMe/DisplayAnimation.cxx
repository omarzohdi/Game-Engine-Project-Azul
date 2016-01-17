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


void DisplayAnimation(KFbxAnimStack* pAnimStack, KFbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(KFbxAnimLayer* pAnimLayer, KFbxNode* pNode, bool isSwitcher = false);
void DisplayChannels(KFbxNode* pNode, KFbxAnimLayer* pAnimLayer, 
                     void (*DisplayCurve) (KFCurve *pCurve), 
                     void (*DisplayListCurve) (KFCurve *pCurve, KFbxProperty* pProperty), 
                     bool isSwitcher);
void DisplayCurveKeys(KFCurve *pCurve);
void DisplayCurveDefault(KFCurve *pCurve);
void DisplayListCurveKeys(KFCurve *pCurve, KFbxProperty* pProperty);
void DisplayListCurveDefault(KFCurve *pCurve, KFbxProperty* pProperty);

void DisplayAnimation(KFbxScene* pScene)
{
    int i;
    for (i = 0; i < pScene->GetSrcObjectCount(FBX_TYPE(KFbxAnimStack)); i++)
    {
        KFbxAnimStack* lAnimStack = KFbxCast<KFbxAnimStack>(pScene->GetSrcObject(FBX_TYPE(KFbxAnimStack), i));

        KString lOutputString = "Animation Stack Name: ";
        lOutputString += lAnimStack->GetName();
        lOutputString += "\n\n";
        printf(lOutputString);

        DisplayAnimation(lAnimStack, pScene->GetRootNode(), true);
        DisplayAnimation(lAnimStack, pScene->GetRootNode());
    }
}

void DisplayAnimation(KFbxAnimStack* pAnimStack, KFbxNode* pNode, bool isSwitcher)
{
    int l;
    int nbAnimLayers = pAnimStack->GetMemberCount(FBX_TYPE(KFbxAnimLayer));
    KString lOutputString;

    lOutputString = "Animation stack contains ";
    lOutputString += nbAnimLayers;
    lOutputString += " Animation Layer(s)\n";
    printf(lOutputString);

    for (l = 0; l < nbAnimLayers; l++)
    {
        KFbxAnimLayer* lAnimLayer = pAnimStack->GetMember(FBX_TYPE(KFbxAnimLayer), l);

        lOutputString = "AnimLayer ";
        lOutputString += l;
        lOutputString += "\n";
        printf(lOutputString);

        DisplayAnimation(lAnimLayer, pNode, isSwitcher);
    }
}

void DisplayAnimation(KFbxAnimLayer* pAnimLayer, KFbxNode* pNode, bool isSwitcher)
{
    int lModelCount;
    KString lOutputString;

    lOutputString = "     Node Name: ";
    lOutputString += pNode->GetName();
    lOutputString += "\n\n";
    printf(lOutputString);

    DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
    printf ("\n");

    for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
        DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
    }
}


void DisplayChannels(KFbxNode* pNode, KFbxAnimLayer* pAnimLayer, void (*DisplayCurve) (KFCurve *pCurve), void (*DisplayListCurve) (KFCurve *pCurve, KFbxProperty* pProperty), bool isSwitcher)
{
    KFbxAnimCurve* lAnimCurve = NULL;

    // Display general curves.
    if (!isSwitcher)
    {
        lAnimCurve = pNode->LclTranslation.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_T_X);
        if (lAnimCurve)
        {
            printf("        TX\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = pNode->LclTranslation.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_T_Y);
        if (lAnimCurve)
        {
            printf("        TY\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = pNode->LclTranslation.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_T_Z);
        if (lAnimCurve)
        {
            printf("        TZ\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }

        lAnimCurve = pNode->LclRotation.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_R_X);
        if (lAnimCurve)
        {
            printf("        RX\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = pNode->LclRotation.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_R_Y);
        if (lAnimCurve)
        {
            printf("        RY\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = pNode->LclRotation.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_R_Z);
        if (lAnimCurve)
        {
            printf("        RZ\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }

        lAnimCurve = pNode->LclScaling.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_S_X);
        if (lAnimCurve)
        {
            printf("        SX\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }    
        lAnimCurve = pNode->LclScaling.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_S_Y);
        if (lAnimCurve)
        {
            printf("        SY\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = pNode->LclScaling.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_S_Z);
        if (lAnimCurve)
        {
            printf("        SZ\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
    }

    // Display curves specific to a light or marker.
    KFbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

    if (lNodeAttribute)
    {
        lAnimCurve = lNodeAttribute->Color.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_COLOR_RED);
        if (lAnimCurve)
        {
            printf("        Red\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = lNodeAttribute->Color.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_COLOR_GREEN);
        if (lAnimCurve)
        {
            printf("        Green\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }
        lAnimCurve = lNodeAttribute->Color.GetCurve<KFbxAnimCurve>(pAnimLayer, KFCURVENODE_COLOR_BLUE);
        if (lAnimCurve)
        {
            printf("        Blue\n");
            DisplayCurve(lAnimCurve->GetKFCurve());
        }

        // Display curves specific to a light.
        KFbxLight* light = pNode->GetLight();
        if (light)
        {
            lAnimCurve = light->Intensity.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Intensity\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = light->ConeAngle.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Cone Angle\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = light->Fog.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Fog\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }
        }

        // Display curves specific to a camera.
        KFbxCamera* camera = pNode->GetCamera();
        if (camera)
        {
            lAnimCurve = camera->FieldOfView.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Field of View\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = camera->FieldOfViewX.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Field of View X\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = camera->FieldOfViewY.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Field of View Y\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = camera->OpticalCenterX.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if (lAnimCurve)
            {
                printf("        Optical Center X\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = camera->OpticalCenterY.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if(lAnimCurve)
            {
                printf("        Optical Center Y\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }

            lAnimCurve = camera->Roll.GetCurve<KFbxAnimCurve>(pAnimLayer);
            if(lAnimCurve)
            {
                printf("        Roll\n");
                DisplayCurve(lAnimCurve->GetKFCurve());
            }
        }

        // Display curves specific to a geometry.
        if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMESH ||
            lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURB ||
            lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::ePATCH)
        {
            KFbxGeometry* lGeometry = (KFbxGeometry*) lNodeAttribute;

			int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(KFbxDeformer::eBLENDSHAPE);
			for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
			{
				KFbxBlendShape* lBlendShape = (KFbxBlendShape*)lGeometry->GetDeformer(lBlendShapeIndex, KFbxDeformer::eBLENDSHAPE);

				int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
				for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
				{
					KFbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
					const char* lChannelName = lChannel->GetName();

					lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer, true);
					if(lAnimCurve)
					{
						printf("        Shape %s\n", lChannelName);
						DisplayCurve(lAnimCurve->GetKFCurve());
					}
				}
			}
        }
    }

    // Display curves specific to properties
    KFbxProperty lProperty = pNode->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(KFbxProperty::eUSER))
        {
            KString lFbxFCurveNodeName  = lProperty.GetName();
            KFbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

            if (!lCurveNode){
                lProperty = pNode->GetNextProperty(lProperty);
                continue;
            }

            KFbxDataType lDataType = lProperty.GetPropertyDataType();
			if (lDataType.GetType() == eBOOL1 || lDataType.GetType() == eDOUBLE1 || lDataType.GetType() == eFLOAT1 || lDataType.GetType() == eINTEGER1)
            {
                KString lMessage;

                lMessage =  "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                };

                DisplayString(lMessage.Buffer());

                for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
                {
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                        DisplayCurve(lAnimCurve->GetKFCurve());
                }
            }
			else if(lDataType.GetType() == eDOUBLE3 || lDataType.GetType() == eDOUBLE4 || lDataType.Is(DTColor3) || lDataType.Is(DTColor4))
            {
				char* lComponentName1 = (lDataType.Is(DTColor3) ||lDataType.Is(DTColor4)) ? (char*)KFCURVENODE_COLOR_RED : (char*)"X";
                char* lComponentName2 = (lDataType.Is(DTColor3) ||lDataType.Is(DTColor4)) ? (char*)KFCURVENODE_COLOR_GREEN : (char*)"Y";
                char* lComponentName3 = (lDataType.Is(DTColor3) ||lDataType.Is(DTColor4)) ? (char*)KFCURVENODE_COLOR_BLUE  : (char*)"Z";
                KString      lMessage;
                
				lMessage =  "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                }
                DisplayString(lMessage.Buffer());

                for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
                {
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                    {
                        DisplayString("        Component ", lComponentName1);
                        DisplayCurve(lAnimCurve->GetKFCurve());
                    }
                }

                for( int c = 0; c < lCurveNode->GetCurveCount(1U); c++ )
                {
                    lAnimCurve = lCurveNode->GetCurve(1U, c);
                    if (lAnimCurve)
                    {
                        DisplayString("        Component ", lComponentName2);
                        DisplayCurve(lAnimCurve->GetKFCurve());
                    }
                }

                for( int c = 0; c < lCurveNode->GetCurveCount(2U); c++ )
                {
                    lAnimCurve = lCurveNode->GetCurve(2U, c);
                    if (lAnimCurve)
                    {
                        DisplayString("        Component ", lComponentName3);
                        DisplayCurve(lAnimCurve->GetKFCurve());
                    }
                }
            }
			else if (lDataType.GetType() == eENUM)
            {
                KString lMessage;

				lMessage =  "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                };
                DisplayString(lMessage.Buffer());

                for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
                {
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                        DisplayListCurve(lAnimCurve->GetKFCurve(), &lProperty);
                }                
            }
        }

        lProperty = pNode->GetNextProperty(lProperty);
    } // while

}


static int InterpolationFlagToIndex(int flags)
{
    if ((flags&KFCURVE_INTERPOLATION_CONSTANT)==KFCURVE_INTERPOLATION_CONSTANT)
        return 1;
    if ((flags&KFCURVE_INTERPOLATION_LINEAR)==KFCURVE_INTERPOLATION_LINEAR)
        return 2;
    if ((flags&KFCURVE_INTERPOLATION_CUBIC)==KFCURVE_INTERPOLATION_CUBIC)
        return 3;
    return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
    if ((flags&KFCURVE_CONSTANT_STANDARD)==KFCURVE_CONSTANT_STANDARD)
        return 1;
    if ((flags&KFCURVE_CONSTANT_NEXT)==KFCURVE_CONSTANT_NEXT)
        return 2;
    return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
    if ((flags&KFCURVE_TANGEANT_AUTO) == KFCURVE_TANGEANT_AUTO)
        return 1;
    if ((flags&KFCURVE_TANGEANT_AUTO_BREAK)==KFCURVE_TANGEANT_AUTO_BREAK)
        return 2;
    if ((flags&KFCURVE_TANGEANT_TCB) == KFCURVE_TANGEANT_TCB)
        return 3;
    if ((flags&KFCURVE_TANGEANT_USER) == KFCURVE_TANGEANT_USER)
        return 4;
    if ((flags&KFCURVE_GENERIC_BREAK) == KFCURVE_GENERIC_BREAK)
        return 5;
    if ((flags&KFCURVE_TANGEANT_BREAK) ==KFCURVE_TANGEANT_BREAK)
        return 6;
    return 0;
}

static int TangentweightFlagToIndex(int flags)
{
    if ((flags&KFCURVE_WEIGHTED_NONE) == KFCURVE_WEIGHTED_NONE)
        return 1;
    if ((flags&KFCURVE_WEIGHTED_RIGHT) == KFCURVE_WEIGHTED_RIGHT)
        return 2;
    if ((flags&KFCURVE_WEIGHTED_NEXT_LEFT) == KFCURVE_WEIGHTED_NEXT_LEFT)
        return 3;
    return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
    if ((flags&KFCURVE_VELOCITY_NONE) == KFCURVE_VELOCITY_NONE)
        return 1;
    if ((flags&KFCURVE_VELOCITY_RIGHT) == KFCURVE_VELOCITY_RIGHT)
        return 2;
    if ((flags&KFCURVE_VELOCITY_NEXT_LEFT) == KFCURVE_VELOCITY_NEXT_LEFT)
        return 3;
    return 0;
}

void DisplayCurveKeys(KFCurve *pCurve)
{
    static char* interpolation[] = { "?", "constant", "linear", "cubic"};
    static char* constantMode[] =  { "?", "Standard", "Next" };
    static char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
    static char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

    KTime   lKeyTime;
    float   lKeyValue;
    char    lTimeString[256];
    KString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString);
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " [ ";
        lOutputString += interpolation[ InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount)) ];
        if ((pCurve->KeyGetInterpolation(lCount)&KFCURVE_INTERPOLATION_CONSTANT) == KFCURVE_INTERPOLATION_CONSTANT)
        {
            lOutputString += " | ";
            lOutputString += constantMode[ ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount)) ];
        }
        else if ((pCurve->KeyGetInterpolation(lCount)&KFCURVE_INTERPOLATION_CUBIC) == KFCURVE_INTERPOLATION_CUBIC)
        {
            lOutputString += " | ";
            lOutputString += cubicMode[ TangentmodeFlagToIndex(pCurve->KeyGetTangeantMode(lCount)) ];
            lOutputString += " | ";
            lOutputString += tangentWVMode[ TangentweightFlagToIndex(pCurve->KeyGetTangeantWeightMode(lCount)) ];
            lOutputString += " | ";
            lOutputString += tangentWVMode[ TangentVelocityFlagToIndex(pCurve->KeyGetTangeantVelocityMode(lCount)) ];
        }
        lOutputString += " ]";
        lOutputString += "\n";
        printf (lOutputString);
    }
}

void DisplayCurveDefault(KFCurve *pCurve)
{
    KString lOutputString;

    lOutputString = "            Default Value: ";
    lOutputString += static_cast<float> (pCurve->GetValue());
    lOutputString += "\n";
    printf (lOutputString);
}

void DisplayListCurveKeys(KFCurve *pCurve, KFbxProperty* pProperty)
{
    KTime   lKeyTime;
    int     lKeyValue;
    char    lTimeString[256];
    KString lListValue;
    KString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString);
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " (";
        lOutputString += pProperty->GetEnumValue(lKeyValue);
        lOutputString += ")";

        lOutputString += "\n";
        printf (lOutputString);
    }
}

void DisplayListCurveDefault(KFCurve *pCurve, KFbxProperty*)
{
    DisplayCurveDefault(pCurve);
}
