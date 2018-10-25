#pragma once
#include "Geometry.h"
#include "Ray.h"
class Object
{
public:
	Object();
	virtual bool hit(Ray r, float&  t) = 0;

};

