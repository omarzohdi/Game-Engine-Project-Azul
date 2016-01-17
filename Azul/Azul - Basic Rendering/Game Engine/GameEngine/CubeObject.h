#ifndef CUBE_OBJECT_H
#define CUBE_OBJECT_H

#include "GraphicsObject.h"

class Cube;

class CubeObject : public GraphicsObject
{
public:
	CubeObject( const char * const _Name, Cube *const c);
	
   // Specific to Pyramid
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setStartPos ( const Vect & v);
	void setRotationSpeeds (float x, float y, float z);
	void setTranslationDir(float x, float y, float z);
	void getCurrPos (Vect &outPos);
	void loadObjectData ( void );
	bool checkCulling( void );

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);

	int objectRenderState;

private:
	// internal variables for the instance
	Vect	startPos;
	Vect	currPos;
	Vect	lightColor;
	Vect	lightPos;
	Vect	dir;
	Vect	angles;
	Vect	rotSpeeds;
	char  fileName[24];

	bool isCulled;

	Cube *cubeModel;
};



#endif