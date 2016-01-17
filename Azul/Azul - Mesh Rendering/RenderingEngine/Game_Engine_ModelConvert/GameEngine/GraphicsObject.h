#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "PCSNode.h"
#include "GraphicModel.h"

class GraphicsObject : public PCSNode
{

public:	

	virtual void transform( void ) = 0;   
	virtual void draw(void) = 0;
	virtual void setRenderState(void) = 0;


};


#endif