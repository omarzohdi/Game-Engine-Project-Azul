#include "OpenGLWrapper.h"

#include "MathEngine.h"
#include "Camera.h"
#include "UserInterface.h"
#include "AnimationObject.h"

#define UNUSED_VAR(_x)  _x;

extern Camera *pCamera;
extern AnimationObject * p1;
extern AnimationData * WalkAnim;
extern AnimationData * IdleAnim;

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int _x, int _y)
{
   // Not using these variables
   UNUSED_VAR(_x);
   UNUSED_VAR(_y);

    bool cameraUpdate = false;
	bool updateLookAt = true;

	Vect position;
	Vect dir;
	pCamera->getDir(dir);
	Vect lookAt;
	pCamera->getPos(position);

	if (key == GLUT_KEY_F1)
	{
		p1->SpeedUP();
	}
	if (key == GLUT_KEY_F2)
	{
		p1->SlowDown();
	}

	if (key == GLUT_KEY_F3)
	{
		p1->ForwardPlay();
	}
	if (key == GLUT_KEY_F4)
	{
		p1->BackwardPlay();
	}

	if (key == GLUT_KEY_F6 )
	{
		p1->Loop();
	}


	if (key == GLUT_KEY_F5)
	{
		p1->QueueNextAnimation();
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

	w = 800;
	h = 600;
   // set the view port
   pCamera->setViewport(0, 0, w, h);
   // set the perspective
   pCamera->setPerspective( 35.0f, float(w)/float(h), 1.0f, 500.0f);

	// position the camera   (up,lookat, pos)
	pCamera->setOrientAndPosition( Vect(0.0f, 0.0f,1.0f), Vect(0.0f,10.0f,10.0f), Vect(180.0f,0.0f,90.0f) );


}