#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "Time.h"
#include "PCSTree.h"
#include "AnimationObject.h"

// Singleton
class AnimationManager
{
public:

	static AnimationManager * getInstance( void );
	void Update();
	void addObject( AnimationObject *p);

private:
	static AnimationManager *privGetInstance();
	AnimationManager();
	class PCSTree   AnimTree;
	void UpdateAnimations(AnimationObject * node);


};


#endif
