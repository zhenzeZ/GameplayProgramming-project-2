#pragma once
#include "Vector3.h"
#include <iostream>

#ifndef MATRIX3_H
#define MATRIX3_H

class Matrix3
{
public:
	Matrix3();
	Matrix3(Vector3 &row1, Vector3 &row2, Vector3 &row3);
	Matrix3(double _A11, double _A12, double _A13,
		double _A21, double _A22, double _A23,
		double _A31, double _A32, double _A33);


	enum class Axis { X, Y, Z };
	friend Vector3 operator *(Matrix3 M, Vector3 &V);
	Matrix3 Transpose();
	friend Matrix3 operator +(Matrix3 M, Matrix3 &M1);
	friend Matrix3 operator -(Matrix3 M, Matrix3 &M1);
	friend Matrix3 operator *(Matrix3 M, double &d);
	friend Matrix3 operator *(Matrix3 M, Matrix3 &M1);
	double  Determinant();
	Matrix3 Inverse();
	Vector3 Column(int c);
	Vector3 Row(int r);
	static Matrix3  Rotation(double  angle, const Axis &);

	std::string toString();

private:
	double m_A1_x;
	double m_A1_y;
	double m_A1_z;
	double m_A2_x;
	double m_A2_y;
	double m_A2_z;
	double m_A3_x;
	double m_A3_y;
	double m_A3_z;
};



#endif