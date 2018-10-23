#pragma once
#include "Geometry.h"
#include "Ray.h"
class Object
{
public:
	Vec3f pos;
	Object(float x,float y,float z);
	virtual bool hit(Ray r, float  t) = 0;

};

