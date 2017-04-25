#pragma once
#include "Vector3.h"



Vector3::Vector3() :
	m_x(0),
	m_y(0),
	m_z(0)
{
}

Vector3::Vector3(double x, double y, double z) :
	m_x(x),
	m_y(y),
	m_z(z)
{
}

double Vector3::length()
{//(double)Math.Sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
	return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

double Vector3::lengthSquared()
{
	return ((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

void Vector3::normalise()
{
	double len = length();
	if (len > 0.0)
	{
		m_x /= len;
		m_y /= len;
		m_z /= len;
	}
}

Vector3 operator+(Vector3 &V, Vector3 &V1)
{
	return Vector3((V.M_X() + V1.M_X()), (V.M_Y() + V1.M_Y()), (V.M_Z() + V1.M_Z()));
}

Vector3 operator-(Vector3 &V, Vector3 &V1)
{
	return Vector3((V.M_X() - V1.M_X()), (V.M_Y() - V1.M_Y()), (V.M_Z() - V1.M_Z()));
}

Vector3 operator-(Vector3 &V)
{
	return Vector3(-V.M_X(), -V.M_Y(), -V.M_Z());
}

double operator*(Vector3 &V, Vector3 &V1)
{
	return ((V.M_X() * V1.M_X()) + (V.M_Y() * V1.M_Y()) + (V.M_Z() * V1.M_Z()));
}

Vector3 operator*(Vector3 &V, double &d)
{
	return Vector3((V.M_X() * d), (V.M_Y() * d), (V.M_Z() * d));
}

Vector3 operator*(Vector3 &V, float &f)
{
	return Vector3((V.M_X() * f), (V.M_Y() * f), (V.M_Z() * f));
}

Vector3 operator*(Vector3 &V, int &i)
{
	return Vector3((V.M_X() * i), (V.M_Y() * i), (V.M_Z() * i));
}

Vector3 operator^(Vector3 &V, Vector3 &V1)
{
	return Vector3(((V.M_Y() * V1.M_Z()) - (V.M_Z() * V1.M_Y())), ((V.M_Z() * V1.M_X()) - (V.M_X() * V1.M_Z())), ((V.M_X() * V1.M_Z()) - (V.M_Y() * V1.M_X())));
}

std::string Vector3::toString()
{
	std::string answer;
	answer = std::to_string(m_x) + "," + std::to_string(m_y) + "," + std::to_string(m_z);
	return answer;
}

double Vector3::M_X()const
{
	return m_x;
}

double Vector3::M_Y()const
{
	return m_y;
}

double Vector3::M_Z()const
{
	return m_z;
}


