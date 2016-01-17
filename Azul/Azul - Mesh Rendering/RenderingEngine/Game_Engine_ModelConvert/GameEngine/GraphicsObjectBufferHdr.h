#ifndef GRAPHICS_OBJECT_BUFFER_HDR_H
#define GRAPHICS_OBJECT_BUFFER_HDR_H

#include <string.h>

#include "TextureData.h"

#define OBJECT_NAME_SIZE   32


struct gObjBufferHdr
{  
	char         objName[ OBJECT_NAME_SIZE ];
   
	// vertex buffer
   int          numVerts;
   int          vertBufferOffset;
   
   // trilist index 
   int          numTriList;
   int          triListBufferOffset;

   BufferData buffData;

   gObjBufferHdr()
   {
      memset( this, 0x0, sizeof(gObjBufferHdr) );
   }
};

#endif