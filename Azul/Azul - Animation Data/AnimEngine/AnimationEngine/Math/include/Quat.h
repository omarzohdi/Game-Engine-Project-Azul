/** @file */ 
/*****************************************************************************/
/*                                                                           */
/* file: Quat.h                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H

/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include "Matrix.h"
#include "Enum.h"


	/*************************************************************************
	*
	* This class contains functions and data structure for a 4-dimensional 
	* Quaternions.  The quaternions are internally stored interms of the 
	* imaginary components (x,y,z) and the real components (real).
	*
	* In the Math library, quaternions can be thought of as "rotational"
	* matrices, everything that you can represent with rotational matrices
	* you can use with quaternions.  Such as quaternion concatenation,
	* inverses, transpose, normalize, multiplying with matrices, and with
	* vectors.
	*  
	*  
	**************************************************************************/

class Quat 
{
public:
  
    //ctors and dtors
    Quat();
    Quat(const float x, const float y, const float z, const float w);
    Quat(const Vect& v, const float w);
    Quat(const Matrix& m);
    Quat(const Quat& inQuat);
    const Quat& operator=( const Quat& inQuat);
    const Quat& operator=( const Matrix& inMatrix);
    ~Quat();

    //Special ctors
    explicit Quat(const MatrixSpecialType type);
    Quat(const RotType rotType, float eulerAng);
    Quat(const Rot3AxisType rot3Type, float xAng, float yAng, float zAng);
    Quat(const RotAxisAngleType, const Vect &vAxis, const float angle_radians);
    Quat(const RotOrientType type, const Vect &dof, const Vect &vup);

    void set(const float x, const float y, const float z, const float w = 1.0f);
    void set(const Vect& v, const float w);
    void set(const Matrix& m);
	void set(const Quat& inQuat);
    void set(const MatrixSpecialType type);
    void set(const RotType rotType, float eulerAng);
    void set(const Rot3AxisType rot3Type, float xAng, float yAng, float zAng);
    void set(const RotAxisAngleType, const Vect &vAxis, const float angle_radians);
    void set(const RotOrientType type, const Vect &dof, const Vect &vup); 

    void setVect(const Vect& v);

	//Accessors for coordinates
	const float& operator[](const x_enum) const;
		  float& operator[](const x_enum);
	const float& operator[](const y_enum) const;
		  float& operator[](const y_enum);
	const float& operator[](const z_enum) const;
		  float& operator[](const z_enum);
	const float& operator[](const w_enum) const;
		  float& operator[](const w_enum);

    //Other Accessors
    float getAngle() const;
    void getAxis(Vect &v) const;
    void getVect(Vect &v) const;
    float mag() const;
    float magSquared() const;
    float invMag() const;

    Quat getConj() const;
    Quat getInv() const;
    Quat getNorm() const;
    Quat getT();
    void conj();
    void inv();
    void T();
    Quat& norm();

    const Quat multByElement(const Quat &q) const;
    float dot(const Quat &q) const;
    void Lqcvq(const Vect&, Vect& out) const;
    void Lqvqc(const Vect&, Vect& out) const;

    //boolean operators
    bool isNormalized(const float epsilon) const;
    bool isZero(const float epsilon) const;
    bool isIdentity(const float epsilon) const;
    bool isEqual(const Quat& q, const float epsilon) const;
    bool isEquivalent(const Quat& q, const float epsilon) const;
    bool isConjugateEqual(const Quat& q, const float epsilon) const;
    bool isNegEqual(const Quat& q, const float epsilon) const;

    //binary operators
	const Quat operator+(const Quat& inQuat) const;
	const Quat operator-(const Quat& inQuat) const;
    const Quat operator*(const Quat& inQuat) const;
    const Quat operator/(const Quat& inQuat) const;

    const Quat& operator+=(const Quat& inQuat);
	const Quat& operator-=(const Quat& inQuat);
    const Quat& operator*=(const Quat& inQuat);
    const Quat& operator/=(const Quat& inQuat);

    const Quat operator+(const float& inFloat) const;
    const Quat operator-(const float& inFloat) const;
	const Quat operator*(const float& inFloat) const;
    const Quat operator/(const float& inFloat) const;
    const Quat& operator+=(const float& inFloat);
    const Quat& operator-=(const float& inFloat);
	const Quat& operator*=(const float& inFloat);
    const Quat& operator/=(const float& inFloat);

	const Quat operator*(const Matrix& inMatrix) const;
	const Quat& operator*=(const Matrix& inMatrix);

    //unary operators
	const Quat operator+() const;
	const Quat operator-() const;

private:
    //helper functions
    void setRotOrient( const Vect &vect_dof, const Vect &vect_vup);
	void setRotInvOrient( const Vect &vect_dof, const Vect &vect_vup);

// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)
	// anonymous union
	union 
	{
		__m128	m;

		// anonymous struct
		struct 
			{
			float qx;
			float qy;
			float qz;
			float qw;
			};
	};


};


//Non-member functions related to Quat's operations
//unless it becomes neccessary, this is not a friend function
Quat operator+(const float inFloat, const Quat& Quat);
Quat operator-(const float inFloat, const Quat& Quat);
Quat operator*(const float inFloat, const Quat& Quat);
Quat operator/(const float inFloat, const Quat& Quat);

#endif // #ifndef QUAT_H

/**** END of Quat.h ********************************************************/
