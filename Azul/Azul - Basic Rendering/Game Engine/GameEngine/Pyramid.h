#ifndef PYRAMID_H
#define PYRAMID_H

#include "MathEngine.h"

#define  CREATE_PYRAMID_DATA 0 // 1 - creates file, 0- uses file

class Pyramid
{

// Holds the bulk data and texture
public:
	Pyramid();
	void createVAO(const char * const fileName);

	GLuint				vao;
	GLuint				textureID;
	float				cullR;
	Vect				cullC;
};

#endif