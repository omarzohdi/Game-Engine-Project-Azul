#include "OpenGLWrapper.h"
#include "RenderState.h"
#include "Pyramid.h"
#include "TextureManager.h"

extern GLShaderManager		shaderManager;

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    // Black background
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f );

	 shaderManager.InitializeStockShaders();

	 glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    
    glCullFace(GL_BACK);
  
}

void ShutdownRC(void)
{
   // DELETE Textures
   TextureManager::deleteTextures();

}