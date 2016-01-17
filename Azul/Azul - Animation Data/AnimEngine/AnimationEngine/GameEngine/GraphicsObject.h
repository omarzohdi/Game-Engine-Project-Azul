#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "PCSNode.h"
#include "AnimDataTypes.h"

class GraphicsObject : public PCSNode
{
public:
	 
	GraphicsObject()
	{
		World.set(IDENTITY);
		ModelView.set(IDENTITY);
	};


	Matrix getWorld( void ) const;
	void setBoneOrientation( const Matrix & );
	Matrix getBoneOrientation( void ) const;

	// These are pure virtual, needed to be defined by the derived class
	virtual void transform( void ) = 0;   
	virtual void draw(void) = 0;
	virtual void setRenderState(void) = 0;
	virtual void Animtransform( Frame_Bucket *pFrame ) = 0;


protected:
	Matrix	World;
	Matrix	ModelView;
	Matrix  Local;
	Matrix  BoneOrientation;
};


#endif