#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

class Material {
	Color base_color;
	float diffuse;
	float specular;
	float reflection;

public:
	Material()
	{
		base_color = Color(Vector3(1.0f, 1.0f, 1.0f), 1.0f);
		diffuse = 0.5f;
		specular = 0.6f;
		reflection = 0.2f;
	}

	Material(Color color, float diffuse, float specular, float reflection)
	{
		base_color = color;
		this->diffuse = diffuse;
		this->specular = specular;
		this->reflection = reflection;
	}

	Color get_color()
	{
		return base_color;
	}

	float get_diffuse()
	{
		return diffuse;
	}

	float get_specular()
	{
		return specular;
	}

	float get_reflection()
	{
		return reflection;
	}

};

#endif