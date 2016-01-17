#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H

// SIMD includes
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Enum.h"
#include "Vect.h"

class Quat;

class Matrix
{
public:

	Matrix();
	Matrix(const Vect& v0, const Vect& v1, const Vect& v2, const Vect& v3);
	//No 16 parameter constructor, because I shudder just thinking about it
	Matrix(const Matrix& inMatrix);
	const Matrix& operator=(const Matrix& inMatrix);
	~Matrix();

	//Special case constructors
	explicit Matrix(const MatrixSpecialType type);
	explicit Matrix(const RotType rotType, float eulerAng);
	explicit Matrix(const MatrixTransType transType, const float x, const float y, const float z);
	explicit Matrix(const MatrixScaleType scaleType, const float x, const float y, const float z);

	Matrix(const Quat &q);
	Matrix( MatrixTransType type, const Vect &vIn );
	Matrix( MatrixScaleType type, const Vect &vIn );

	//set functions
	void set(const Vect& v0, const Vect& v1, const Vect& v2, const Vect& v3);
    void set(const Quat& q);
	void set(const Matrix& inMatrix);
	void set(const MatrixSpecialType type);
	void set(const RotType rotType, const float eulerAng);
    void set(const RotOrientType mode, const Vect &dof, const Vect &vup);
    void set(const RotAxisAngleType, const Vect &vAxis, const float angle_radians );
	void set(const MatrixTransType transType, const float x, const float y, const float z);
	void set(const MatrixScaleType scaleType, const float x, const float y, const float z);
	void set(const MatrixRowType row, const Vect& inVect);

	//get function
	Vect get(MatrixRowType type) const;

	//Accessors for coordinates
	const float& operator[](const m0_enum) const;
		  float& operator[](const m0_enum);
	const float& operator[](const m1_enum) const;
		  float& operator[](const m1_enum);
	const float& operator[](const m2_enum) const;
		  float& operator[](const m2_enum);
	const float& operator[](const m3_enum) const;
		  float& operator[](const m3_enum);
	const float& operator[](const m4_enum) const;
		  float& operator[](const m4_enum);
	const float& operator[](const m5_enum) const;
		  float& operator[](const m5_enum);
	const float& operator[](const m6_enum) const;
		  float& operator[](const m6_enum);
	const float& operator[](const m7_enum) const;
		  float& operator[](const m7_enum);
	const float& operator[](const m8_enum) const;
		  float& operator[](const m8_enum);
	const float& operator[](const m9_enum) const;
		  float& operator[](const m9_enum);
	const float& operator[](const m10_enum) const;
		  float& operator[](const m10_enum);
	const float& operator[](const m11_enum) const;
		  float& operator[](const m11_enum);
	const float& operator[](const m12_enum) const;
		  float& operator[](const m12_enum);
	const float& operator[](const m13_enum) const;
		  float& operator[](const m13_enum);
	const float& operator[](const m14_enum) const;
		  float& operator[](const m14_enum);
	const float& operator[](const m15_enum) const;
		  float& operator[](const m15_enum);

	//binary operators
	const Matrix operator+(const Matrix& inMatrix) const;

	const Matrix operator-(const Matrix& inMatrix) const;

	const Matrix operator*(const Matrix& inMatrix) const;

	const Matrix operator*(const float& inFloat) const;

	const Matrix& operator+=(const Matrix& inMatrix);

	const Matrix& operator-=(const Matrix& inMatrix);

	const Matrix& operator*=(const Matrix& inMatrix);

	const Matrix& operator*=(const float& inFloat);

	//unary operators
	const Matrix operator+() const;
	const Matrix operator-() const;

	//other math functions
	const float det() const;
	void T();
	const Matrix getT() const;
	void inv();
	const Matrix getInv() const;

	//boolean functions
	const bool isEqual(const Matrix& inMatrix, const float epsilon = 0.0001f) const;
	const bool isIdentity(const float epsilon = 0.0001f) const;

private:
    //helper functions
    void setRotOrient( const Vect &vect_dof, const Vect &vect_vup);
	void setRotInvOrient( const Vect &vect_dof, const Vect &vect_vup);
	
// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)

union 
		{
		struct 
			{
			Vect v0;
			Vect v1;
			Vect v2;
			Vect v3;
			};

		struct 
			{
			float m0;
			float m1;
			float m2;
			float m3;
			float m4;
			float m5;
			float m6;	
			float m7;
			float m8;
			float m9;
			float m10;
			float m11;
			float m12;
			float m13;
			float m14;
			float m15;
			};
		};
};

//Non-member functions related to Matrix's operations
//unless it becomes neccessary, this is not a friend function
Matrix operator*(const float inFloat, const Matrix& Matrix);

#endif