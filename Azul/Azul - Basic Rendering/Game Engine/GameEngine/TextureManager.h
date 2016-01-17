#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "OpenGLWrapper.h"
#include "TextureData.h"

struct TextureIDs
{
   TextureIDs  *next;
   GLuint      texID;
};


class TextureManager
{
public:
 //  static TextureManager *getInstance( void ); 
   static GLuint addTexture( TextureData &textData );
   static void deleteTextures( void );

private:
   static TextureManager *privGetInstance( void );
   GLuint privAddTexture( TextureData &textData );
   void   privDeleteTextures( void );

   TextureManager(void);

   // single link list of all the textureIDs
   TextureIDs *head;
   int         textureCount; 

};


#endif