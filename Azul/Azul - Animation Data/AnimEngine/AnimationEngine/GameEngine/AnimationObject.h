#ifndef ANIM_OBJECT_H
#define ANIM_OBJECT_H

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
#include "AnimationData.h"

#define NUM_BONES 20
#define BONE_WIDTH 2.0f


class AnimationObject : public PCSNode
{
public:
	void ProcessAnimation( Time tCurr );
	AnimationObject();
	AnimationObject(AnimationData * Animations);

	void SetAnimationPose( GraphicsObject *root, Time tCurr );
	void setBonePose( GraphicsObject *node );
	void walk_anim_node( GraphicsObject *node );
	void SetAnimationData();
	void QueueNextAnimation();
	void findMaxTime( Time &tMax );
	void Update();
	void setStartingPos(Vect &inVect);
	bool isroot;
	void SpeedUP();
	void SlowDown();
	void ForwardPlay();
	void BackwardPlay();
	void Loop();
	int animHeadoffset;
	Frame_Bucket * retrieveIdleAnimationData(int FrameNumber, Frame_Bucket *pPrev);

private:
	PyramidObject *firstBone;
	Pyramid *pPyramid;
	PCSNode *root;
	Time tCurrent;
	Vect start;
	
	float dir;
	float speed;
	bool isLooping;
	bool loopflag;
	bool changeAnim;
	AnimationData * anim;
	Frame_Bucket *currAnimHead;


	void SetAnimatioHierarchy();

};



#endif