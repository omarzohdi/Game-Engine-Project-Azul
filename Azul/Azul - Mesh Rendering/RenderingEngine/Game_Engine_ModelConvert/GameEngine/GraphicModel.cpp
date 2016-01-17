#include <assert.h>
#include "OpenGLWrapper.h"
#include "MathEngine.h"
#include "GraphicModel.h"
#include "Camera.h"
#include "File.h"
#include "GraphicsObjectFileHdr.h"
#include "TextureManager.h"
#include "PackageHeader.h"
#include "ChunkHeader.h"
#include "GraphicsObjectBufferHdr.h"
#include <vector>
#include <algorithm>

GraphicModel::GraphicModel(char * filename)
{
	loadDatafromFile(filename);
}

void GraphicModel::loadDatafromFile(char * filename)
{
	FileHandle fh;
	FileError ferror;
	PackageHeader * pkH = new PackageHeader();
	ChunkHeader * CDM = new ChunkHeader();
	ChunkHeader * VBO = new ChunkHeader();
	ChunkHeader * TRI = new ChunkHeader();
	ChunkHeader * TEXRGB = new ChunkHeader();
	CDM_Header * ModelHeader = new CDM_Header();

	////////////////////////////////////READ PACKAGE//////////////////////////////

	ferror = File::open(fh,filename,FILE_READ);
	assert(ferror == FILE_SUCCESS);

	ferror= File::read(fh,pkH,sizeof(PackageHeader));
	assert(ferror == FILE_SUCCESS);

	if (pkH->numChunks > 0)
	{
		ferror= File::read(fh,CDM,sizeof(ChunkHeader));
		assert(ferror == FILE_SUCCESS);

		ferror= File::read(fh,ModelHeader,sizeof(CDM_Header));
		assert(ferror == FILE_SUCCESS);
	}

	if (pkH->numChunks > 1)
	{
		////////////////////////////////////VBOS///////////////////////////////////////
		ferror= File::read(fh,VBO,sizeof(ChunkHeader));
		assert(ferror == FILE_SUCCESS);

		int totalsize = VBO->chunkSize/ sizeof(FBX_Vertex_vsn);
		this->model_vert.reserve(totalsize);
		FBX_Vertex_vsn * tempRead = new FBX_Vertex_vsn();

		for (int i=0; i< totalsize; i++)
		{
			ferror= File::read(fh,tempRead,sizeof(FBX_Vertex_vsn));

			this->model_vert.push_back(*tempRead);
		}

	}

	if (pkH->numChunks > 2)
	{
		/////////////////////////////////TRIANGLE LIST///////////////////////////////

		ferror= File::read(fh,TRI,sizeof(ChunkHeader));
		assert(ferror == FILE_SUCCESS);

		int totalsize = TRI->chunkSize/ sizeof(VBO_Trilist);
		this->model_trilist.reserve(totalsize);
		VBO_Trilist * tempRead = new VBO_Trilist();

		for (int i=0; i< totalsize; i++)
		{
			ferror= File::read(fh,tempRead,sizeof(VBO_Trilist));

			this->model_trilist.push_back(*tempRead);
		};

	}

	//////////////////////////////CREATE VAO with TRILIST and VBOs////////////////////////
	createVAO();
	

	if (pkH->numChunks > 3 && ModelHeader->incTex)
	{
		////////////////////////////////////TEXTURE//////////////////////////////////////
		ferror= File::read(fh,TEXRGB,sizeof(ChunkHeader));
		assert(ferror == FILE_SUCCESS);

		TEX_Header * texHeader = new TEX_Header();
		ferror = File::read(fh, texHeader,sizeof(TEX_Header));
		
		assert(ferror == FILE_SUCCESS);
		
		int size = TEXRGB->chunkSize - sizeof(TEX_Header);
		char * texBuffer = (char *)malloc(size);

		ferror = File::read(fh, texBuffer,size);

		assert(ferror == FILE_SUCCESS);

		ferror = File::close(fh);

		loadTexturefromBuffer(texBuffer,*texHeader);

		free (texBuffer);
	}
	else
	{
		//loadTexture(texName)
	}


	delete	pkH;
	delete  CDM;		
	delete  VBO;		
	delete  TRI;		
	delete  TEXRGB;		
	delete	ModelHeader;
}

MyVertex_stride * GraphicModel::ConvertVertextoVao( void )
{
	MyVertex_stride * pVerts = new MyVertex_stride[this->model_vert.size()];

	for (int i=0; i<(int)this->model_vert.size(); i++)
	{
		pVerts[i].x = model_vert[i].x;
		pVerts[i].y = model_vert[i].y;
		pVerts[i].z = model_vert[i].z;
		
		pVerts[i].s = model_vert[i].s;
		pVerts[i].t = model_vert[i].t;

		pVerts[i].nx = model_vert[i].nx;
		pVerts[i].ny = model_vert[i].ny;
		pVerts[i].nz = model_vert[i].nz;
	}

	return pVerts;
}

MyTriList * GraphicModel::ConvertVBOTritoTrilist( void )
{
	MyTriList * tlist = new MyTriList[this->model_trilist.size()];

	for (int i=0; i<(int)this->model_trilist.size(); i++)
	{
		tlist[i].v1 = this->model_trilist[i].v0;
		tlist[i].v2 = this->model_trilist[i].v1;
		tlist[i].v3 = this->model_trilist[i].v2;
	}

	return tlist;
}

void GraphicModel::createVAO( void )
{
	MyVertex_stride * pVerts = ConvertVertextoVao();

	MyTriList * tlist = ConvertVBOTritoTrilist();

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
		      glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * this->model_vert.size(), pVerts, GL_STATIC_DRAW);
		
   // VERTEX data: ---------------------------------------------------------

            // Specifies the index of the generic vertex attribute to be enabled
		      glEnableVertexAttribArray(0);  

		   /* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
			void *offsetVert = (void *)((unsigned int)&pVerts[0].x - (unsigned int)pVerts);
		      glVertexAttribPointer(0, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetVert);
		 
 
    // Texture data: ---------------------------------------------------------
     
				// Set Attribute to 3
            //           WHY - 3? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
            //           GLT_ATTRIBUTE_TEXTURE0 = 3

            // Specifies the index of the generic vertex attribute to be enabled
				glEnableVertexAttribArray(3);  

		      /* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
				void *offsetTex = (void *)((unsigned int)&pVerts[0].s - (unsigned int)pVerts);
				glVertexAttribPointer(3, 2, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetTex);


      // Normal data: ---------------------------------------------------------
		
            // Specifies the index of the generic vertex attribute to be enabled
		      glEnableVertexAttribArray(2);  

		      /* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
			void *offsetNorm = (void *)((unsigned int)&pVerts[0].nx - (unsigned int)pVerts);
			glVertexAttribPointer(2, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetNorm);
		 

      // Load the index data: ---------------------------------------------------------
	

            /* Bind our 2nd VBO as being the active buffer and storing index ) */
		      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	         /* Copy the index data to our buffer */
		      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList)*this->model_trilist.size(), tlist, GL_STATIC_DRAW);


}

void GraphicModel::loadTexture( char * texName)
{

   gObjFileHdr  graphicsFileHdr;

   // object name
   strncpy_s( graphicsFileHdr.objName, OBJECT_NAME_SIZE, "model", _TRUNCATE);

   // squirrel away the parameters
   graphicsFileHdr.textData.target = GL_TEXTURE_2D;
   graphicsFileHdr.textData.minFilter = GL_LINEAR;
   graphicsFileHdr.textData.magFilter = GL_LINEAR;
   graphicsFileHdr.textData.wrapMode = GL_CLAMP_TO_EDGE;
   strncpy_s( graphicsFileHdr.textData.textName, TEXTURE_NAME_SIZE, texName, _TRUNCATE);

   this->textureID = TextureManager::loadFile( graphicsFileHdr.textData );

}

void GraphicModel::loadTexturefromBuffer( char * buffer, TEX_Header texHeader)
{
	gObjBufferHdr  graphicsBufferHdr;

	// object name
	strncpy_s( graphicsBufferHdr.objName, OBJECT_NAME_SIZE, "model", _TRUNCATE);

	// squirrel away the parameters
	graphicsBufferHdr.buffData.target = GL_TEXTURE_2D;
	graphicsBufferHdr.buffData.minFilter = GL_LINEAR;
	graphicsBufferHdr.buffData.magFilter = GL_LINEAR;
	graphicsBufferHdr.buffData.wrapMode = GL_CLAMP_TO_EDGE;
	graphicsBufferHdr.buffData.Height = texHeader.height;
	graphicsBufferHdr.buffData.Width = texHeader.width;
	graphicsBufferHdr.buffData.bitDepth = texHeader.bitDepth;

	this->textureID = TextureManager::loadBuffer( graphicsBufferHdr.buffData, buffer);

}

int GraphicModel::getVBOCount()
{
	return this->model_vert.size();
}
int GraphicModel::getTriangleCount ()
{
	return this->model_trilist.size();
}