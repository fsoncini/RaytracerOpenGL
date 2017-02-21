#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera {

	Vector3 position;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

public:

	Camera()
	{
		position = Vector3();
		right = Vector3(1.0f, 0.0f, 0.0f);
		up = Vector3(0.0f, 1.0f, 0.0f);
		forward = Vector3(0.0f, 0.0f, 1.0f);
	}

	Camera(Vector3 pos, Vector3 r, Vector3 u, Vector3 f)
	{
		position = pos;
		right = r;
		up = u;
		forward = f;
	}

	Vector3 get_position()
	{
		return position;
	}

	Vector3 get_right()
	{
		return right;
	}

	Vector3 get_up()
	{
		return up;
	}

	Vector3 get_forward()
	{
		return forward;
	}

};

#endif
