#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Enum.h"

// forward declare
class Matrix;
class Quat;

class Vect
{
public:

	//ctors and dtors
	Vect();
	Vect(const float x, const float y, const float z, const float w = 1.0f);
	Vect(const Vect& inVect);
	const Vect& operator=( const Vect& inVect);
	~Vect();

	//set function
	void set(const float x, const float y, const float z, const float w = 1.0f);
	void set(const Vect& inVect);

	//Accessors for coordinates
	const float& operator[](const x_enum) const;
		  float& operator[](const x_enum);
	const float& operator[](const y_enum) const;
		  float& operator[](const y_enum);
	const float& operator[](const z_enum) const;
		  float& operator[](const z_enum);
	const float& operator[](const w_enum) const;
		  float& operator[](const w_enum);

	//binary operators
	const Vect operator+(const Vect& inVect) const;

	const Vect operator-(const Vect& inVect) const;

	const Vect operator*(const float& inFloat) const;

	const Vect operator*(const Matrix& inMatrix) const;

    const Vect operator*(const Quat& inQuat) const;

	const Vect& operator+=(const Vect& inVect);

	const Vect& operator-=(const Vect& inVect);

	const Vect& operator*=(const float& inFloat);

	const Vect& operator*=(const Matrix& inMatrix);

    const Vect& operator*=(const Quat& inQuat);

	//unary operators
	const Vect operator+() const;
	const Vect operator-() const;

	//other math functions
	const float dot(const Vect& inVect) const;
	const Vect cross(const Vect& inVect) const;
	const float mag() const;
	const float magSqr() const;
	void norm();
	const Vect getNorm() const;
	const float getAngle(const Vect& inVect) const;

	//boolean functions
	const bool isEqual(const Vect& inVect, const float epsilon = 0.0001f) const;
	const bool isZero(const float epsilon = 0.0001f) const;

private:

// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)
	// anonymous union
	union 
	{
		__m128	m;

		// anonymous struct
		struct 
			{
			float x;
			float y;
			float z;
			float w;
			};
	};
};

//Non-member functions related to Vector's operations
//unless it becomes neccessary, this is not a friend function
Vect operator*(const float inFloat, const Vect& inVect);

#endif