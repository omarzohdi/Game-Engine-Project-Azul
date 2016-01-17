#include "OpenGLWrapper.h"
#include "Camera.h"
#include "Object.h"

extern Camera			*pCamera;
extern GLShaderManager	shaderManager;

// Constructor make sure everything is initialized
Object:: Object()
{
	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);
}
Object :: Object( const char * const _Name, char * const grModFile )
{
	this->setName( _Name );

	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);
	this->currScale.set(1.0f, 1.0f, 1.0f);
	this->currAng.set(0.0f,0.0f,0.0f);

	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);

	this->graphModel = new GraphicModel(grModFile);

};

void Object::setStartPos( const Vect & v)
{
	this->startPos = v;
};

void Object::setLightColor( const Vect & v)
{
	this->lightColor = v;
};

void Object::setLightPos( const Vect & v)
{
	this->lightPos = v;
};

void Object::setCurrentScale(const Vect & v)
{
	this->currScale = v;
}
void Object::setCurrentAng(const Vect & v)
{
	this->currAng = v;
}

void Object::transform( void )
{
   // update the angles
	//this->angle_y += 0.0008f;
	//this->angle_x -= 0.0001f;
	
	// create temp matrices
	Matrix Scale(SCALE, this->currScale[x], this->currScale[y], this->currScale[z]);
	Matrix RotX( ROT_X, this->currAng[x] );
	Matrix RotY( ROT_Y, this->currAng[y] );
	Matrix RotZ( ROT_Z, this->currAng[z] );
	Matrix Trans( TRANS, this->startPos[x], this->startPos[y],this->startPos[z]);

	// Create the local to world matrix (ie Model)
	this->LocalToWorld = Scale * RotY * RotX * RotZ * Trans;

	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
   this->ModelView = this->LocalToWorld * pCamera->getViewMatrix();

};


void Object::setRenderState( void )
{
	// Bind the texture
   glBindTexture(GL_TEXTURE_2D, this->graphModel->textureID);

   // Use the stock shader
   shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                &ModelView,
                                &pCamera->getProjMatrix(), 
                                &this->lightPos, 
                                &this->lightColor, 
                                0);

};

void Object::draw( void )
{
   glBindVertexArray(this->graphModel->vao);

   /*  render primitives from array data */
   // (GLenum  mode,  GLsizei  count,  GLenum  type,  const GLvoid *  indices);
  
	glDrawElements(GL_TRIANGLES, this->graphModel->getTriangleCount()*3, GL_UNSIGNED_SHORT, 0);   //The starting point of the IBO

};