#include "OpenGLWrapper.h"
#include "RenderScene.h"
#include "MathEngine.h"
#include "Camera.h"
#include "GraphicsManager.h"
#include "GraphicsObject.h"


extern Camera *pCamera;


void RenderScene(void)
{
   // Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      
	// Our new camera, make sure everything is consistent
   pCamera->updateCamera();

   // get the graphic object manager (singleton)
	GraphicsObjectManager *gom = GraphicsObjectManager::getInstance();

	// Draw them
	gom->drawObjects();


   // Flush drawing commands
   glutPostRedisplay();
   glutSwapBuffers();

}


