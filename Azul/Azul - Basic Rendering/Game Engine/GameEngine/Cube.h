#ifndef CUBE_H
#define CUBE_H


#define  CREATE_CUBE_DATA 0

#include "MathEngine.h"

class Cube
{
public:
	Cube();
	void createVAO(const char * const fileName);
   

	GLuint				vao;
	GLuint              textureID;
	float				cullR;
	Vect				cullC;

};

#endif