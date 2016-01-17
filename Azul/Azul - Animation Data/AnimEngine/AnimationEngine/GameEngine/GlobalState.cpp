#include <stdio.h>
#include "OpenGLWrapper.h"
#include "GlobalState.h"
#include "UserInterface.h"
#include "RenderScene.h"
#include "Camera.h"
#include "Timer.h"
//#include "Anim.h"
#include "PyramidObject.h"
#include "AnimationManager.h"
#include "AnimationObject.h"
#include "AnimationData.h"

AnimationManager * AnimMan;
Camera *pCamera;
AnimationObject * p1;
AnimationData * WalkAnim;
AnimationData * IdleAnim;
extern PyramidObject *firstBone;


void createGraphicsWindow()
{
	printf("createGraphicsWindows()\n");
	// Set the display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Set the window size
	glutInitWindowSize(800, 600);

	// create the window and name it.
	glutCreateWindow("Pyramid");

	// Callback if someone changes size
	glutReshapeFunc(ChangeSize);

	// Callback for key presses
	glutSpecialFunc(SpecialKeys);

	//call back for Rendering (Draw) Scene
	glutDisplayFunc(RenderScene);

	// Extension library - initialize
	GLenum err = glewInit();
	if (GLEW_OK != err) 
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		// return error
	}
	
	// add a callback timer
	glutTimerFunc(16, TimerFunction, 1);	

	// Startup
	frameworkStartup();
}


// User functions, called at start up (once)
void frameworkStartup( void )
{
	// todo: move this to a one time call only
	pCamera = new Camera();

	// Initially setup the camera
	pCamera->setViewport( 0, 0,800, 600);
	pCamera->setPerspective( 35.0f, float(800)/float(600), 1.0f, 500.0f);

	WalkAnim = new AnimationData("anim.anim");
	//IdleAnim = new AnimationData("Idle.anim");

	p1 = new AnimationObject(WalkAnim);
	p1->animHeadoffset = 0;
	//p1->QueueNextAnimation();

	AnimMan = AnimMan->getInstance();
	AnimMan->addObject(p1);

}


void GameLoop( void )
{
	AnimMan->Update();
};


void TimerFunction(int )
{
      // Create a Timer.
     static Timer timer;

     Time elapsedTime = timer.toc();

//     int timeInMs = Time :: quotient( elapsedTime, Time(TIME_ONE_MILLISECOND) );
     
    // printf("Frames: ms:%d\n",timeInMs);
      // Mark our begin time.
     timer.tic();

	// trigger the game loop, every 16ms
	GameLoop();

	// reset the timer
	glutTimerFunc(16,TimerFunction, 1);

};

