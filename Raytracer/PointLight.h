#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Color.h"

class PointLight {

	Vector3 position;
	Color color;

public:

	PointLight()
	{
		position = Vector3();
		color = Color();
	}

	PointLight(Vector3 pos, Color clr)
	{
		position = pos;
		color = clr;
	}

	Vector3 get_position()
	{
		return position;
	}

	Color get_color()
	{
		return color;
	}

};

#endif
