#include "Camera.h"

Camera::Camera(Vec3f origin, Vec3f gaze, Vec3f up, float d) {
	e = origin;
	w = -(gaze - origin).normalize(); // -w
	u = up.cross_product(w).normalize();
	v = w.cross_product(u);
	distance = d;
}


Ray Camera::cast_ray(float uP, float vP) {
	return Ray(e, (u*uP + v * vP - w * distance).normalize());
}
