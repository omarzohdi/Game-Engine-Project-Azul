#ifndef OPENGLWRAPPER_H
#define OPENGLWRAPPER_H

#include <GLTools.h>	// OpenGL toolkit

#define FREEGLUT_STATIC
#include <GL/glut.h>


// When enabliing warning level 4, we get warning C4505: unreferenced local function has been removed
// This pragma kills this needless warning
#pragma warning(disable: 4505)


#endif