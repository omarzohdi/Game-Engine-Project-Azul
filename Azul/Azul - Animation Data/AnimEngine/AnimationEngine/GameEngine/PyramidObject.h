#ifndef PYRAMID_OBJECT_H
#define PYRAMID_OBJECT_H

#include "AnimDataTypes.h"
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

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);

	// should this be at this level??? or higher or change hierarchy
	void setIndex( int val );
	void Animtransform( Frame_Bucket *pFrame );

private:
	Vect	startPos;
	Vect	lightColor;
	Vect	lightPos;
	float	angle_x;
	float	angle_y;

	Pyramid *pyramidModel;

	// Used for the animation array
	// Get's me thinking of different graphics object (Hierarchy, World, Animation)
	int   indexBoneArray;

};



#endif