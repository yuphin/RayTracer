#include "Material.h"
#include "Geometry.h"

Material::Material(const Vec3f & ar, const Vec3f & dr, const Vec3f & sr, const Vec3f & mr, const float & pe) :
	ambientReflectance(ar), diffuseReflectance(dr), specularReflectance(sr), mirrorReflectance(mr),phongExponent(pe) {}
Material::~Material() {}
