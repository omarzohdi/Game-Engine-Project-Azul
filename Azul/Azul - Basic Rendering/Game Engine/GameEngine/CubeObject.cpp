#include "OpenGLWrapper.h"
#include "Cube.h"
#include "CubeObject.h"
#include "Camera.h"
#include "GraphicsObjectFileHdr.h"
#include "TextureManager.h"
#include "File.h"
#include <assert.h>

extern Camera			*pCamera;
extern GLShaderManager	shaderManager;
extern int				CubeTexture;
extern bool				SphereRender;

GLuint			CtextureID;

// Constructor make sure everything is initialized
CubeObject :: CubeObject(const char * const _Name, Cube * const c)
{

	strcpy(this->fileName,_Name);
	strcat(this->fileName,".cdm");

	this->cubeModel = c;

	this->setName( _Name );

	this->angles.set(Vect((float)(rand()%100)/100.0f * MATH_PI,
						(float)(rand()%100)/100.0f * MATH_PI,
						(float)(rand()%100)/100.0f * MATH_PI));
	
	this->dir.set(Vect(0,0,0));

	this->rotSpeeds.set(Vect(0,0,0));

	this->objectRenderState = 0;
  
	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);

	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);

};

void CubeObject::setLightColor( const Vect & v)
{
	this->lightColor = v;
};

void CubeObject::setLightPos( const Vect & v)
{
	this->lightPos = v;
};

void CubeObject::setStartPos ( const Vect & v)
{
	this-> startPos = v;
	this->currPos = this->startPos;
}

void CubeObject::getCurrPos (Vect &outPos)
{
	outPos = this->currPos;
}

void CubeObject::setRotationSpeeds (float x, float y, float z)
{
	this->rotSpeeds.set(Vect(x,y,z));
}

void CubeObject::setTranslationDir (float x, float y, float z)
{
	this->dir.set(Vect(x,y,z));
}

void CubeObject::transform( void )
{
   // update the angles
	this->angles += this->rotSpeeds;
	
	// create temp matrices
	Matrix Scale(SCALE, 0.5f, 0.5f, 0.5f);
	Matrix RotX( ROT_X, this->angles[x] );
	Matrix RotY( ROT_Y, this->angles[y] );
	Matrix RotZ( ROT_Z, this->angles[z] );

	///////////////////////DEMO CODE, UNECESSARY REMOVE ///////////////////////////
	if (this->currPos[y] >= 2)
		this->dir[y] *= -1;
	if (this->currPos[y] <= -2)
		this->dir[y] *= -1;

	/////////////////////////////////////////////////////////////////////////////

	this->currPos[x] += this->dir[x];
	this->currPos[y] += this->dir[y];
	this->currPos[z] += this->dir[z];

	Matrix Trans( TRANS, this->currPos[x], this->currPos[y], this->currPos[z]);

	// Create the local to world matrix (ie Model)
	
	this->LocalToWorld = Scale * RotY * RotX * RotZ * Trans;

	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
   this->ModelView = this->LocalToWorld * pCamera->getViewMatrix();
};

void CubeObject::setRenderState( void )
{
	gObjFileHdr  graphicsFileHdr;

	if (this->objectRenderState == 3)
	{
		if (0 == CtextureID)
		{
			const char * texture;

			if (CubeTexture == 1)
				texture = "stone.tga";
			if (CubeTexture == 2)
				texture = "stone2.tga";
			if (CubeTexture == 3)
				texture = "stone3.tga";

			strncpy_s( graphicsFileHdr.objName, OBJECT_NAME_SIZE, "cube", _TRUNCATE);

			graphicsFileHdr.textData.target = GL_TEXTURE_2D;
			graphicsFileHdr.textData.minFilter = GL_LINEAR;
			graphicsFileHdr.textData.magFilter = GL_LINEAR;
			graphicsFileHdr.textData.wrapMode = GL_CLAMP_TO_EDGE;

			strncpy_s( graphicsFileHdr.textData.textName, TEXTURE_NAME_SIZE, texture, _TRUNCATE);

			CtextureID = TextureManager::addTexture( graphicsFileHdr.textData );
		}

		glBindTexture(GL_TEXTURE_2D, CtextureID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, this->cubeModel->textureID); 
	}

	Matrix t =ModelView*pCamera->getProjMatrix();

	switch(this->objectRenderState)
	{
		case 0:
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, &ModelView, &pCamera->getProjMatrix(), &this->lightPos, &this->lightColor, 0);
			break;

		case 1:
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, &t, 0);
			break;

		case 2:	
			shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, &ModelView, &pCamera->getProjMatrix(), &this->lightPos, &this->lightColor);
			break;
		
		case 3:
	  			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, &t, 0);
			break;
	}

};

bool CubeObject::checkCulling( void )
{
	Vect center = this->cubeModel->cullC;
	center *= this->LocalToWorld;
	float radius = this->cubeModel->cullR;
	
	///////////////////////////////////////////////////////////////////GET NON UNIFORM SCALE OF RADIUS///////////////////////////////////////////////////////////////////
	float scale_x = (this->LocalToWorld.m0 * this->LocalToWorld.m0) + (this->LocalToWorld.m1 * this->LocalToWorld.m1) + (this->LocalToWorld.m2 * this->LocalToWorld.m2);
	scale_x = sqrt(scale_x);
	float scale_y = (this->LocalToWorld.m4 * this->LocalToWorld.m4) + (this->LocalToWorld.m5 * this->LocalToWorld.m5) + (this->LocalToWorld.m6 * this->LocalToWorld.m6);
	scale_y = sqrt(scale_y);
	float scale_z = (this->LocalToWorld.m8 * this->LocalToWorld.m8) + (this->LocalToWorld.m9 * this->LocalToWorld.m9) + (this->LocalToWorld.m10 * this->LocalToWorld.m10);
	scale_z = sqrt(scale_z);

	if (scale_x > scale_y && scale_x > scale_z)
		radius *= scale_x;
	else if (scale_y > scale_x && scale_y > scale_z)
		radius *= scale_y;
	else 
		radius *= scale_z;


	////////////////////////////////////////////////////////////////CACLCULATE POINTS ON SPHERE////////////////////////////////////////////////////////////////////////////
	Vect pX = center + (Vect(1.0, 0.0, 0.0) * this->cubeModel->cullR);
	Vect pY = center + (Vect(0.0, 1.0, 0.0) * this->cubeModel->cullR);
	Vect pZ = center + (Vect(0.0, 0.0, 1.0) * this->cubeModel->cullR);

	pX *= this->LocalToWorld;
	pY *= this->LocalToWorld;
	pZ *= this->LocalToWorld;

	Vect FTR;
	Vect NBL;
	
	pCamera->getNearBottomLeft(NBL);
	pCamera->getFarTopRight(FTR);


	///////////////////////////////////////////////////////////////CHECK IF IN FRUSTRUM/////////////////////////////////////////////////////////////////////////////////////
	Vect Normal;
	float Result;
	
	Vect Distance = (center - NBL);

	pCamera->getBottomNorm(Normal);
	Result = Distance.dot(Normal);
	
	if (Result > radius)
		return true;

	pCamera->getLeftNorm(Normal);
	Result = Distance.dot(Normal);
	
	if (Result > radius)
		return true;
	
	pCamera->getFrontNorm(Normal);
	Result = Distance.dot(Normal);
	
	if (Result > radius)
		return true;

	
	Distance = (center - FTR);
	
	pCamera->getBackNorm(Normal);
	Result = Distance.dot(Normal);
	
	if (Result > radius)
		return true;

	pCamera->getRightNorm(Normal);
	Result = Distance.dot(Normal);
	
	if (Result > radius)
		return true;

	pCamera->getTopNorm(Normal);
	Result = Distance.dot(Normal);
	
	if (Result > radius)
		return true;

	return false;
}

void CubeObject::draw( void )
{	
	glBindVertexArray(this->cubeModel->vao);
	
	this->isCulled = checkCulling();
	char buffer[16];
	this->getName(buffer,16);

	if (!isCulled)
		glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_SHORT, 0);
	else
	{

		printf("\n");
		printf("Object Culled: %s",buffer);
		
	}
	
	if (SphereRender)
		glutWireSphere(this->cubeModel->cullR, 20,20);
};

void CubeObject::loadObjectData ( void )
{
	#if CREATE_CUBE_DATA

   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, this->fileName, FILE_WRITE );
   assert( ferror == FILE_SUCCESS );

   // write the data
   gObjFileHdr  graphicsFileHdr;

	graphicsFileHdr.cullC = this->cubeModel->cullC;
	graphicsFileHdr.cullR = this->cubeModel->cullR;

	graphicsFileHdr.strPos = this->startPos;
	graphicsFileHdr.lightPos = this->lightPos;
	graphicsFileHdr.lightCol = this->lightColor;
	graphicsFileHdr.dir = this->dir;
	graphicsFileHdr.rotSpeeds = this->rotSpeeds;
	graphicsFileHdr.render = this->objectRenderState;

   // object name
   strncpy_s( graphicsFileHdr.objName, OBJECT_NAME_SIZE, "cube", _TRUNCATE);

   // squirrel away the parameters
   graphicsFileHdr.textData.target = GL_TEXTURE_2D;
   graphicsFileHdr.textData.minFilter = GL_LINEAR;
   graphicsFileHdr.textData.magFilter = GL_LINEAR;
   graphicsFileHdr.textData.wrapMode = GL_CLAMP_TO_EDGE;
   strncpy_s( graphicsFileHdr.textData.textName, TEXTURE_NAME_SIZE, "stone2.tga", _TRUNCATE);

    // write the data
   ferror = File::write( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
   assert( ferror == FILE_SUCCESS );

   // close for now
   ferror = File::close( fh );
   assert( ferror == FILE_SUCCESS );

#else

   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, this->fileName, FILE_READ );
   assert( ferror == FILE_SUCCESS );

   // write the data
   gObjFileHdr  graphicsFileHdr;

   // Read the header
   ferror = File::read( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
   assert( ferror == FILE_SUCCESS );

	this->cubeModel->cullC = graphicsFileHdr.cullC;
	this->cubeModel->cullR = graphicsFileHdr.cullR;
	this->setStartPos(graphicsFileHdr.strPos);
	this->lightPos = graphicsFileHdr.lightPos;
	this->lightColor = graphicsFileHdr.lightCol;
	this->dir = graphicsFileHdr.dir;
	this->rotSpeeds = graphicsFileHdr.rotSpeeds;
	this->objectRenderState = graphicsFileHdr.render;

   // close
   ferror = File::close( fh );
   assert( ferror == FILE_SUCCESS );

#endif

   // install texture
   this->cubeModel->textureID = TextureManager::addTexture( graphicsFileHdr.textData );

   this->cubeModel->createVAO(this->fileName);
}