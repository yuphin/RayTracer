#pragma once
#include "Object.h"
class Sphere :
	public Object
{
public:
	// Inherited via Object
	float R;
	Vec3f center;
	Sphere(const Vec3f& vec,float R,const Material& material);
	virtual bool hit(const Ray& r, float& t) override;
	inline Vec3f getNormal(const Vec3f & point) {
		return (point - center).normalize();
	}


	// Inherited via Object
	virtual Vec3f getNormal(const Ray & r,float t) override;

};

