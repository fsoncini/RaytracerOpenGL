#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Ray.h"
#include "Material.h"

class Primitive {
public:

	Vector3 position;
	Material material;

	Primitive()
	{
		position = Vector3();
		material = Material();
	}

	virtual float get_intersection(Ray ray)
	{
		return -1.0f;
	}

	virtual Vector3 get_normal(Vector3 point)
	{
		return Vector3();
	}

	Material get_material()
	{
		return material;
	}

	void set_material(Material m)
	{
		material = m;
	}

};

#endif
