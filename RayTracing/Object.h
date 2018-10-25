#pragma once
#include "Geometry.h"
#include "Ray.h"
#include "Material.h"
#include "HitRecord.h"
class Object
{
public:
	Material material;
	Object(const Material& material);
	virtual bool hit(const Ray& r, float&  t) = 0;
	virtual Vec3f getNormal(const Ray&r,float t) = 0;

};

