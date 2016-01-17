#include <stdio.h>
#include "OpenGLWrapper.h"
#include "GlobalState.h"
#include "UserInterface.h"
#include "RenderScene.h"
#include "Pyramid.h"
#include "PyramidObject.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "Cube.h"
#include "CubeObject.h"
#include "SqrAPrism.h"
#include "SqrAPrismObject.h"


Camera *pCamera;

bool SphereRender = false;
int PyramidTexture = 1;
int CubeTexture = 2;
int SqrAPrismTexture = 3;
PyramidObject *PT;

void createGraphicsWindow()
{
	printf("createGraphicsWindows()\n");
	// Set the display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Set the window size
	glutInitWindowSize(1280, 800);

	// create the window and name it.
	glutCreateWindow("Graphic Demo");

	// Callback if someone changes size
   glutReshapeFunc(ChangeSize);

	// Callback for Special key presses
   glutSpecialFunc(SpecialKeys);

   // Callback for normal Key Presses
   glutKeyboardFunc(KeyPressFunc);

	//call back for Rendering (Draw) Scene
   glutDisplayFunc(RenderScene);

		// Extension library - initialize
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		// return error
		}

   // Startup
	frameworkStartup();

}

// User functions, called at start up (once)
void frameworkStartup( void )
{

   // todo: move this to a one time call only
   pCamera = new Camera();

   // Initially setup the camera
	pCamera->setViewport( 0, 0,800, 600);
	pCamera->setPerspective( 35.0f, float(800)/float(600), 1.0f, 500.0f);
	pCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,2.5f,-18.0f), Vect(0.0f,0.0f,7.0f) );
   	
   //srand(time(NULL));

   // Create Objects
	Pyramid *pPyramid = new Pyramid();
	Cube *pCube = new Cube();
	SqrAPrism *pSqrAPrism = new SqrAPrism();

	GraphicsObjectManager *goMgr = GraphicsObjectManager::getInstance();

#if CREATE_PYRAMID_DATA
	////////////////TRANSFORMATION///////////////////////////////
	PyramidObject *p = new PyramidObject( "pyramid_A",pPyramid);
	p->setStartPos( Vect(-3.0f, 3.0f, -6.0f) );
	p->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p->setLightColor( Vect(0.0f, 1.0f, 0.0f, 1.0f)*2.0f );
	p->setRotationSpeeds(-0.001f,0.005f,0);
	p->loadObjectData();
	goMgr->addObject(p);

   PyramidObject	*p1 = new PyramidObject( "pyramid_B",pPyramid);
	p1->setStartPos( Vect(-3.0f, 1.0f, -6.0f) );
	p1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p1->setLightColor( Vect(1.0f, 0.0f, 0.0f, 1.0f)*2.0f );
	p1->setRotationSpeeds(-0.005f,0.002f, 0.003f);
	p1->loadObjectData();
	goMgr->addObjectToObject(p1,p);

   PyramidObject *p2 = new PyramidObject( "pyramid_C",pPyramid);
	p2->setStartPos( Vect(-3.0f, -1.0f, -6.0f) );
	p2->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p2->setLightColor( Vect(0.30f, 0.30f, 1.2f, 1.0f)*2.0f );
	p2->setRotationSpeeds(-0.003f,-0.001f, 0.001f);
	p2->loadObjectData();
	goMgr->addObjectToObject(p2,p1);

   PyramidObject *p3 = new PyramidObject( "pyramid_D",pPyramid);
	p3->setStartPos( Vect(-3.0f, -3.0f, -6.0f) );
	p3->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p3->setLightColor( Vect(1.0f, 1.0f, 1.0f, 1.0f)*2.0f );
	p3->setRotationSpeeds(-0.003f,-0.005f, 0.004f);
	p3->loadObjectData();
	goMgr->addObjectToObject(p3,p2);


	//////////////////SHADERS///////////////////////////////
	PyramidObject *p_1 = new PyramidObject( "pyramid_AB",pPyramid);
	p_1->setStartPos( Vect(-3.0f, 2.5f, -18.0f) );
	p_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p_1->setRotationSpeeds(-0.001f,0.005f,0.001f);
	p_1->objectRenderState = 0;
	p_1->loadObjectData();
	goMgr->addObject(p_1);

	PyramidObject *p1_1 = new PyramidObject( "pyramid_BB",pPyramid);
	p1_1->setStartPos( Vect(-3.0f, 0.5f, -18.0f) );
	p1_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p1_1->setRotationSpeeds(-0.005f,0.002f,0.0f);
	p1_1->objectRenderState = 1;//rand() % 3;
	p1_1->loadObjectData();
	goMgr->addObject(p1_1);

	PyramidObject *p2_1 = new PyramidObject( "pyramid_CB",pPyramid);
	p2_1->setStartPos( Vect(-3.0f, -1.5f, -18.0f) );
	p2_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p2_1->setRotationSpeeds(-0.003f,-0.001f, -0.004f);
	p2_1->objectRenderState = 2;//rand() % 3;
	p2_1->loadObjectData();
	goMgr->addObject(p2_1);

	////////////////////////////////TEXTURE///////////////////////////////
	PyramidObject *p1_2 = new PyramidObject( "pyramid_BC",pPyramid);
	p1_2->setStartPos( Vect(-3.0f, 0.5f, -30.0f) );
	p1_2->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	p1_2->setRotationSpeeds(-0.005f,0.002f,0.0f);
	p1_2->setTranslationDir(0.007f,0.0f,0.0f);
	p1_2->objectRenderState = 3;//rand() % 3;
	p1_2->loadObjectData();
	goMgr->addObject(p1_2);


#else


	//////////////////////////////////////TRANSFORMATION///////////////////////////////////
	PyramidObject *p = new PyramidObject( "pyramid_A",pPyramid);
	p->loadObjectData();
	goMgr->addObject(p);

	PyramidObject	*p1 = new PyramidObject( "pyramid_B",pPyramid);
	p1->loadObjectData();
	goMgr->addObjectToObject(p1,p);

	PyramidObject *p2 = new PyramidObject( "pyramid_C",pPyramid);
	p2->loadObjectData();
	goMgr->addObjectToObject(p2,p1);

	PyramidObject *p3 = new PyramidObject( "pyramid_D",pPyramid);
	p3->loadObjectData();
	goMgr->addObjectToObject(p3,p2);

	//////////////////////////////////////////////////SHADERS///////////////////////////////
	PyramidObject *p_1 = new PyramidObject( "pyramid_AB",pPyramid);
	p_1->loadObjectData();
	goMgr->addObject(p_1);

	PyramidObject *p1_1 = new PyramidObject( "pyramid_BB",pPyramid);
	p1_1->loadObjectData();
	goMgr->addObject(p1_1);

	PyramidObject *p2_1 = new PyramidObject( "pyramid_CB",pPyramid);
	p2_1->loadObjectData();
	goMgr->addObject(p2_1);

	/////////////////////////////////TEXTURE///////////////////////////////
	PyramidObject *p1_2 = new PyramidObject( "pyramid_BC",pPyramid);
	p1_2->loadObjectData();
	goMgr->addObject(p1_2);

#endif

#if CREATE_CUBE_DATA

	/////////////////////////////////////////////TRANSFORMATION//////////////////////////////
	CubeObject *c = new CubeObject( "Cube_A",pCube);
	c->setStartPos( Vect(0.0f, 3.0f, -6.0f) );
	c->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	c->setLightColor( Vect(0.0f, 1.0f, 0.0f, 1.0f)*2.0f );
	c->setRotationSpeeds(0.003f,-0.005f, 0.0f);
	c->loadObjectData();
	goMgr->addSibelingToObject(c,p);

	CubeObject	*c1 = new CubeObject( "Cube_B",pCube );
	c1->setStartPos( Vect(0.0f, 1.0f, -6.0f) );
	c1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	c1->setLightColor( Vect(1.0f, 0.0f, 0.0f, 1.0f)*2.0f );
	c1->setRotationSpeeds(0.01f,-0.002f, 0.01f);
	c1->loadObjectData();
	goMgr->addObjectToObject(c1,c);

	CubeObject *c2 = new CubeObject( "Cube_C",pCube );
	c2->setStartPos( Vect(0.0f, -1.0f, -6.0f) );
	c2->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	c2->setLightColor( Vect(0.30f, 0.30f, 1.2f, 1.0f)*2.0f );
	c2->setRotationSpeeds(0.007f,-0.001f, -0.007f);
	c2->loadObjectData();
	goMgr->addObjectToObject(c2,c1);

	CubeObject *c3 = new CubeObject( "Cube_D" ,pCube);
	c3->setStartPos( Vect(0.0f, -3.0f, -6.0f) );
	c3->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	c3->setLightColor( Vect(1.0f, 1.0f, 1.0f, 1.0f)*2.0f );
	c3->setRotationSpeeds(0.005f,-0.005f, 0.0f);
	c3->loadObjectData();
	goMgr->addObjectToObject(c2,c3);

	///////////////////////////////////////SHADERS//////////////////////////////////////
	CubeObject *c_1 = new CubeObject( "Cube_AB",pCube);
	c_1->setStartPos( Vect(0.0f, 2.5f, -18.0f) );
	c_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	c_1->setRotationSpeeds(0.003f,-0.005f, -0.007f);
	c_1->objectRenderState = 0;
	c_1->loadObjectData();
	goMgr->addObject(c_1);

	CubeObject	*c1_1 = new CubeObject( "Cube_BB",pCube );
	c1_1->setStartPos( Vect(0.0f, 0.5f, -18.0f) );
	c1_1->setLightPos( Vect(1.0f, 0.0f, 0.0f) );
	c1_1->setRotationSpeeds(0.01f,-0.002f, 0.004f);
	c1_1->objectRenderState = 1;
	c1_1->loadObjectData();
	goMgr->addObject(c1_1);

	CubeObject *c2_1 = new CubeObject( "Cube_CB",pCube );
	c2_1->setStartPos( Vect(0.0f, -1.5f, -18.0f) );
	c2_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	c2_1->setRotationSpeeds(0.007f,-0.001f, 0.009f);
	c2_1->objectRenderState = 2;
	c2_1->loadObjectData();
	goMgr->addObject(c2_1);

	/////////////////////////////////////TEXTURE/////////////////////////////////////

	CubeObject	*c1_2 = new CubeObject( "Cube_BC",pCube );
	c1_2->setStartPos( Vect(0.0f, 0.5f, -30.0f) );
	c1_2->setLightPos( Vect(1.0f, 0.0f, 0.0f) );
	c1_2->setRotationSpeeds(0.01f,-0.002f, -0.005f);
	c1_2->setTranslationDir(0.0f,0.007f,0.0f);
	c1_2->objectRenderState = 3;//rand() % 3;
	c1_2->loadObjectData();
	goMgr->addObject(c1_2);


#else

	/////////////////////////////////////////////TRANSFORMATION//////////////////////////////
	CubeObject *c = new CubeObject( "Cube_A",pCube);
	c->loadObjectData();
	goMgr->addSibelingToObject(c,p);

	CubeObject	*c1 = new CubeObject( "Cube_B",pCube );
	c1->loadObjectData();
	goMgr->addObjectToObject(c1,c);

	CubeObject *c2 = new CubeObject( "Cube_C",pCube );
	c2->loadObjectData();
	goMgr->addObjectToObject(c2,c1);

	CubeObject *c3 = new CubeObject( "Cube_D" ,pCube);
	c3->loadObjectData();
	goMgr->addObjectToObject(c3,c2);

	///////////////////////////////////////SHADERS//////////////////////////////////////
	CubeObject *c_1 = new CubeObject( "Cube_AB",pCube);
	c_1->loadObjectData();
	goMgr->addObject(c_1);

	CubeObject	*c1_1 = new CubeObject( "Cube_BB",pCube );
	c1_1->loadObjectData();
	goMgr->addObject(c1_1);

	CubeObject *c2_1 = new CubeObject( "Cube_CB",pCube );
	c2_1->loadObjectData();
	goMgr->addObject(c2_1);

	/////////////////////////////////////TEXTURE/////////////////////////////////////

	CubeObject	*c1_2 = new CubeObject( "Cube_BC",pCube );
	c1_2->loadObjectData();
	goMgr->addObject(c1_2);

#endif
	
#if CREATE_SQRAPRISM_DATA

	/////////////////////////////////////////////TRANSFORMATION//////////////////////////////
	SqrAPrismObject *s = new SqrAPrismObject( "SqrAPrism_A" ,pSqrAPrism );
	s->setStartPos( Vect(3.0f, 3.0f, -6.0f) );
	s->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s->setLightColor( Vect(0.0f, 1.0f, 0.0f, 1.0f)*2.0f );
	s->setRotationSpeeds(0.001f,0.001f, 0.001f);
	s->loadObjectData();
	goMgr->addSibelingToObject(s,p);

	SqrAPrismObject	*s1 = new SqrAPrismObject( "SqrAPrism_B" ,pSqrAPrism );
	s1->setStartPos( Vect(3.0f, 1.0f, -6.0f) );
	s1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s1->setLightColor( Vect(1.0f, 0.0f, 0.0f, 1.0f)*2.0f );
	s1->setRotationSpeeds(0.0f,0.002f, 0.01f);
	s1->loadObjectData();
	goMgr->addObjectToObject(s1,s);

	SqrAPrismObject *s2 = new SqrAPrismObject( "SqrAPrism_C",pSqrAPrism );
	s2->setStartPos( Vect(3.0f, -1.0f, -6.0f) );
	s2->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s2->setLightColor( Vect(0.30f, 0.30f, 1.2f, 1.0f)*2.0f );
	s2->setRotationSpeeds(-0.001f,0.003f, -0.01f);
	s2->loadObjectData();
	goMgr->addObjectToObject(s2,s1);

	SqrAPrismObject *s3 = new SqrAPrismObject( "SqrAPrism_D",pSqrAPrism );
	s3->setStartPos( Vect(3.0f, -3.0f, -6.0f) );
	s3->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s3->setLightColor( Vect(1.0f, 1.0f, 1.0f, 1.0f)*2.0f );
	s3->setRotationSpeeds(-0.003f,0.001f, 0.005f);
	s3->loadObjectData();
	goMgr->addObjectToObject(s3,s2);
	
	/////////////////////////////////////////////SHADERS//////////////////////////////
	SqrAPrismObject *s_1 = new SqrAPrismObject( "SqrAPrism_AB" ,pSqrAPrism );
	s_1->setStartPos( Vect(3.0f, 2.5f, -18.0f) );
	s_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s_1->setRotationSpeeds(0.001f,0.001f, 0.02f);
	s_1->objectRenderState = 0;//rand() % 3;
	s_1->loadObjectData();
	goMgr->addObject(s_1);

	SqrAPrismObject	*s1_1 = new SqrAPrismObject( "SqrAPrism_BB" ,pSqrAPrism );
	s1_1->setStartPos( Vect(3.0f, 0.5f, -18.0f) );
	s1_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s1_1->setRotationSpeeds(0.0f,0.002f, -0.004f);
	s1_1->objectRenderState = 1;//rand() % 3;
	s1_1->loadObjectData();
	goMgr->addObject(s1_1);

	SqrAPrismObject *s2_1 = new SqrAPrismObject( "SqrAPrism_CB",pSqrAPrism );
	s2_1->setStartPos( Vect(3.0f, -1.5f, -18.0f) );
	s2_1->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s2_1->setRotationSpeeds(-0.001f,0.003f, -0.01f);
	s2_1->objectRenderState = 2;//rand() % 3;
	s2_1->loadObjectData();
	goMgr->addObject(s2_1);

	/////////////////////////////////////////////TEXTURES//////////////////////////////
	SqrAPrismObject	*s1_2 = new SqrAPrismObject( "SqrAPrism_BC" ,pSqrAPrism );
	s1_2->setStartPos( Vect(3.0f, 0.5f, -30.0f) );
	s1_2->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	s1_2->setRotationSpeeds(0.0f,0.002f, 0.01f);
	s1_2->setTranslationDir (0.0f, 0.0f, 0.007f);
	s1_2->objectRenderState = 3;//rand() % 3;
	s1_2->loadObjectData();
	goMgr->addObject(s1_2);

#else

	/////////////////////////////////////////////TRANSFORMATION//////////////////////////////
	SqrAPrismObject *s = new SqrAPrismObject( "SqrAPrism_A" ,pSqrAPrism );
	s->loadObjectData();
	goMgr->addSibelingToObject(s,p);

	SqrAPrismObject	*s1 = new SqrAPrismObject( "SqrAPrism_B" ,pSqrAPrism );
	s1->loadObjectData();
	goMgr->addObjectToObject(s1,s);

	SqrAPrismObject *s2 = new SqrAPrismObject( "SqrAPrism_C",pSqrAPrism );
	s2->loadObjectData();
	goMgr->addObjectToObject(s2,s1);

	SqrAPrismObject *s3 = new SqrAPrismObject( "SqrAPrism_D",pSqrAPrism );
	s3->loadObjectData();
	goMgr->addObjectToObject(s3,s2);
	
	/////////////////////////////////////////////SHADERS//////////////////////////////
	SqrAPrismObject *s_1 = new SqrAPrismObject( "SqrAPrism_AB" ,pSqrAPrism );
	s_1->loadObjectData();
	goMgr->addObject(s_1);

	SqrAPrismObject	*s1_1 = new SqrAPrismObject( "SqrAPrism_BB" ,pSqrAPrism );
	s1_1->loadObjectData();
	goMgr->addObject(s1_1);

	SqrAPrismObject *s2_1 = new SqrAPrismObject( "SqrAPrism_CB",pSqrAPrism );
	s2_1->loadObjectData();
	goMgr->addObject(s2_1);


	/////////////////////////////////////////////TEXTURES//////////////////////////////
	SqrAPrismObject	*s1_2 = new SqrAPrismObject( "SqrAPrism_BC" ,pSqrAPrism );
	s1_2->loadObjectData();
	goMgr->addObject(s1_2);


#endif;


	PT = new PyramidObject( "pyramid_AT" ,pPyramid);
	PT->loadObjectData();
	PT->setStartPos( Vect(-3.0f, -1.0f, -48.0f) );
	PT->setLightPos( Vect(1.0f, 1.0f, 0.0f) );
	PT->setLightColor( Vect(0.0f, 1.0f, 0.0f, 1.0f)*2.0f );
	PT->setRotationSpeeds(-0.001f,0.005f,0);
	goMgr->addObject(PT);

}





