#ifndef PYRAMID_H
#define PYRAMID_H

class Pyramid
{

// Holds the bulk data and texture
public:
   void loadTexture( void );
   void createVAO( void );

   GLuint				vao;
	GLuint				textureID;
};

#endif