#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include "OpenGLWrapper.h"

#define TEXTURE_NAME_SIZE  32

struct TextureData
{
   char        textName[ TEXTURE_NAME_SIZE ]; 
   GLenum      target;     // texture target
   GLenum      minFilter;  // min/mag filters
   GLenum      magFilter;
   GLenum      wrapMode;   // texture wrap moode
};


#endif