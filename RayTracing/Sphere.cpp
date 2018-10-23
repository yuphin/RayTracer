#include "Sphere.h"
#include <algorithm>
Sphere::Sphere(float x, float y, float z) : Object(x, y, z) {
	R = pos.length();
}

bool Sphere::hit(Ray r, float t) {
	float t0, t1;
	if (solve_quadratic_eq(r.direction.dot_product(r.direction), 2 * (r.origin.dot_product(r.direction)), r.origin.dot_product(r.origin) - R * R,t0,t1)) {
		t = std::min({ t0,t1 });
		return true;
	 }
	return false;
}
