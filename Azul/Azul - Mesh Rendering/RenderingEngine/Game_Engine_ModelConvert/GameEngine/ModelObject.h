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

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);

};



#endif