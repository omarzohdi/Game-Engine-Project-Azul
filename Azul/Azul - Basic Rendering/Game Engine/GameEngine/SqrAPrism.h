#ifndef SQRAPRISM_H
#define SQRAPRISM_H

#include "MathEngine.h"

#define  CREATE_SQRAPRISM_DATA 0


class SqrAPrism
{
public:
	SqrAPrism();
	void createVAO(const char * const fileName);

	GLuint				vao;
	GLuint              textureID;
	float				cullR;
	Vect				cullC;

};

#endif