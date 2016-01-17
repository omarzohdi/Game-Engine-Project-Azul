#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

enum ChunkType
{
   VERTS_TYPE = 0xA0000000,
   NORMS_TYPE,
   ANIM_TYPE,
   TEXTURE_TYPE,
   UV_TYPE
};

#define CHUNK_NAME_SIZE 20

struct ChunkHeader
{
   ChunkType      type;
   char           chunkName[CHUNK_NAME_SIZE];
   int            chunkSize;
   unsigned int   hashNum;
};



#endif