#pragma once
#include "Geometry.h"
#include "Ray.h"
struct Camera{
	Vec3f e, u, v, w;
	float distance;
	Camera(Vec3f origin, Vec3f gaze, Vec3f up, float d);
	Ray cast_ray(float uP, float vP);
};

