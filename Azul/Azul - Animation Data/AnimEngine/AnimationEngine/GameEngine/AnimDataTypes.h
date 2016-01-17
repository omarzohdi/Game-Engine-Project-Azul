

#pragma once

#include"MathEngine.h"
#include"Time.h"


enum AnimationType
{
	WALK = 0,
	IDLE,
	NONE
};

struct Bone 
{
   Vect  T;
   Quat  Q;
   Vect  S;
};

struct Frame_Bucket
{
	Frame_Bucket *nextBucket;
	Frame_Bucket *prevBucket;
	Time		  KeyTime;
	Bone		  *pBone;
};