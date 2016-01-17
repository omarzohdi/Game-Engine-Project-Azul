#include "OpenGLWrapper.h"
#include "RenderState.h"
#include "GlobalState.h"
#include "Time.h"
#include "Timer.h"

/////////////////////////////////////////////////////////////////////////////////
// An assortment of needed classes
GLShaderManager		shaderManager;

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
	{
   

// Timer test ----------------------------------


	// Create a Timer.
	Timer timer;
 
	// Mark our begin time.
	timer.tic();

    volatile int c = 0;

	for (int i=0; i<1000000000; i++)
	{
		c += 1;
	}

	Time elapsedTime = timer.toc();

	int timeInSeconds_Integer = Time :: quotient( elapsedTime, Time(TIME_ONE_SECOND) );
	int timeInSeconds_ms      = Time :: quotient( elapsedTime, Time( TIME_ONE_MILLISECOND) );

	Time timeInMs_remainder = Time :: remainder( elapsedTime, Time(TIME_ONE_SECOND) );
	int timeInSeconds_ms_remainder = Time :: quotient(  timeInMs_remainder, Time( TIME_ONE_MILLISECOND) );

	printf(" time in seconds: %d \n", timeInSeconds_Integer );
	printf(" time in     ms : %d \n", timeInSeconds_ms );
	printf(" remainder   ms : %d \n", timeInSeconds_ms_remainder);


	Time t1(TIME_MAX);

	volatile int minutes = (Time :: quotient( t1, Time(TIME_ONE_MINUTE ) ));

	printf(" minutes: %d  or  hours %d\n",minutes, minutes/60);


   
   // Timer test ----------------------------------




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

