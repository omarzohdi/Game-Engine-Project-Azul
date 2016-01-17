/****************************************************************************************

   Copyright (C) 2011 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplayTexture.h"
#include "DisplayMaterial.h"
#include "DisplayLink.h"
#include "DisplayShape.h"

#include <stdio.h>


void DisplayNurb(KFbxNode* pNode)
{
    KFbxNurb* lNurb = (KFbxNurb*) pNode->GetNodeAttribute ();
    int i;

    DisplayString("Nurb Name: ", (char *) pNode->GetName());
    DisplayMetaDataConnections(lNurb);

    char* lSurfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

    DisplayString("    Surface Mode: ", lSurfaceModes[lNurb->GetSurfaceMode()]);

    int lControlPointsCount = lNurb->GetControlPointsCount();
    KFbxVector4* lControlPoints = lNurb->GetControlPoints();

    for (i = 0; i < lControlPointsCount; i++)
    {
        DisplayInt("    Control Point ", i);
        Display3DVector("        Coordinates: ", lControlPoints[i]);
        DisplayDouble("        Weight: ", lControlPoints[i][3]);
    }

    char* lNurbTypes[] = { "Periodic", "Closed", "Open" };

    DisplayString("    Nurb U Type: ", lNurbTypes[lNurb->GetNurbUType()]);
    DisplayInt("    U Count: ", lNurb->GetUCount());
    DisplayString("    Nurb V Type: ", lNurbTypes[lNurb->GetNurbVType()]);
    DisplayInt("    V Count: ", lNurb->GetVCount());
    DisplayInt("    U Order: ", lNurb->GetUOrder());
    DisplayInt("    V Order: ", lNurb->GetVOrder());
    DisplayInt("    U Step: ", lNurb->GetUStep());
    DisplayInt("    V Step: ", lNurb->GetVStep());

    KString lString;
    int lUKnotCount = lNurb->GetUKnotCount();
    int lVKnotCount = lNurb->GetVKnotCount();
    int lUMultiplicityCount = lNurb->GetUCount();
    int lVMultiplicityCount = lNurb->GetVCount();
    double* lUKnotVector = lNurb->GetUKnotVector();
    double* lVKnotVector = lNurb->GetVKnotVector();
    int* lUMultiplicityVector = lNurb->GetUMultiplicityVector();
    int* lVMultiplicityVector = lNurb->GetVMultiplicityVector();

    lString = "    U Knot Vector: ";

    for (i = 0; i < lUKnotCount; i++)
    {
        lString += (float) lUKnotVector[i];

        if (i < lUKnotCount - 1)
        {
            lString += ", ";
        }
    }

    lString += "\n";
    printf(lString);

    lString = "    V Knot Vector: ";

    for (i = 0; i < lVKnotCount; i++)
    {
        lString += (float) lVKnotVector[i];

        if (i < lVKnotCount - 1)
        {
            lString += ", ";
        }
    }

    lString += "\n";
    printf(lString);

    lString = "    U Multiplicity Vector: ";

    for (i = 0; i < lUMultiplicityCount; i++)
    {
        lString += lUMultiplicityVector[i];

        if (i < lUMultiplicityCount - 1)
        {
            lString += ", ";
        }
    }

    lString += "\n";
    printf(lString);

    lString = "    V Multiplicity Vector: ";

    for (i = 0; i < lVMultiplicityCount; i++)
    {
        lString += lVMultiplicityVector[i];

        if (i < lVMultiplicityCount - 1)
        {
            lString += ", ";
        }
    }

    lString += "\n";
    printf(lString);

    DisplayString("");

    DisplayTexture(lNurb);
    DisplayMaterial(lNurb);
    DisplayLink(lNurb);
    DisplayShape(lNurb);
}
