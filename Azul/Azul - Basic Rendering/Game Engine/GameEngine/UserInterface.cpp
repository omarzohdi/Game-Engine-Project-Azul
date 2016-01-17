#include "OpenGLWrapper.h"
#include "MathEngine.h"
#include "Camera.h"
#include "UserInterface.h"
#include "GraphicsManager.h"
#include "PyramidObject.h"
#include "Pyramid.h"
#include "CubeObject.h"
#include "SqrAPrismObject.h"

#define UNUSED_VAR(_x)  _x;

extern Camera *pCamera;
int state = 0;
extern GLuint PtextureID;
extern GLuint CtextureID;
extern GLuint StextureID;

extern int PyramidTexture;
extern int CubeTexture;
extern int SqrAPrismTexture;
extern bool SphereRender;
bool deleteMode = false;
extern		PyramidObject *PT;
bool all_dead = false;

#define ANGLE 0.01f

void KeyPressFunc (unsigned char key, int _x, int _y)
{
	GraphicsObjectManager *goMgr = GraphicsObjectManager::getInstance();

	if (32 == key)
	{
		if (0 == state)
		{
			glEnable(GL_BLEND);
			glEnable(GL_LINE_SMOOTH);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
			state = 1;
		}

		else if (1 == state)
		{
			glDisable(GL_BLEND);
			glDisable(GL_LINE_SMOOTH);
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_CULL_FACE);
			state=0;
		}
		
	}

	if (13 == key)
	{
		if (1 == PyramidTexture)
			PyramidTexture = 2;
		else if (2 == PyramidTexture)
			PyramidTexture = 3;
		else if(3 == PyramidTexture)
			PyramidTexture = 1;

		PtextureID = 0;

		if (1 == CubeTexture)
			CubeTexture = 2;
		else if(2 == CubeTexture)
			CubeTexture = 3;
		else if(3 == CubeTexture)
			CubeTexture = 1;

		CtextureID = 0;

		if (1 == SqrAPrismTexture)
			SqrAPrismTexture = 2;
		else if(2 == SqrAPrismTexture)
			SqrAPrismTexture = 3;
		else if(3 == SqrAPrismTexture)
			SqrAPrismTexture = 1;

		StextureID = 0;
	}

	bool cameraUpdate = false;
	bool updateLookAt = true;

	Vect position;
	Vect dir;
	Vect lookAt;
	pCamera->getPos(position);
	dir = pCamera->vDir;

	if (119 == key) ////W
	{
		position -= dir;
		cameraUpdate = true;
	}
	if (115 == key)//////S
	{
		position += dir;
		cameraUpdate = true;
	}
	if (97 == key) //////A
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[x] -= 0.15f;
		pCamera->setPos( vTmp );
	}
	if (100 == key)/////D
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[x] += 0.15f;
		pCamera->setPos( vTmp );
	}
	if (114 == key)/////r
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[z] -= 0.15f;
		pCamera->setPos( vTmp );
	}
	if (102 == key)/////f
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[z] += 0.15f;
		pCamera->setPos( vTmp );
	}


	if ( 9 == key)
	{
		if (SphereRender)
		{
			SphereRender = false;
		}
		else 
		{
			SphereRender = true;
		}
	}

	if(127 == key)
	{
		if (!deleteMode)
		{
			goMgr->removeObject(PT);
			deleteMode = true;
		}
	}

	if (61 == key)
	{
		if (!deleteMode)
		{
			PyramidObject * lastNode = PT;

			while(lastNode->getChild() != 0)
			{
				lastNode =(PyramidObject *)lastNode->getChild();
			}

			Vect Pos;
			lastNode->getCurrPos(Pos);
			Pos.set(Pos[x],Pos[y],Pos[z]-6.0f);

			Pyramid * pPyramid= new Pyramid();
			PyramidObject * PT2 = new PyramidObject( "pyramid_AT" ,pPyramid);
			PT2->loadObjectData();
			PT2->setStartPos(Pos);
			PT2->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
			PT2->setLightColor( Vect(0.7f, 1.0f, 4.0f, 1.0f)*2.0f );
			PT2->setRotationSpeeds(-0.001f,0.005f,0);
			
			
			goMgr->addObjectToObject(PT2,lastNode);
		}
	}

	if (45 == key)
	{
		if (!deleteMode)
		{
			if (PT->getChild() != 0)
			{
				PyramidObject * lastNode = PT;

				while(lastNode->getChild() != 0)
				{
					lastNode =(PyramidObject *)lastNode->getChild();
				}

				goMgr->removeObject(lastNode);
			}
		}

	}

	if (53 == key)
	{
		int count = 0;
		PyramidObject * TempNode = 0;

		if (!deleteMode)
		{
			if (PT->getChild() != 0)
			{
				PyramidObject * lastNode = PT;

				while(lastNode->getChild() != 0)
				{
					lastNode =(PyramidObject *)lastNode->getChild();
					count++;
					if (count% 5 == 0)
					{
						TempNode = lastNode;
					}
					
				}

				if (TempNode != 0)
					goMgr->removeObject(TempNode);
			}
		}

	}

	if (50 == key)
	{
		int count = 0;
		PyramidObject * TempNode = 0;

		if (!deleteMode)
		{
			if (PT->getChild() != 0)
			{
				PyramidObject * lastNode = PT;

				while(lastNode->getChild() != 0)
				{
					lastNode =(PyramidObject *)lastNode->getChild();
					count++;
					if (count % 2 == 0)
					{
						TempNode = lastNode;
					}
				}

				if (TempNode != 0)
					goMgr->removeObject(TempNode);
			}
		}

	}



	if(cameraUpdate)
	{
		if(updateLookAt)
		{
			lookAt = position+(-dir);
		}
		else
		{
			lookAt = -dir;
		}
		pCamera->setOrientAndPosition(Vect(0,1,0),lookAt,position);
	}


	glutPostRedisplay();
}

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int _x, int _y)
{
   // Not using these variables
   UNUSED_VAR(_x);
   UNUSED_VAR(_y);

   GraphicsObjectManager *goMgr = GraphicsObjectManager::getInstance();

   	bool cameraUpdate = false;
	bool updateLookAt = true;

	Vect position;
	Vect dir;
	Vect lookAt;
	pCamera->getPos(position);
	dir = pCamera->vDir;

	if(key == GLUT_KEY_INSERT)
	{
		if (deleteMode)
		{
			goMgr->addObject(PT);
			deleteMode = false;
		}
			
	}


	if(key == GLUT_KEY_UP)
	{
		Matrix rotX(ROT_X,ANGLE);
		dir *= rotX;
		cameraUpdate = true;
	}

	if(key == GLUT_KEY_DOWN)
	{
		Matrix rotX(ROT_X,-ANGLE);
		dir *= rotX;
		cameraUpdate = true;
	}
	
	if(key == GLUT_KEY_LEFT)
	{
		Matrix rotY(ROT_Y,ANGLE);
		dir *= rotY;
		cameraUpdate = true;
	}
    
	if(key == GLUT_KEY_RIGHT)
	{
		Matrix rotY(ROT_Y,-ANGLE);
		dir *= rotY;
		cameraUpdate = true;
	}
	
	if (key == GLUT_KEY_PAGE_DOWN)
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[y] -= 0.15f;
		pCamera->setPos( vTmp );
	}
	
	if (key == GLUT_KEY_PAGE_UP)
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[y] += 0.15f;
		pCamera->setPos( vTmp );
	}

	if(key == GLUT_KEY_HOME)
	{
		pCamera->startOrbit();	
	}

	if(key == GLUT_KEY_END)
	{
		pCamera->stopOrbit();

	}

	if(cameraUpdate)
	{
		if(updateLookAt)
		{
			lookAt = position+(-dir);
		}
		else
		{
			lookAt = -dir;
		}
		pCamera->setOrientAndPosition(Vect(0,1,0),lookAt,position);
	}

	glutPostRedisplay();

}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{

 //  pCamera = new Camera();

   // set the view port
   pCamera->setViewport(0, 0, w, h);
   // set the perspective
   pCamera->setPerspective( 35.0f, float(w)/float(h), 1.0f, 500.0f);
	// position the camera
	pCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,2.5f,-18.0f), Vect(0.0f,0.0f,7.0f) );

}