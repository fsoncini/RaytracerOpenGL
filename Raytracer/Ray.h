#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray {

	Vector3 origin;
	Vector3 direction;

public:

	Ray()
	{
		origin = Vector3();
		direction = Vector3(0.0f, 0.0f, 1.0f);
	}

	Ray(Vector3 o, Vector3 d)
	{
		origin = o;
		direction = d;
	}

	Vector3 get_origin()
	{
		return origin;
	}

	Vector3 get_direction()
	{
		return direction;
	}

};

#endif

