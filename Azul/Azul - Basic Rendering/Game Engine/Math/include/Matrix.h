#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H

// SIMD includes
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Enum.h"
#include "Vect.h"



class Matrix
{
public:

	// Big 4
	Matrix();
	const Matrix &operator = (const Matrix &A);
	Matrix( const Matrix &tM );
	~Matrix();

	// Constructors
	Matrix( const Vect &tV0, const Vect &tV1, const Vect &tV2, const Vect &tV3);
	Matrix( RotType type, const float angle );
	Matrix( MatrixTransType type, const float tx, const float ty, const float tz );
	Matrix( MatrixScaleType type, const float sx, const float sy, const float sz );
    explicit Matrix( MatrixSpecialType type );

	// Set 
	void set(const Vect &V0, const Vect &V1, const Vect &V2, const Vect &V3);
	void set( RotType type, const float angle );
	void set( const RotAxisAngleType, const Vect &vAxis, const float angle_radians );
	void set( const RotOrientType , const Vect &dof, const Vect &up);
	void set( MatrixTransType type, const float tx, const float ty, const float tz );
	void set( MatrixScaleType type, const float sx, const float sy, const float sz );
	void set( MatrixSpecialType type );
	void set(MatrixRowType type, const Vect &V);

	// Get
	Vect get(MatrixRowType type) const;

	// Matrix functions
	const float det() const;
	Matrix &T( void );
	const Matrix getT( void )const ;
	const Matrix getInv( void ) const;
	Matrix &inv( void );

	// comparison
	bool isEqual( const Matrix &A, const float epsilon = MATH_TOLERANCE) const;
	bool isIdentity( const float epsilon = MATH_TOLERANCE) const;

	// bracket operators	
	float &operator[] (const enum m0_enum );
	float &operator[] (const enum m1_enum );
	float &operator[] (const enum m2_enum );
	float &operator[] (const enum m3_enum );	
	float &operator[] (const enum m4_enum );	
	float &operator[] (const enum m5_enum );	
	float &operator[] (const enum m6_enum );	
	float &operator[] (const enum m7_enum );	
	float &operator[] (const enum m8_enum );	
	float &operator[] (const enum m9_enum );	
	float &operator[] (const enum m10_enum );	
	float &operator[] (const enum m11_enum );	
    float &operator[] (const enum m12_enum );	
	float &operator[] (const enum m13_enum );	
	float &operator[] (const enum m14_enum );	
	float &operator[] (const enum m15_enum );
	const float operator[] (const enum m0_enum ) const;
	const float operator[] (const enum m1_enum ) const;	
	const float operator[] (const enum m2_enum ) const;	
	const float operator[] (const enum m3_enum ) const;	
	const float operator[] (const enum m4_enum ) const;	
	const float operator[] (const enum m5_enum ) const;	
	const float operator[] (const enum m6_enum ) const;	
	const float operator[] (const enum m7_enum ) const;	
	const float operator[] (const enum m8_enum ) const;
	const float operator[] (const enum m9_enum ) const;	
	const float operator[] (const enum m10_enum ) const;	
	const float operator[] (const enum m11_enum ) const;	
    const float operator[] (const enum m12_enum ) const;	
	const float operator[] (const enum m13_enum ) const;	
	const float operator[] (const enum m14_enum ) const;	
	const float operator[] (const enum m15_enum ) const;

	// add/sub operators
	Matrix operator + (void) const 	{return Matrix(*this);	};
	Matrix operator + (const Matrix &A) const ;
	void operator += (const Matrix &A);
	Matrix operator - (void) const;
	Matrix operator - (const Matrix &A) const ;
	void operator -= (const Matrix &A);

	// Multiply
	Matrix operator * (const float s) const ;
	friend Matrix operator *(const float scale, const Matrix &A );
	void operator *= (const float scale);
	Matrix operator * (const Matrix &A) const;
	void  operator *= (const Matrix &A);

private:
	const Matrix Matrix::getAdj(void) const;
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

#endif