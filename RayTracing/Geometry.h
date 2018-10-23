#pragma once
#include <math.h>
template<typename T = float>
struct Vec3{
	T x, y, z;
	Vec3() : x(T(0)),y(T(0)),z(T(0)) {  }
	Vec3(T val) : x(val), y(val), z(val) {};
	Vec3(T x,T y , T z) : x(x), y(y), z(z) {};
	inline T getX() { return x; }
	inline T getY() { return y; }
	inline T getZ() { return z; }
	inline T getR() { return x; }
	inline T getG() { return y; }
	inline T getB() { return z; }
	inline Vec3 operator+(const Vec3& vec) { return Vec3(x + vec.x, y + vec.y, z + vec.z); }
	inline Vec3 operator-(const Vec3& vec) { return Vec3(x - vec.x, y - vec.y, z - vec.z); }
	inline Vec3 operator-() { return Vec3(-x,-y,-z); }
	inline Vec3 operator*(const T& s) { return Vec3(s*x,s*y,s*z); }
	inline T dot_product(const Vec3& v) { return v.x*x + v.y*y + v.z*z; }
	inline T length() { return sqrtf(x*x + y * y + z * z); }
	Vec3 cross_product(const Vec3& v) { return Vec3(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x); }
	Vec3 normalize() {
		auto vecLength = 1/length();
		return Vec3(x*vecLength, y*vecLength, z*vecLength);
	}
};
using Vec3f = Vec3<>;
bool solve_quadratic_eq(const float&a, const float&b, const float&c, float&x0, float&x1) noexcept;