#pragma once
#include <math.h>
#include <initializer_list>
template<typename T = float>
class Vec3{
public:
	T x, y, z;
	Vec3() : x(T(0)),y(T(0)),z(T(0)) {  }
	Vec3(T val) : x(val), y(val), z(val) {};
	Vec3(T x,T y , T z) : x(x), y(y), z(z) {};
	inline Vec3 operator+(const Vec3& vec)const  { return Vec3(x + vec.x, y + vec.y, z + vec.z);}
	inline Vec3 operator-(const Vec3& vec) const  { return Vec3(x - vec.x, y - vec.y, z - vec.z); }
	inline Vec3 operator-() { return Vec3(-x,-y,-z); }
	inline Vec3 operator*( T& s) const { return Vec3(s*x,s*y,s*z); }
	inline T dot_product(const Vec3& v) const { return v.x*x + v.y*y + v.z*z; }
	inline T length() { return sqrtf(x*x + y * y + z * z); }
	inline Vec3 cross_product(const Vec3& v) const { return Vec3(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x); }
	Vec3 normalize() {
		auto vecLength = 1/length();
		return Vec3(x*vecLength, y*vecLength, z*vecLength);
	}
};
using Vec3f = Vec3<>;
bool solve_quadratic_eq(const float&a, const float&b, const float&c, float&x0, float&x1) noexcept;
inline float calculate_determinant(const Vec3f& a, const Vec3f&b, const Vec3f&c) {
	return a.x * (b.y*c.z - c.y * b.z) + a.y*(c.x*b.z - b.x* c.z) + a.z* (b.x*c.y - c.x * b.y);
}