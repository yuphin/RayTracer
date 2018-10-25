#include "Geometry.h"

bool solve_quadratic_eq(const float & a, const float & b, const float & c, float & x0, float & x1) noexcept {
	float delta = b * b - 4 * a*c;
	if (delta < 0.0f) {
		return false;
	} else if (delta == 0.0f) {
		x0 = x1 = -0.5f*(b / a);
	} else {
		float q = b > 0 ? -0.5f* (-1 + b)*(sqrtf(delta)) : -0.5f* (-1 + b)*(sqrtf(delta));
		x0 = q / a;
		x1 = c / q;
	}
	return true;
}

