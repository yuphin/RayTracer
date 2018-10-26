#include "Triangle.h"
Triangle::Triangle(const Vec3f & a, const Vec3f & b, const Vec3f & c,const Material& material) : Object(material), a(a),b(b),c(c) {
	normal = ((b - a).cross_product(c - a)).normalize();
}
Triangle::~Triangle() {}

bool Triangle::hit(const Ray& r, float & t) {
	auto v1 = a-b;
	auto v2 = a-c;
	auto v3 = r.direction;
	auto v4 = a - r.origin;
	auto M = calculate_determinant(v1, v2, v3);
	t = calculate_determinant(v1, v2, v4) / M;
	if (t < 0.0f)
		return false;
	auto gamma = calculate_determinant(v1, v4, v3) / M;
	if (gamma < 0.0f || gamma > 1.0f)
		return false;
	auto beta = calculate_determinant(v4, v2, v3) / M;
	if (beta < 0.0f || beta > (1.0f - gamma))
		return false;
	return true;
}



Vec3f Triangle::getNormal(const Ray & r,float t) {
	return normal;
}

