#include "OpenGLWrapper.h"

#include "MathEngine.h"
#include "Camera.h"
#include "UserInterface.h"

#define UNUSED_VAR(_x)  _x;

extern Camera *pCamera;

#define ANGLE 0.01f

void KeyPressFunc (unsigned char key, int _x, int _y)
{

	bool cameraUpdate = false;
	bool updateLookAt = true;

	Vect position;
	Vect dir = pCamera->getDir();
	Vect lookAt;
	pCamera->getPos(position);

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

    bool cameraUpdate = false;
	bool updateLookAt = true;

	Vect position;
	Vect dir= pCamera->getDir();
	Vect lookAt;
	pCamera->getPos(position);
	
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
	
	if (key == GLUT_KEY_DOWN)
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[y] -= 0.35f;
		pCamera->setPos( vTmp );
	}
	
	if (key == GLUT_KEY_UP)
	{
		Vect vTmp;
		pCamera->getPos( vTmp );
		vTmp[y] += 0.35f;
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
	pCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,0.0f,-1.0f), Vect(0.0f,50.0f,150.0f) );

}