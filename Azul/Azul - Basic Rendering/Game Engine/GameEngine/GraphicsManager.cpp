#include <stdio.h>
#include <assert.h>

#include "GraphicsManager.h"
#include "GraphicsObject.h"
#include "NullObject.h"
#include "PCSNode.h"
#include "PCSTree.h"



GraphicsObjectManager::GraphicsObjectManager()
{
   // Get Node 
   PCSNode  *root = this->goTree.getRoot();

   // This should be the first time initializing it.
   assert(root == 0 );

   // Create a Dummy root object, it does nothing
   // remember you need to remove this in the destructor
   // gets trickly with singletons
   NullObject *nodeRoot = new NullObject("Root");

   // insert it into tree
   this->goTree.insert( nodeRoot, root );

}

PCSNode * GraphicsObjectManager::getRoot()
{
		return this->goTree.getRoot();
}


void GraphicsObjectManager::addObject(GraphicsObject *p)
{
   // Attach new node to the root
   // later we will expose more hierarchy

       // Get Node 
      PCSNode  *root = this->goTree.getRoot();

      // This should NOT be the first time using the tree.
      assert(root != 0 );

      // insert it into tree
      this->goTree.insert( p, root );
}

void GraphicsObjectManager::addObjectToObject(GraphicsObject *child,GraphicsObject *parent)
{
   // Attach new node to the root
   // later we will expose more hierarchy

      // This should NOT be the first time using the tree.
      assert(parent != 0 );

      // insert it into tree
      this->goTree.insert( child, parent );
}

void GraphicsObjectManager::addSibelingToObject(GraphicsObject *inSibeling,GraphicsObject *node)
{
   // Attach new node to the root
   // later we will expose more hierarchy

      // This should NOT be the first time using the tree.
      assert(node != 0 );
	  assert(node->getParent() != 0 );

      // insert it into tree
	  this->goTree.insert(inSibeling, node->getParent());
}

void GraphicsObjectManager::removeObject(GraphicsObject *inNode)
{
   // Attach new node to the root
   // later we will expose more hierarchy

      // This should NOT be the first time using the tree.
	  assert(inNode != 0 );

      // insert it into tree
	  this->goTree.remove(inNode);
}




void GraphicsObjectManager::drawObjects()
{
   // Get Node 
   GraphicsObject  *root = (GraphicsObject  *)this->goTree.getRoot();
   this->privDrawObjectsDepthFirst( root );

}


void GraphicsObjectManager::privDrawObjectsDepthFirst( GraphicsObject *node ) const
{
   GraphicsObject *child = 0;

   // --------- Do draw stuff here -----------------------
   
    //node->print();
	node->transform();
	node->setRenderState();
	node->draw();
   
   // --------- Do draw stuff here -----------------------


   // iterate through all of the active children 
   if (node->getChild() != 0)
	{  
	   child =	(GraphicsObject *)node->getChild();
	   // make sure that allocation is not a child node 
	   while (child != 0)
	   {
         privDrawObjectsDepthFirst( child );
         // goto next sibling
         child = (GraphicsObject *)child->getSibling();
	   }
   }
   else
	{
      // bye bye exit condition
	}

}


GraphicsObjectManager * GraphicsObjectManager::getInstance( void )
{
	return privGetInstance();
}


GraphicsObjectManager * GraphicsObjectManager::privGetInstance( void )
{
	static GraphicsObjectManager gom;
	return &gom;
}