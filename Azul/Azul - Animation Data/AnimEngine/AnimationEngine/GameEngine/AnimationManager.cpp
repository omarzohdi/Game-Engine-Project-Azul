#include "AnimationManager.h"
#include "Time.h"
#include "PCSTree.h"


AnimationManager::AnimationManager()
{
	PCSNode  *root = this->AnimTree.getRoot();
	AnimationObject *nodeRoot = new AnimationObject();
	nodeRoot->isroot = true;

   // insert it into tree
   this->AnimTree.insert( nodeRoot, root );

}

AnimationManager * AnimationManager::getInstance( void )
{
	return privGetInstance();
}

AnimationManager * AnimationManager::privGetInstance( void )
{
	static AnimationManager anm;
	return &anm;
}

void AnimationManager::Update()
{
	static Time tCurrent(TIME_ZERO);

	AnimationObject *root = (AnimationObject  *)this->AnimTree.getRoot();
	this->UpdateAnimations( root );
}

void AnimationManager::UpdateAnimations(AnimationObject *node)
{
	AnimationObject *child = 0;

   // --------- Do draw stuff here -----------------------
	if (!node->isroot)
		node->Update();
   
   // --------- Do draw stuff here -----------------------

   if (node->getChild() != 0)
	{  
	   child =	(AnimationObject *)node->getChild();
	  
	   while (child != 0)
	   {
         UpdateAnimations( child );
         child = (AnimationObject *)child->getSibling();
	   }
   }
}

void AnimationManager::addObject(AnimationObject *p)
{
      PCSNode  *root = this->AnimTree.getRoot();
	  
	  this->AnimTree.insert( p, root );
}
