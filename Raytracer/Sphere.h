#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.h"

class Sphere : public Primitive {
public:

	float radius;

	Sphere()
	{
		radius = 1.0f;
	}

	Sphere(Vector3 cent, float rad)
	{
		position = cent;
		radius = rad;
	}

	virtual Vector3 get_normal(Vector3 point)
	{
		return (point + position.negative()).normalize();
	}

	virtual float get_intersection(Ray ray)
	{
		Vector3 ray_origin = ray.get_origin();
		Vector3 ray_direction = ray.get_direction();

		float a = 1.0f;
		float b = (2.0f*(ray_origin.x() - position.x())*ray_direction.x()) + (2.0f*(ray_origin.y() - position.y())*ray_direction.y()) + (2.0f*(ray_origin.z() - position.z())*ray_direction.z());
		float c = pow(ray_origin.x() - position.x(), 2) + pow(ray_origin.y() - position.y(), 2) + pow(ray_origin.z() - position.z(), 2) - pow(radius, 2);

		float delta = pow(b, 2) - 4 * a*c;

		if (delta > 0.0f) {
			float r1 = (-b - sqrtf(delta)) / 2.0f - 0.00001f;
			float r2 = (sqrtf(delta) - b) / 2.0f - 0.00001f;
			return (r1 > r2) ? r1 : r2;
		}

		return -1.0f;
	}

};

#endif

