#include "Sphere.h"
#include <algorithm>
Sphere::Sphere(const Vec3f& vec,float R,const Material& material) : Object(material),center(vec),R(R) {	}

Vec3f Sphere::getNormal(const Ray & r, float t) {
	return (r.origin + r.direction*t - center).normalize();
	
}
bool Sphere::hit(const Ray& r, float& t) {
	float t0, t1;
	auto cVec = r.origin - center;
	if (solve_quadratic_eq(r.direction.dot_product(r.direction), 2 * (r.direction.dot_product(cVec)), cVec.dot_product(cVec) - R * R,t0,t1) && (t0 >= 0.0f || t1>= 0.0f )) {
		t = std::min({ t0,t1 });
		if (t < 0)
			t = t1;
	
		return true;
	 }
	return false;
}


