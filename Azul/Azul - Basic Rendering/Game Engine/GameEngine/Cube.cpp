#include "OpenGLWrapper.h"
#include "Cube.h"
#include "MathEngine.h"
#include "camera.h"
#include "GraphicsObjectFileHdr.h"
#include "File.h"
#include "TextureManager.h"
#include <assert.h>

extern GLShaderManager			shaderManager;
extern Camera					*pCamera;


Cube::Cube()
{
	this->cullC.set(Vect(0,0,0));
	this->cullR = 1.7f;
}

void Cube::createVAO(const char * const fileName )
{

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

#if CREATE_CUBE_DATA

	MyTriList	tlist[12];

	//Top
	tlist[0].v1 = 2;
	tlist[0].v2 = 1;
	tlist[0].v3 = 0;
	
	tlist[1].v1 = 2;
	tlist[1].v2 = 3;
	tlist[1].v3 = 1;

	tlist[2].v1 = 5;
	tlist[2].v2 = 1;
	tlist[2].v3 = 3;

	tlist[3].v1 = 4;
	tlist[3].v2 = 5;
	tlist[3].v3 = 3;

	tlist[4].v1 = 2;
	tlist[4].v2 = 0;
	tlist[4].v3 = 6;

	tlist[5].v1 = 6;
	tlist[5].v2 = 7;
	tlist[5].v3 = 2;

	tlist[6].v1 = 6;
	tlist[6].v2 = 4;
	tlist[6].v3 = 7;
	
	tlist[7].v1 = 6;
	tlist[7].v2 = 5;
	tlist[7].v3 = 4;

	tlist[8].v1 = 2;
	tlist[8].v2 = 7;
	tlist[8].v3 = 4;

	tlist[9].v1 = 3;
	tlist[9].v2 = 2;
	tlist[9].v3 = 4;

	tlist[10].v1 = 1;
	tlist[10].v2 = 5;
	tlist[10].v3 = 6;

	tlist[11].v1 = 0;
	tlist[11].v2 = 1;
	tlist[11].v3 = 6;

	MyVertex_stride pVerts[8];

	pVerts[0].x = 1.0f;
	pVerts[0].y = 1.0f;
	pVerts[0].z = 1.0f;
	pVerts[0].s = 0.0f;
	pVerts[0].t = 0.0f;

	pVerts[0].nx = 3.0f;
	pVerts[0].ny = 5.0f;
	pVerts[0].nz = 3.0f;

	// left front
	pVerts[1].x = -1.0f;
	pVerts[1].y = 1.0f;
	pVerts[1].z = 1.0f;
	pVerts[1].s = 1.0f;
	pVerts[1].t = 0.0f;

	pVerts[1].nx = -3.0f;
	pVerts[1].ny = 5.0f;
	pVerts[1].nz = 3.0f;

	// right front
	pVerts[2].x = 1.0f;
	pVerts[2].y = 1.0f;
	pVerts[2].z = -1.0f;
	pVerts[2].s = 1.0f;
	pVerts[2].t = 1.0f;

	pVerts[2].nx = 3.0f;
	pVerts[2].ny = 5.0f;
	pVerts[2].nz = -3.0f;

	// left back
	pVerts[3].x = -1.0f;
	pVerts[3].y = 1.0f;
	pVerts[3].z = -1.0f;
	pVerts[3].s = 0.0f;
	pVerts[3].t = 1.0f;

	pVerts[3].nx = -3.0f;
	pVerts[3].ny = 5.0f;
	pVerts[3].nz = -3.0f; 

	// right back
	pVerts[4].x = -1.0f;
	pVerts[4].y = -1.0f;
	pVerts[4].z = -1.0f;
	pVerts[4].s = 0.0f;
	pVerts[4].t = 0.0f;

	pVerts[4].nx = -3.0f;
	pVerts[4].ny = -5.0f;
	pVerts[4].nz = -3.0f; 

	pVerts[5].x = -1.0f;
	pVerts[5].y = -1.0f;
	pVerts[5].z = 1.0f;
	pVerts[5].s = 1.0f;
	pVerts[5].t = 1.0f;

	pVerts[5].nx = -3.0f;
	pVerts[5].ny = -5.0f;
	pVerts[5].nz = 3.0f;

	pVerts[6].x = 1.0f;
	pVerts[6].y = -1.0f;
	pVerts[6].z = 1.0f;
	pVerts[6].s = 0.0f;
	pVerts[6].t = 1.0f;

	pVerts[6].nx = 3.0f;
	pVerts[6].ny = -5.0f;
	pVerts[6].nz = 3.0f; 

	pVerts[7].x = 1.0f;
	pVerts[7].y = -1.0f;
	pVerts[7].z = -1.0f;
	pVerts[7].s = 1.0f;
	pVerts[7].t = 0.0f;

	pVerts[7].nx = 3.0f;
	pVerts[7].ny = -5.0f;
	pVerts[7].nz = -3.0f;  

	FileHandle fh;
	FileError  ferror;

	ferror = File::open(fh, fileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );
	gObjFileHdr  graphicsFileHdr;

	ferror = File::read( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
	assert( ferror == FILE_SUCCESS );

	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

	ferror = File::open(fh, fileName, FILE_READ_WRITE );
	assert( ferror == FILE_SUCCESS );

	ferror = File::write( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
	assert( ferror == FILE_SUCCESS );

	graphicsFileHdr.numVerts = 8;
   
	ferror = File::tell( fh, graphicsFileHdr.vertBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::write( fh, pVerts, 8 * sizeof(MyVertex_stride) );
	assert( ferror == FILE_SUCCESS );

	graphicsFileHdr.numTriList = 12;

	ferror = File::tell( fh, graphicsFileHdr.triListBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::write( fh, tlist, 12 * sizeof(MyTriList) );
	assert( ferror == FILE_SUCCESS );

	ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
	assert( ferror == FILE_SUCCESS );

	ferror = File::write( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
	assert( ferror == FILE_SUCCESS );

	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

#else

	FileHandle fh;
	FileError  ferror;

	ferror = File::open(fh, fileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );

	gObjFileHdr  gFileHdr;

	ferror = File::read( fh, &gFileHdr, sizeof(gObjFileHdr) );
	assert( ferror == FILE_SUCCESS );

	// create the vertex buffer
	MyVertex_stride *pVerts = (MyVertex_stride *)malloc( gFileHdr.numVerts * sizeof(MyVertex_stride) );

	ferror = File::seek( fh, FILE_SEEK_BEGIN, gFileHdr.vertBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, pVerts, gFileHdr.numVerts  * sizeof(MyVertex_stride) );
	assert( ferror == FILE_SUCCESS );

	MyTriList	*tlist = (MyTriList	*)malloc( gFileHdr.numTriList * sizeof(MyTriList) );

	ferror = File::seek( fh, FILE_SEEK_BEGIN, gFileHdr.triListBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, tlist, gFileHdr.numTriList * sizeof(MyTriList)  );
	assert( ferror == FILE_SUCCESS );

	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

#endif

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
	GLuint	vbo[2]; 
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * 8, pVerts, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	void *offsetVert = (void *)((unsigned int)&pVerts[0].x - (unsigned int)pVerts);
	glVertexAttribPointer(0, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetVert);

	glEnableVertexAttribArray(3);  
    void *offsetTex = (void *)((unsigned int)&pVerts[0].s - (unsigned int)pVerts);
	glVertexAttribPointer(3, 2, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetTex);

	glEnableVertexAttribArray(2);  
	void *offsetNorm = (void *)((unsigned int)&pVerts[0].nx - (unsigned int)pVerts);
	glVertexAttribPointer(2, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetNorm);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList)*12, tlist, GL_STATIC_DRAW);
}
