#pragma once
#include "Object.h"
class Triangle :
	public Object
{
public:
	Vec3f a, b, c;
	Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c);
	~Triangle();
	// Inherited via Object
	virtual bool hit(Ray r, float & t) override;
};

