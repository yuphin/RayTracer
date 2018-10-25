#pragma once
#include "Object.h"
class Sphere :
	public Object
{
public:
	// Inherited via Object
	float R;
	Vec3f center;
	Sphere(const Vec3f& vec,float R);
	virtual bool hit(Ray r, float& t) override;

};

