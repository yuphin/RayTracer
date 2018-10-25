#pragma once
#include "Geometry.h"
class Material
{
public:
	Vec3f ambientReflectance, diffuseReflectance, specularReflectance, mirrorReflectance;
	float phongExponent;
	Material(const Vec3f& ar, const Vec3f &dr, const Vec3f& sr, const Vec3f& mr, const float& pe);
	~Material();
};

