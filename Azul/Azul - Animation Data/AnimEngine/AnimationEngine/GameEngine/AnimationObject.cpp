#include "AnimationObject.h"
#include "Time.h"

#include "OpenGLWrapper.h"
#include "PyramidObject.h"
#include "GraphicsManager.h"
#include "Pyramid.h"


AnimationObject::AnimationObject()
{
	anim = 0;
	this->currAnimHead = 0;
	PCSNode *root = 0;
	this->tCurrent;
	start.set(0.0f,0.0f,0.0f);
	isroot = false;
	int animHeadoffset = 0;
	int animHeadtotal = 0 ;
	this->dir = 1.0f;
	this->speed = 1.05f;
	isLooping = true;
	loopflag = true;
	changeAnim = false;
}
AnimationObject::AnimationObject(AnimationData* inAnim)
{
	anim = inAnim;
	if (anim == 0)
		this->currAnimHead = 0;
	else
		this->currAnimHead = anim->getHead();

	PCSNode *root = 0;
	this->tCurrent;
	start.set(0.0f,0.0f,0.0f);
	SetAnimatioHierarchy();
	
	isroot = false;
	
	int animHeadoffset = 0;
	this->dir = 1.0f;
	this->speed = 1.05f;
	isLooping = true;
	loopflag = true;
	changeAnim = false;
}

void AnimationObject::setStartingPos(Vect &inVect)
{
	start.set(inVect);
}

void AnimationObject::ProcessAnimation( Time tCurr )
{
	
	Frame_Bucket *pTmp = this->currAnimHead->nextBucket;

	// Get the result bone array
	// Remember the firs Frame is the result
	Bone *bResult = this->currAnimHead->pBone;

	// First one is the result, skip it
	pTmp = this->currAnimHead->nextBucket;
 
	// Find which key frames
	while( tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0 )
	{
		pTmp = pTmp->nextBucket;
	}

	// pTmp is the "B" key frame
	// pTmp->prev is the "A" key frame
	Frame_Bucket *pA = pTmp->prevBucket;
	Frame_Bucket *pB = pTmp;

	// find the "S" of the time
	float tS = (tCurr - pA->KeyTime)  /(pB->KeyTime - pA->KeyTime);
	
	// interpolate to 
	Bone *bA = pA->pBone;
	Bone *bB = pB->pBone;

   // Interpolate to tS time, for all bones
	for( int i = 0; i < NUM_BONES; i++ )
	{
		// interpolate ahoy!
		VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
		QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
		VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
		
		// advance the pointer
		bA++;
		bB++;
		bResult++;
   }
}

void AnimationObject::SetAnimatioHierarchy()
{
   // setup the bone model, this case we are using pyramid
   // todo - create a cool Bone Object, ball(drk blue) + pyramid arm (dark yellow)
	pPyramid = new Pyramid();
	pPyramid->createVAO();
	pPyramid->loadTexture();
	
   // Get the manager
	GraphicsObjectManager *goMgr = GraphicsObjectManager::getInstance();


// Attached it into the tree
	PCSTree *tree = goMgr->getTree();
   //PCSNode *root = tree->getRoot();
    root = tree->getRoot();

	PyramidObject *pBip01 = new PyramidObject( "xxx", pPyramid );
	pBip01->setIndex(0);
	pBip01->setName("Bip01");
	tree->insert( pBip01, root);

	PyramidObject *pBip01_Pelvis = new PyramidObject( "xxx", pPyramid );
	pBip01_Pelvis->setIndex(1);
	pBip01_Pelvis->setName("Bip01_Pelvis");
	tree->insert( pBip01_Pelvis, pBip01 );

	PyramidObject *pBip01_Spine = new PyramidObject( "xxx", pPyramid );
	pBip01_Spine->setIndex(2);
	pBip01_Spine->setName("Bip01_Spine");
	tree->insert( pBip01_Spine, pBip01_Pelvis );

	PyramidObject *pBip01_L_Thigh = new PyramidObject( "xxx", pPyramid );
	pBip01_L_Thigh->setIndex(3);
	pBip01_L_Thigh->setName("Bip01_L_Thigh");
	tree->insert( pBip01_L_Thigh, pBip01_Spine );

	PyramidObject *pBip01_L_Calf = new PyramidObject( "xxx", pPyramid );
	pBip01_L_Calf->setIndex(4);
	pBip01_L_Calf->setName("Bip01_L_Calf");
	tree->insert( pBip01_L_Calf, pBip01_L_Thigh );

	PyramidObject *pBip01_L_Foot = new PyramidObject( "xxx", pPyramid );
	pBip01_L_Foot->setIndex(5);
	pBip01_L_Foot->setName("Bip01_L_Foot");
	tree->insert( pBip01_L_Foot, pBip01_L_Calf ); 
	
	PyramidObject *pBip01_R_Thigh = new PyramidObject( "xxx", pPyramid );
	pBip01_R_Thigh->setIndex(6);
	pBip01_R_Thigh->setName("Bip01_R_Thigh");
	tree->insert( pBip01_R_Thigh, pBip01_Spine );

	PyramidObject *pBip01_R_Calf = new PyramidObject( "xxx", pPyramid );
	pBip01_R_Calf->setIndex(7);
	pBip01_R_Calf->setName("Bip01_R_Calf");
	tree->insert( pBip01_R_Calf, pBip01_R_Thigh );

	PyramidObject *pBip01_R_Foot = new PyramidObject( "xxx", pPyramid );
	pBip01_R_Foot->setIndex(8);
	pBip01_R_Foot->setName("Bip01_R_Foot");
	tree->insert( pBip01_R_Foot, pBip01_R_Calf );

	PyramidObject *pBip01_Spine1 = new PyramidObject( "xxx", pPyramid );
	pBip01_Spine1->setIndex(9);
	pBip01_Spine1->setName("Bip01_Spine1");
	tree->insert( pBip01_Spine1, pBip01_Spine );

	PyramidObject *pBip01_Neck = new PyramidObject( "xxx", pPyramid );
	pBip01_Neck->setIndex(10);
	pBip01_Neck->setName("Bip01_Neck");
	tree->insert( pBip01_Neck, pBip01_Spine1 );

	PyramidObject *pBip01_L_Clavicle = new PyramidObject( "xxx", pPyramid );
	pBip01_L_Clavicle->setIndex(11);
	pBip01_L_Clavicle->setName("Bip01_L_Clavicle");
	tree->insert( pBip01_L_Clavicle, pBip01_Neck );

	PyramidObject *pBip01_L_UpperArm = new PyramidObject( "xxx", pPyramid );
	pBip01_L_UpperArm->setIndex(12);
	pBip01_L_UpperArm->setName("Bip01_L_UpperArm");
	tree->insert( pBip01_L_UpperArm, pBip01_L_Clavicle );

	PyramidObject *pBip01_L_Forearm = new PyramidObject( "xxx", pPyramid );
	pBip01_L_Forearm->setIndex(13);
	pBip01_L_Forearm->setName("Bip01_L_Forearm");
	tree->insert( pBip01_L_Forearm, pBip01_L_UpperArm );

	PyramidObject *pBip01_L_Hand = new PyramidObject( "xxx", pPyramid );
	pBip01_L_Hand->setIndex(14);
	pBip01_L_Hand->setName("Bip01_L_Hand");
	tree->insert( pBip01_L_Hand, pBip01_L_Forearm );

	PyramidObject *pBip01_R_Clavicle = new PyramidObject( "xxx", pPyramid );
	pBip01_R_Clavicle->setIndex(15);
	pBip01_R_Clavicle->setName("Bip01_R_Clavicle");
	tree->insert( pBip01_R_Clavicle, pBip01_Neck );

	PyramidObject *pBip01_R_UpperArm = new PyramidObject( "xxx", pPyramid );
	pBip01_R_UpperArm->setIndex(16);
	pBip01_R_UpperArm->setName("Bip01_R_UpperArm");
	tree->insert( pBip01_R_UpperArm, pBip01_R_Clavicle );

	PyramidObject *pBip01_R_Forearm = new PyramidObject( "xxx", pPyramid );
	pBip01_R_Forearm->setIndex(17);
	pBip01_R_Forearm->setName("Bip01_R_Forearm");
	tree->insert( pBip01_R_Forearm, pBip01_R_UpperArm );

	PyramidObject *pBip01_R_Hand = new PyramidObject( "xxx", pPyramid );
	pBip01_R_Hand->setIndex(18);
	pBip01_R_Hand->setName("Bip01_R_Hand");
	tree->insert( pBip01_R_Hand, pBip01_R_Forearm );

	PyramidObject *pBip01_Head = new PyramidObject( "xxx", pPyramid );
	pBip01_Head->setIndex(19);
	pBip01_Head->setName("Bip01_Head");
	tree->insert( pBip01_Head, pBip01_Neck );


	pBip01->setLightColor( Vect(0.5f, 1.5f, 0.5f) );
	pBip01_L_Foot->setLightColor( Vect(1.5f, 0.5f, 0.5f) );
	pBip01_R_Foot->setLightColor( Vect(0.5f, 0.5f, 1.5f) );

	firstBone = pBip01;

	tree->dumpTree();

}

void AnimationObject::SetAnimationPose( GraphicsObject *root, Time tCurr )
{
	// First thing, get the first frame of animation
//	Time tCurr(TIME_ZERO);
	ProcessAnimation( tCurr );

	// walks the anim node does the pose for everything that
	walk_anim_node( root );
}

void AnimationObject::setBonePose( GraphicsObject *node )
{
	// Now get the world matrices
	GraphicsObject *childNode = (GraphicsObject *)node;
	GraphicsObject *parentNode = (GraphicsObject *)node->getParent();
	  
	if( parentNode == root )
		return;

	if( parentNode != 0 && childNode != 0 )
	{
		// starting point
		//start = Vect(0,0,0);

	   //  At this point, Find the two bones initial positions in world space
	   //  Now get the length and directions
		parentNode->Animtransform(this->currAnimHead);
		Vect ptA = start * parentNode->getWorld();

		childNode->transform();
		Vect ptB = start * childNode->getWorld();

		// direction between the anchor points of the respective bones
		Vect dir = -(ptB - ptA);

		// length of the bone 0
		float mag = dir.mag();


		Matrix T_flip(TRANS, 0.0f, 0.0f, 1.0f);

		// rotates it to Z axis
		Matrix R_flip( ROT_X, 180.0f * MATH_PI_180);

		Matrix S( SCALE, BONE_WIDTH, BONE_WIDTH, mag);
		Quat Q( ROT_ORIENT, dir.getNorm(), Vect( 0.0f, 1.0f, 0.0f) );
		Matrix T( TRANS, ptB);
		
		Matrix M;
		M.set(Q);

		Matrix BoneOrient = S * M * T ;

		childNode->setBoneOrientation(BoneOrient); 
	}
}

void AnimationObject::walk_anim_node( GraphicsObject *node ) 
{

   // --------- Do pose stuff here -----------------------
   
	setBonePose( node );
   
   // --------- Do pose stuff here -----------------------

    GraphicsObject *child = 0;

   // iterate through all of the active children 
   if (node->getChild() != 0)
	{  
	   child =	(GraphicsObject *)node->getChild();
	   // make sure that allocation is not a child node 
	   while (child != 0)
	   {
          walk_anim_node( child );
         // goto next sibling
         child = (GraphicsObject *)child->getSibling();
	   }
   }
   else
	{
      // bye bye exit condition
	}
}

void AnimationObject::findMaxTime( Time &tMax )
{
		Frame_Bucket *pTmp = this->currAnimHead->nextBucket;
		while( pTmp->nextBucket != 0 )
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;
}

void AnimationObject::Update()
{
	if (!this->isroot)
	{
		this->SetAnimationPose( firstBone, tCurrent );
		// Clean this stuff up into an animation object

		// update the time
		Time deltaTime = 0.1f* Time(TIME_NTSC_30_FRAME);
		Time maxTime;
		Time Zero;
		
		findMaxTime( maxTime );


		if ( loopflag )
		{
			tCurrent += (this->dir) * deltaTime / (this->speed);
		}
		else if (changeAnim)
		{
			SetAnimationData();
			isLooping = true;
		}

		if( tCurrent < Time(TIME_ZERO) )
		{
			tCurrent = maxTime;
		}
		else if ( tCurrent > maxTime )
		{
			tCurrent = Time(TIME_ZERO);
			loopflag = false;
		}
		
		if(isLooping)
			loopflag = true;

	}
}

void AnimationObject::SlowDown()
{
	this->speed += 0.01f;

}
void AnimationObject::SpeedUP()
{
	this->speed -= 0.01f;
	if (this->speed <= 0)
		this->speed = 0.01f;


}
void AnimationObject::ForwardPlay()
{
	this->dir = 1;
}
void AnimationObject::BackwardPlay()
{
	this->dir = -1;
}
void AnimationObject::Loop()
{
	if (this->isLooping == false)
		isLooping = true;
	else 
		isLooping = false;
}

void AnimationObject::QueueNextAnimation()
{
	animHeadoffset++;
	if (animHeadoffset >= this->anim->totalAnimations)
		animHeadoffset = 0;

	changeAnim = true;
	isLooping = false;
}
void AnimationObject::SetAnimationData()
{
	this->currAnimHead = anim->getHead(animHeadoffset);
	changeAnim = false;
}
