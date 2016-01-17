#ifndef PYRAMID_OBJECT_H
#define PYRAMID_OBJECT_H

#include "GraphicsObject.h"

// forward declaration
class Pyramid;

class PyramidObject : public GraphicsObject
{
public:
	PyramidObject( const char * const _Name, Pyramid * const p );
	
	void setStartPos( const Vect & v);
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setRotationSpeeds (float x, float y, float z);
	void setTranslationDir (float x, float y, float z);
	void loadObjectData ( void );
	void getCurrPos (Vect &outPos);
	bool checkCulling( void );

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);

	int objectRenderState;


private:
	Vect	startPos;
	Vect	currPos;
	Vect	lightColor;
	Vect	lightPos;
	Vect	dir;
	Vect	angles;
	Vect	rotSpeeds;
	char  fileName[24];

	bool isCulled;

	Pyramid *pyramidModel;
};



#endif