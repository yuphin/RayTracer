#pragma once
#include "Geometry.h"
class Light
{
public:
	Vec3f pos, intensity;
	Light(const Vec3f& pos,const Vec3f& intensity);
	~Light();
};

