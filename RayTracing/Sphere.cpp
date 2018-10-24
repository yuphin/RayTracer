#include "Sphere.h"
#include <algorithm>
Sphere::Sphere(const Vec3f& vec,float R) :center(vec),R(R) {	}

bool Sphere::hit(Ray r, float& t) {
	float t0, t1;
	auto cVec = r.origin - center;
	if (solve_quadratic_eq(r.direction.dot_product(r.direction), 2 * (r.direction.dot_product(cVec)), cVec.dot_product(cVec) - R * R,t0,t1)) {
		t = std::min({ t0,t1 });
		return true;
	 }
	return false;
}
