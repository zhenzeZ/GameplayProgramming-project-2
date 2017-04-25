#pragma once
#include <math.h>
#include <string>
class Vector3
{
public:
	Vector3();
	Vector3(double x, double y, double z);
	double length();
	double lengthSquared();
	void normalise();
	friend Vector3 operator+ (Vector3 &, Vector3 &);
	friend Vector3 operator- (Vector3 &, const Vector3 &);
	friend Vector3 operator- (Vector3 &);
	friend double operator* (Vector3 &, Vector3 &);
	friend Vector3 operator* (Vector3 &, double &);
	friend Vector3 operator* (Vector3 &, float &);
	friend Vector3 operator* (Vector3 &, int &);
	friend Vector3 operator^ (Vector3 &, Vector3 &);
	std::string toString();
	double M_X()const;
	double M_Y()const;
	double M_Z()const;

private:
	double m_x;
	double m_y;
	double m_z;

};

