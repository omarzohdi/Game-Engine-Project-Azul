#ifndef CAMERA_H
#define CAMERA_H

#include "MathEngine.h"

class Camera
{
public:
	// Default constructor
   Camera();

   // Setup on single camera
	void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
	void setViewport(const int inX, const int inY, const int width, const int height);
	void setOrientAndPosition(const Vect &inUp, const Vect &inLookAt, const Vect &pos);

   // update camera system
	void updateCamera(void);

   // Get the matrices for rendering
	Matrix &getViewMatrix();
	Matrix &getProjMatrix();

	// accessors
	void getPos( Vect &outPos) const;
	void setPos( const Vect &inPos);
			
	void getFieldOfView( float &Value) const;
	void setFieldOfView( const float Value);

	void getNearDist( float &Value) const;
	void setNearDist( const float Value);

	void getFrontNorm( Vect &outNorm );
	void getBackNorm( Vect &outNorm );
	void getRightNorm( Vect &outNorm );
	void getLeftNorm( Vect &outNorm );
	void getTopNorm( Vect &outNorm ); 
	void getBottomNorm( Vect &outNorm );

	void getNearBottomLeft( Vect &outNBL );
	void getFarTopRight( Vect &outFTR );

	void startOrbit( void );
	void stopOrbit( void );

		Vect	vDir;
private:  // methods should never be public
   void privSetViewState( void );
	void privCalcPlaneHeightWidth( void );
	void privCalcFrustumVerts( void );
	void privCalcFrustumCollisionNormals( void );
	void privUpdateProjectionMatrix( void );
	void privUpdateViewMatrix( void );
	void Orbit( void );

			
private:  // data  (Keep it private)

	// camera unit vectors (up, dir, right)
	Vect	vUp;
	//Vect	vDir;
	Vect	vRight;  // derived by up and dir
	Vect	vPos;    

	// Define the frustum inputs
	float	nearDist;
	float	farDist;
	float	fovy;
	float	aspectRatio;

	// Screen size in world space
	float	near_height;
	float	near_width;
	float	far_height;
	float	far_width;

	// viewports
	int		viewport_x;
	int		viewport_y;
	int		viewport_width;
	int		viewport_height;

	// world space coords for viewing frustum
	Vect	nearTopLeft;
	Vect	nearTopRight;
	Vect	nearBottomLeft;
	Vect	nearBottomRight;
	Vect	farTopRight;
	Vect	farTopLeft;
	Vect	farBottomLeft;
	Vect	farBottomRight;

	// Normals of the frustum
	Vect	frontNorm;
	Vect	backNorm;
	Vect	rightNorm;
	Vect	leftNorm;
	Vect	topNorm;
	Vect	bottomNorm;

	// Projection Matrix
	Matrix	projMatrix;
	Matrix	viewMatrix;

	//Other
	bool IsOrbit;
};


#endif

