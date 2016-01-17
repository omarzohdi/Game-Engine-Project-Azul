#ifndef GRAPHICS_OBJECT_MANAGER_H
#define GRAPHICS_OBJECT_MANAGER_H

#include "PCSTree.h"

// forward declare (get into the habit, it's good to use
class GraphicsObject;

// Singleton
class GraphicsObjectManager
{
public:

	void addObject( GraphicsObject *p);
	void addObjectToObject(GraphicsObject *child,GraphicsObject *parent);
	void addSibelingToObject(GraphicsObject *inSibeling,GraphicsObject *node);
	void removeObject(GraphicsObject *inNode);
	void drawObjects( void );
	static GraphicsObjectManager * getInstance( void );
	PCSNode * getRoot();

private:
	static GraphicsObjectManager *privGetInstance();
   void privDrawObjectsDepthFirst( GraphicsObject *node ) const;
   GraphicsObjectManager();

   // Data -----------------------------------------
   class PCSTree                 goTree;

};


#endif
