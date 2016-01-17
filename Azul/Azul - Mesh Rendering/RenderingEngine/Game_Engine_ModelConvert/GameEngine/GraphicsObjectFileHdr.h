#ifndef GRAPHICS_OBJECT_FILE_HDR_H
#define GRAPHICS_OBJECT_FILE_HDR_H

#include <string.h>

#include "TextureData.h"

#define OBJECT_NAME_SIZE   32


struct gObjFileHdr
{
   // object name
   char         objName[ OBJECT_NAME_SIZE ];
   
   // vertex buffer
   int          numVerts;
   int          vertBufferOffset;
   
   // trilist index 
   int          numTriList;
   int          triListBufferOffset;

   // texture info
  // char         textName[ TEXTURE_NAME_SIZE ];
  // unsigned int textTarget;
  // unsigned int textMinFilter;
  // unsigned int textMagFilter;
   //unsigned int textWrapMode;
   TextureData textData;

   gObjFileHdr()
   {
      memset( this, 0x0, sizeof(gObjFileHdr) );
   }
};

#endif