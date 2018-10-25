#pragma once
#include "Object.h"
class Triangle :
	public Object
{
public:
	Vec3f a, b, c,normal;
	Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c,const Material& material);
	~Triangle();
	virtual bool hit(const Ray& r, float & t) override;
	virtual Vec3f getNormal(const Ray & r,float t) override;
};

