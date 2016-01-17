#include "TextureManager.h"

// private only to this file
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

TextureManager * TextureManager::privGetInstance( void )
{
   // this is the real storage lies
   static TextureManager texMan;

   return ( &texMan );

}

TextureManager::TextureManager(void)
{
    this->textureCount = 0;
    this->head = 0;
}


GLuint TextureManager::addTexture( TextureData &textData )
{
   return ( privGetInstance()->privAddTexture( textData ) );
}

void TextureManager::deleteTextures( )
{
   privGetInstance()->privDeleteTextures();
}

void TextureManager::privDeleteTextures()
{
   TextureIDs *pTextID = this->head;
   TextureIDs *ptmp = 0;

   while( pTextID != 0 )
   {
      // remove from OpenGL
      glDeleteTextures(1, &pTextID->texID);
      ptmp = pTextID;
      this->textureCount--;

      // goto next texture
      pTextID = pTextID->next;

      // remove the pTextID
      this->head = ptmp->next;
      delete ptmp;

   }
}

GLuint TextureManager::privAddTexture( TextureData &textData )
{
   // Generate and bind the texture to OpenGL
   GLuint textID;
   glGenTextures(1, &textID);
   glBindTexture(textData.target, textID);

   // Add the textID to the manager tracking

         TextureIDs *pTextID = new TextureIDs();
         pTextID->texID = textID;

         // push to front of list
         pTextID->next = this->head;
         this->head = pTextID;

         // increment count
         this->textureCount++;

   // do the actual call
   LoadTGATexture(textData.textName, textData.minFilter, textData.magFilter, textData.wrapMode);

   return (textID);
}


// Load a TGA as a 2D Texture. Completely initialize the state
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	
    free(pBits);
    
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR || 
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
    
	return true;
}
