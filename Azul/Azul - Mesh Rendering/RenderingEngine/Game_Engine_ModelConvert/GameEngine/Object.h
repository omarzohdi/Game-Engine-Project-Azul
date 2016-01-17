#ifndef OBJECT_H
#define OBJECT_H

#include "MathEngine.h"
#include "PCSNode.h"
#include "GraphicModel.h"
#include "GraphicsObject.h"

class Object : public GraphicsObject
{

public:	
	Object();
	Object(const char * const _Name, char * const grMod );
	void transform( void );   
	void draw(void);
	void setRenderState(void);

	void setStartPos( const Vect & v);
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setCurrentScale(const Vect & v);
	void setCurrentAng(const Vect & v);

protected:
	Matrix	LocalToWorld;
	Matrix	ModelView;

private:
	Vect	startPos;
	Vect	lightColor;
	Vect	lightPos;
	Vect	currScale;
	Vect	currPos;
	Vect	currAng;
	
	GraphicModel * graphModel;
};


#endif