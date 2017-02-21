#ifndef PLANE_H
#define PLANE_H

#include "Primitive.h"

class Plane : public Primitive {

	Vector3 normal;
	float distance;

public:

	Plane()
	{
		normal = Vector3(0.0f, 1.0f, 0.0f);
		distance = 0.0f;
	}

	Plane(Vector3 n, float dist)
	{
		//position = pos;
		normal = n;
		distance = dist;
	}

	virtual Vector3 get_normal(Vector3 point)
	{
		return normal;
	}

	float get_distance()
	{
		return distance;
	}

	virtual float get_intersection(Ray ray)
	{
		Vector3 ray_direction = ray.get_direction();
		float a = ray_direction.dot(normal);

		if (a == 0) {
			return -1.0f;
		}
		else {
			//float b = normal.dot(ray.get_origin() + (normal*distance).negative());
			//return -b*a;
			float dist = -(ray.get_origin().dot(normal) + -distance) / a;
			return dist;
		}
	}

};

#endif

