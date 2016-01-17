#ifndef GRAPHICMODEL_H
#define GRAPHICMODEL_H

#include <stdio.h>
#include <string>
#include <vector>
#include "ModelData.h"
#include "OpenGLWrapper.h"

struct MyVertex_stride
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx; 
	float ny;
	float nz;
};

struct MyTriList
{
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
};

class GraphicModel
{

private:
		std::vector<FBX_Vertex_vsn> model_vert;
		std::vector<VBO_Trilist> model_trilist;

public:
		GLuint vao;
		GLuint textureID;

private:
		void loadDatafromFile(char * filename);
		void loadTexture(char * texName);
		void loadTexturefromBuffer( char * buffer, TEX_Header texHeader);
		void createVAO( void );
		MyVertex_stride * ConvertVertextoVao( void );
		MyTriList		* ConvertVBOTritoTrilist( void );

public:
		GraphicModel(char * filename);
		int getTriangleCount ();
		int getVBOCount();
};


#endif