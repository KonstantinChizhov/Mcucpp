#pragma once
#include "Vector3d.h"

class Matrix{
public:
	Matrix();
	~Matrix();
	void Clear();
	inline Vector operator [](int n){return Vector(m[n]);}
	inline Vector Row(int n){return Vector(m[n]);}
	inline Vector Col(int n){return Vector(m[0][n], m[1][n], m[2][n]);}
	 void Rotate(const Vector &v, float a);
	
	 void Translate(const Vector &v);
	
	 Matrix ConcatTranslations(const Matrix &in);
	
	 Matrix ConcatRotations(const Matrix &in);
	
	 Matrix &Mult(const Matrix &in);
	
	Vector_type m[4][4];
};