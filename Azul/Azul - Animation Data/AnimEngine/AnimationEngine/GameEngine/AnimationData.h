#ifndef ANIM_DATA_H
#define ANIM_DATA_H

#include "Time.h"
#include "PCSNode.h"
#include "MathApp.h"
#include "MathEngine.h"
#include "Constants.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "OpenGLWrapper.h"
#include "PyramidObject.h"
#include "GraphicsManager.h"
#include "Pyramid.h"
#include "AnimDataTypes.h"
#include "ChunkHeader.h"
#include "PackageHeader.h"
#include "File.h"

#define NUM_BONES 20
#define BONE_WIDTH 2.0f



class AnimationData : public PCSNode
{
public:
	AnimationData();
	AnimationData(char * filename);
	Frame_Bucket * getHead();
	Frame_Bucket * getHead(int offset);
	Frame_Bucket * getNext(Frame_Bucket * pTmp);
	Frame_Bucket * getPrev(Frame_Bucket * pTmp);
	int totalAnimations;

private:
	void LoadDatafromFile(char * filename);
	void LoadAnimationData(FileHandle &fh, int i);
	void CopyAnimationData(Frame_Bucket * pHead);
	Frame_Bucket * pHead;
	Frame_Bucket * currFrame;
	Frame_Bucket ** pHeads;
	


};



#endif