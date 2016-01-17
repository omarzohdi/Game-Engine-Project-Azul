#include "OpenGLWrapper.h"
#include "Pyramid.h"
#include "PyramidObject.h"
#include "Camera.h"
#include "Time.h"

Frame_Bucket *pHead;
extern Camera			*pCamera;
extern GLShaderManager	shaderManager;

// Constructor make sure everything is initialized
PyramidObject :: PyramidObject( const char * const _Name, Pyramid * const p )
{
   this->pyramidModel = p;
   this->setName( _Name );

	this->angle_x = (float)(rand()%100)/100.0f * MATH_PI;
	this->angle_y = (float)(rand()%100)/100.0f * MATH_PI;
  
	this->angle_x = 0.0f;
	this->angle_y = 0.0f;
  

   this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
   this->lightPos.set(1.0f, 1.0f, 0.0f);

   this->World.set(IDENTITY);
   this->ModelView.set(IDENTITY);
   this->BoneOrientation.set(IDENTITY);
   this->Local.set(IDENTITY);

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

void PyramidObject::setIndex( int val )
{
	this->indexBoneArray = val;
}

void PyramidObject::Animtransform( Frame_Bucket *pFrame )
{
	pHead = pFrame;
	this->transform();
}

void PyramidObject::transform( void )
{
	// parent's world matrix, needed for hierarchy concatenation
	Matrix ParentWorld;

   // Does this node have a parent?
	if( this->getParent() == 0 )
	{
      // no - set to identity
		ParentWorld.set(IDENTITY);
	}
	else
	{
      // yes - get it
      PCSNode *p = this->getParent();

      // The following statements can be reduced due to polymorphism
      // 1) GraphicsObject *go = (GraphicsObject *) p;
      // 2) PyramidObject *parentObj = (PyramidObject *) go;
      PyramidObject *parentObj = (PyramidObject *) p;

      // Now get the world matrix
      ParentWorld = parentObj->World;
	}

	// REMEMBER this is for Animation and hierachy, you need to handle models differently
	// Get the result bone array
	Bone *bResult = pHead->pBone;
	
   // Create the local matrix
   // Remember to get the value from the animation system
	Matrix T = Matrix( TRANS, bResult[indexBoneArray].T);
	Matrix S = Matrix( SCALE, bResult[indexBoneArray].S);
	Quat   Q = bResult[indexBoneArray].Q;

	// Isn't awesome that we can multiply Quat with matrices!
	Matrix M = S * Q * T;

	// Find the local
	this->Local =   M ;

	// evaluate the world
	this->World =   this->Local * ParentWorld; 
	
	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
	//this->ModelView = this->BoneOrientation * this->World * pCamera->getViewMatrix();
	this->ModelView = this->BoneOrientation *  pCamera->getViewMatrix();
	

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

   /*  render primitives from array data */
   // (GLenum  mode,  GLsizei  count,  GLenum  type,  const GLvoid *  indices);
   glDrawElements(GL_TRIANGLES, 6*3, GL_UNSIGNED_SHORT, 0);   //The starting point of the IBO
  

};