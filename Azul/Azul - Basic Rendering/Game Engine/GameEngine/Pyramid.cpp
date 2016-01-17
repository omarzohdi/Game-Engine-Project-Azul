#include <assert.h>
#include "OpenGLWrapper.h"
#include "MathEngine.h"
#include "Pyramid.h"
#include "Camera.h"
#include "File.h"
#include "GraphicsObjectFileHdr.h"
#include "TextureManager.h"

 
Pyramid::Pyramid()
{
	this->cullC.set(Vect(0,0,0));
	this->cullR = 1.75f;
}
void Pyramid::createVAO(const char * const fileName )
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



#if CREATE_PYRAMID_DATA

MyTriList	tlist[6];
MyVertex_stride pVerts[5];

// front
tlist[0].v1 = 0;
tlist[0].v2 = 2;
tlist[0].v3 = 1;

// left
tlist[1].v1 = 0;
tlist[1].v2 = 1;
tlist[1].v3 = 3;

// right
tlist[2].v1 = 0;
tlist[2].v2 = 3;
tlist[2].v3 = 4;

// back
tlist[3].v1 = 0;
tlist[3].v2 = 4;
tlist[3].v3 = 2;

// bottom 1
tlist[4].v1 = 2;
tlist[4].v2 = 3;
tlist[4].v3 = 1;

// bottom 2
tlist[5].v1 = 2;
tlist[5].v2 = 4;
tlist[5].v3 = 3;



// apex
pVerts[0].x = 0.0f;
pVerts[0].y = 1.0f;
pVerts[0].z = 0.0f;
pVerts[0].s = 0.5f;
pVerts[0].t = 0.5f;
pVerts[0].nx = 0.0f;
pVerts[0].ny = 1.0f;
pVerts[0].nz = 0.0f;


// left front
pVerts[1].x = -1.0f;
pVerts[1].y = -1.0f;
pVerts[1].z = -1.0f;
pVerts[1].s = 0.0f;
pVerts[1].t = 0.0f;

pVerts[1].nx = -3.0f;
pVerts[1].ny = -5.0f;
pVerts[1].nz = -3.0f;

// right front
pVerts[2].x = 1.0f;
pVerts[2].y = -1.0f;
pVerts[2].z = -1.0f;
pVerts[2].s = 1.0f;
pVerts[2].t = 0.0f;

pVerts[2].nx = 3.0f;
pVerts[2].ny = -5.0f;
pVerts[2].nz = -3.0f;


// left back
pVerts[3].x = -1.0f;
pVerts[3].y = -1.0f;
pVerts[3].z = 1.0f;
pVerts[3].s = 0.0f;
pVerts[3].t = 1.0f;

pVerts[3].nx = -3.0f;
pVerts[3].ny = -5.0f;
pVerts[3].nz = 3.0f;


// right back
pVerts[4].x = 1.0f;
pVerts[4].y = -1.0f;
pVerts[4].z = 1.0f;
pVerts[4].s = 1.0f;
pVerts[4].t = 1.0f;

pVerts[4].nx = 3.0f;
pVerts[4].ny = -5.0f;
pVerts[4].nz = 3.0f;


// Write the data to a file ----------------------------

   FileHandle fh;
   FileError  ferror;

   

   // 1)  Get the existing GraphicsFileHeader from the file

      // Since we didn't wrap the ReOpen functionality, 
      // I need to read, close then write again
      ferror = File::open(fh, fileName, FILE_READ );
      assert( ferror == FILE_SUCCESS );

       // write the data
      gObjFileHdr  graphicsFileHdr;

      // Read the header
      ferror = File::read( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
      assert( ferror == FILE_SUCCESS );

      // close
      ferror = File::close( fh );
      assert( ferror == FILE_SUCCESS );


   // 2)  Write the data (Header, Buffers) to the file

   // write the header (Take 1) 

      // create the file, write the header
      ferror = File::open(fh, fileName, FILE_READ_WRITE );
      assert( ferror == FILE_SUCCESS );

      // write the data
      ferror = File::write( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
      assert( ferror == FILE_SUCCESS );

   // update the header: numVerts, VertBufferOffset

      // update the number of verts
      graphicsFileHdr.numVerts = 5;
   
      // update the offset
      ferror = File::tell( fh, graphicsFileHdr.vertBufferOffset );
      assert( ferror == FILE_SUCCESS );

   // write the vertex data
   ferror = File::write( fh, pVerts, 5 * sizeof(MyVertex_stride) );
   assert( ferror == FILE_SUCCESS );

   // update the header: numTriList, triListBufferOffset

      // update the number of verts
      graphicsFileHdr.numTriList = 6;

      // update the offset
      ferror = File::tell( fh, graphicsFileHdr.triListBufferOffset );
      assert( ferror == FILE_SUCCESS );

   // write the triListBuffer data
   ferror = File::write( fh, tlist, 6 * sizeof(MyTriList) );
   assert( ferror == FILE_SUCCESS );

   // write the header (Take 2) now with updated data

      // reset to the beginning of file
      ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
      assert( ferror == FILE_SUCCESS );

      // write the buffer
      ferror = File::write( fh, &graphicsFileHdr, sizeof(gObjFileHdr) );
      assert( ferror == FILE_SUCCESS );

   // All done - bye bye
   ferror = File::close( fh );
   assert( ferror == FILE_SUCCESS );

#else


   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, fileName, FILE_READ );
   assert( ferror == FILE_SUCCESS );


    // write the data
   gObjFileHdr  gFileHdr;

   // Read the header
   ferror = File::read( fh, &gFileHdr, sizeof(gObjFileHdr) );
   assert( ferror == FILE_SUCCESS );

   // create the vertex buffer
   MyVertex_stride *pVerts = (MyVertex_stride *)malloc( gFileHdr.numVerts * sizeof(MyVertex_stride) );

   // load the verts

      // seek to the location
      ferror = File::seek( fh, FILE_SEEK_BEGIN, gFileHdr.vertBufferOffset );
      assert( ferror == FILE_SUCCESS );

      // read it
      ferror = File::read( fh, pVerts, gFileHdr.numVerts  * sizeof(MyVertex_stride) );
      assert( ferror == FILE_SUCCESS );


   // create the triLists buffer
   MyTriList	*tlist = (MyTriList	*)malloc( gFileHdr.numTriList * sizeof(MyTriList) );

   // load the triList

      // seek to the location
      ferror = File::seek( fh, FILE_SEEK_BEGIN, gFileHdr.triListBufferOffset );
      assert( ferror == FILE_SUCCESS );

      // read it
      ferror = File::read( fh, tlist, gFileHdr.numTriList * sizeof(MyTriList)  );
      assert( ferror == FILE_SUCCESS );


   // close
   ferror = File::close( fh );
   assert( ferror == FILE_SUCCESS );

#endif





     /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &this->vao);
 
    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(this->vao);

    //GLuint vbo;
    GLuint vbo[2];

    /* Allocate and assign two Vertex Buffer Objects to our handle */
    glGenBuffers(2, vbo);
 
    // Load the combined data: ---------------------------------------------------------

		      /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
		      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
 
		      /* Copy the vertex data to our buffer */
            // glBufferData(type, size in bytes, data, usage) 
		      glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * 5, pVerts, GL_STATIC_DRAW);
		
   // VERTEX data: ---------------------------------------------------------

		      // Set Attribute to 0
            //           WHY - 0? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
            //           GLT_ATTRIBUTE_VERTEX = 0

            // Specifies the index of the generic vertex attribute to be enabled
		      glEnableVertexAttribArray(0);  

		      /* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
            // ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
            void *offsetVert = (void *)((unsigned int)&pVerts[0].x - (unsigned int)pVerts);
		      glVertexAttribPointer(0, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetVert);
		 
 
    // Texture data: ---------------------------------------------------------
     
				// Set Attribute to 3
            //           WHY - 3? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
            //           GLT_ATTRIBUTE_TEXTURE0 = 3

            // Specifies the index of the generic vertex attribute to be enabled
		      glEnableVertexAttribArray(3);  

		      /* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
            // ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
            void *offsetTex = (void *)((unsigned int)&pVerts[0].s - (unsigned int)pVerts);
		      glVertexAttribPointer(3, 2, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetTex);


      // Normal data: ---------------------------------------------------------
		
				// Set Attribute to 2
            //           WHY - 2? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
            //           GLT_ATTRIBUTE_NORMAL = 2

            // Specifies the index of the generic vertex attribute to be enabled
		      glEnableVertexAttribArray(2);  

		      /* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
            // ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
            void *offsetNorm = (void *)((unsigned int)&pVerts[0].nx - (unsigned int)pVerts);
		      glVertexAttribPointer(2, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetNorm);
		 

      // Load the index data: ---------------------------------------------------------
	

            /* Bind our 2nd VBO as being the active buffer and storing index ) */
		      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	         /* Copy the index data to our buffer */
            // glBufferData(type, size in bytes, data, usage) 
		      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList)*6, tlist, GL_STATIC_DRAW);


}



