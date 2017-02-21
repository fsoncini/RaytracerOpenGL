#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"

class Color {

	Vector3 rgb;
	float alpha;

public:

	Color()
	{
		rgb = Vector3(1.0f, 1.0f, 1.0f);
		alpha = 1.0f;
	}

	Color(Vector3 clr, float a)
	{
		rgb = Vector3(fmin(clr.x(), 1.0f), fmin(clr.y(), 1.0f), fmin(clr.z(), 1.0f));
		alpha = a;
	}

	Vector3 get_rgb()
	{
		return rgb;
	}

	float get_alpha()
	{
		return alpha;
	}

};

#endif
