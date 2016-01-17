#include <stdio.h>
#include "OpenGLWrapper.h"
#include "GlobalState.h"
#include "UserInterface.h"
#include "RenderScene.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "GraphicModel.h"
#include "Object.h"


Camera *pCamera;


void createGraphicsWindow()
{
	printf("createGraphicsWindows()\n");
	// Set the display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Set the window size
	glutInitWindowSize(800, 600);

	// create the window and name it.
	glutCreateWindow("Game Engine");

	// Callback if someone changes size
   glutReshapeFunc(ChangeSize);

	// Callback for key presses
   glutSpecialFunc(SpecialKeys);
   glutKeyboardFunc(KeyPressFunc);
	
   //call back for Rendering (Draw) Scene
   glutDisplayFunc(RenderScene);

		// Extension library - initialize
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		// return error
		}

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
	pCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,0.0f,-1.0f), Vect(0.0f,-50.0f,150.0f) );

	GraphicsObjectManager *goMgr = GraphicsObjectManager::getInstance();

	Object *space_frigate = new Object( "space_frigate","space_frigate.cdm");
	space_frigate->setStartPos( Vect(-50.0f, 3.0f, -50.0f) );
	space_frigate->setCurrentScale( Vect(0.5f, 0.5f, 0.5f) );
	goMgr->addObject( space_frigate );

	Object *tower = new Object( "tower","tower.cdm");
	tower->setStartPos( Vect(-10.0f, 15.0f, -60.0f) );
	tower->setCurrentScale( Vect(2.0f, 2.0f, 2.0f) );
	goMgr->addObject( tower );

	Object *Hulk = new Object( "Hulk","Hulk.cdm");
	Hulk->setStartPos( Vect(10.0f, 0.0f, -25.0f) );
	Hulk->setCurrentScale( Vect(0.25f, 0.25f, 0.25f) );
	Hulk->setCurrentAng(Vect( -89.5f ,0.0f, 0.0f));
	goMgr->addObject( Hulk );

	Object *Radio = new Object( "Radio","Radio.cdm");
	Radio->setStartPos( Vect(-35.0f, 0.0f, 10.0f) );
	Radio->setCurrentScale( Vect(0.50f, 0.50f, 0.50f) );
	Radio->setCurrentAng(Vect( -89.5f ,0.0f, 0.0f));
	goMgr->addObject( Radio );

}





