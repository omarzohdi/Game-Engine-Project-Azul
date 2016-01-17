#include "OpenGLWrapper.h"
#include "RenderState.h"
#include "GlobalState.h"


/////////////////////////////////////////////////////////////////////////////////
// An assortment of needed classes
GLShaderManager		shaderManager;

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
	{
   
	// set directory
	gltSetWorkingDirectory(argv[0]);
	
	// initialize GLUT
	glutInit(&argc, argv);
	
	// create graphics windows with defaults
	createGraphicsWindow();

	// Set up the Render Context
	// Internal context (state) for the graphics chips
	SetupRC();

	// Main Loop, stays running 
	// Processes, messages, keystrokes until window is closed
	glutMainLoop();
    
	// Closes the rendering context before quitting, removes textures
    ShutdownRC();
    
	return 0;
	}

