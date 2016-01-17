#include "GraphicsObject.h"

    
Matrix GraphicsObject::getWorld(  ) const
{	
	return Matrix(this->World);
}

Matrix GraphicsObject::getBoneOrientation( void ) const
{
	return Matrix(this->BoneOrientation);
}

void GraphicsObject::setBoneOrientation( const Matrix &tmp )
{

	this->BoneOrientation = tmp;
}