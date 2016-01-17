#include "OpenGLWrapper.h"
#include "Pyramid.h"
#include "PyramidObject.h"
#include "Camera.h"

extern Camera			*pCamera;
extern GLShaderManager	shaderManager;

// Constructor make sure everything is initialized
PyramidObject :: PyramidObject( const char * const _Name, Pyramid * const p )
{
   this->pyramidModel = p;
   this->setName( _Name );

	this->angle_x = (float)(rand()%100)/100.0f * MATH_PI;
	this->angle_y = (float)(rand()%100)/100.0f * MATH_PI;
  
   this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
   this->lightPos.set(1.0f, 1.0f, 0.0f);

   this->LocalToWorld.set(IDENTITY);
   this->ModelView.set(IDENTITY);

};

void PyramidObject::setStartPos( const Vect & v)
{
	this->startPos = v;
};

void PyramidObject::setLightColor( const Vect & v)
{
	this->lightColor = v;
};

void PyramidObject::setLightPos( const Vect & v)
{
	this->lightPos = v;
};


void PyramidObject::transform( void )
{
   // update the angles
	this->angle_y += 0.08f;
   	this->angle_x -= 0.01f;
	
	// create temp matrices
	Matrix Scale(SCALE, 0.5f, 0.5f, 0.5f);
	Matrix RotX( ROT_X, this->angle_x );
	Matrix RotY( ROT_Y, this->angle_y );
	Matrix Trans( TRANS, this->startPos[x], this->startPos[y],this->startPos[z]);

	// Create the local to world matrix (ie Model)
	this->LocalToWorld = Scale * RotY * RotX * Trans;

	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
   this->ModelView = this->LocalToWorld * pCamera->getViewMatrix();

};


void PyramidObject::setRenderState( void )
{
	// Bind the texture
   glBindTexture(GL_TEXTURE_2D, this->pyramidModel->textureID);

   // Use the stock shader
   shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                &ModelView,
                                &pCamera->getProjMatrix(), 
                                &this->lightPos, 
                                &this->lightColor, 
                                0);

};

void PyramidObject::draw( void )
{
       
   glBindVertexArray(this->pyramidModel->vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3*18);

   /*  render primitives from array data */
   // (GLenum  mode,  GLsizei  count,  GLenum  type,  const GLvoid *  indices);
   glDrawElements(GL_TRIANGLES, 6*3, GL_UNSIGNED_SHORT, 0);   //The starting point of the IBO
  

};