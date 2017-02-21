#ifndef Vector3_H
#define Vector3_H

#include <math.h>

class Vector3 {

public:

	float v[3];

	Vector3()
	{
		v[0] = v[1] = v[2] = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	float magnitude()
	{
		return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	Vector3 normalize()
	{
		float m = magnitude();
		return Vector3(v[0] / m, v[1] / m, v[2] / m);
	}

	Vector3 negative()
	{
		return (*this) * -1.0f;
	}

	float dot(const Vector3 &rhs)
	{
		return v[0] * rhs.v[0] + v[1] * rhs.v[1] + v[2] * rhs.v[2];
	}

	Vector3 cross(const Vector3 &rhs)
	{
		return Vector3(v[1] * rhs.v[2] - v[2] * rhs.v[1], v[2] * rhs.v[0] - v[0] * rhs.v[2], v[0] * rhs.v[1] - v[1] * rhs.v[0]);
	}

	float get_squared_distance(const Vector3 &rhs)
	{
		float dx = rhs.v[0] - v[0];
		float dy = rhs.v[1] - v[1];
		float dz = rhs.v[2] - v[2];
		return dx*dx + dy*dy + dz*dz;
	}

	float get_distance(const Vector3 &rhs)
	{
		return sqrtf(get_squared_distance(rhs));
	}

	float x() { return v[0]; }
	float y() { return v[1]; }
	float z() { return v[2]; }

	Vector3 operator *(const float &scalar)
	{
		return Vector3(scalar*v[0], scalar*v[1], scalar*v[2]);
	}

	Vector3 operator *(const Vector3 &rhs)
	{
		return Vector3(v[0] * rhs.v[0], v[1] * rhs.v[1], v[2] * rhs.v[2]);
	}

	Vector3 operator +(const Vector3 &rhs)
	{
		return Vector3(v[0] + rhs.v[0], v[1] + rhs.v[1], v[2] + rhs.v[2]);
	}

	Vector3 operator -(const Vector3 &rhs)
	{
		return Vector3(v[0] - rhs.v[0], v[1] - rhs.v[1], v[2] - rhs.v[2]);
	}
};

#endif

