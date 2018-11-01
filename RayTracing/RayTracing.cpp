// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "Ray.h"
#include "Camera.h"
#include "Geometry.h"
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include <algorithm>
#include <thread>
#define gV(X) parserVecToGeo(X)
typedef unsigned char RGB[3];
inline Vec3f parserVecToGeo(const parser::Vec3f& vec) {
	return Vec3f(vec.x, vec.y, vec.z);
}
inline void loadAssets(const parser::Scene& scene, std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights) {
	for (auto& sphere : scene.spheres) {
		auto &materials = scene.materials[sphere.material_id - 1];
		Material material(gV(materials.ambient), gV(materials.diffuse), gV(materials.specular), gV(materials.mirror), materials.phong_exponent);
		objects.emplace_back(std::make_unique<Sphere>(parserVecToGeo(scene.vertex_data[sphere.center_vertex_id - 1]), sphere.radius, material));
	}
	for (auto& triangle : scene.triangles) {
		auto &materials = scene.materials[triangle.material_id - 1];
		Material material(gV(materials.ambient), gV(materials.diffuse), gV(materials.specular), gV(materials.mirror), materials.phong_exponent);
		objects.emplace_back(std::make_unique<Triangle>(parserVecToGeo(scene.vertex_data[triangle.indices.v0_id - 1]), parserVecToGeo(scene.vertex_data[triangle.indices.v1_id - 1]), parserVecToGeo(scene.vertex_data[triangle.indices.v2_id - 1]), material));
	}
	for (auto &mesh : scene.meshes) {
		auto &materials = scene.materials[mesh.material_id - 1];
		Material material(gV(materials.ambient), gV(materials.diffuse), gV(materials.specular), gV(materials.mirror), materials.phong_exponent);
		for (auto&triangle : mesh.faces) {
			objects.emplace_back(std::make_unique<Triangle>(parserVecToGeo(scene.vertex_data[triangle.v0_id - 1]), parserVecToGeo(scene.vertex_data[triangle.v1_id - 1]), parserVecToGeo(scene.vertex_data[triangle.v2_id - 1]), material));

		}
	}
	for (auto& light : scene.point_lights) {
		lights.emplace_back(std::make_unique<Light>(gV(light.position), gV(light.intensity)));
	}
}
bool traceWithRecord(const Ray& cameraRay, const std::vector<std::unique_ptr<Object>>& objects, Object*& hitObject, HitRecord& rec) {
	auto hit = false;
	float t, t1 = std::numeric_limits<float>::infinity();
	t = t1;
	for (auto& o : objects) {
		if ((o->hit(cameraRay, t) && (t < t1)) && t > 0.0f) {
			t1 = t;
			hitObject = o.get();
			rec.t = t;
			rec.normal = hitObject->getNormal(cameraRay, t);
			hit = true;
		}
	}
	return hit;
}
bool shadowTrace(const Ray& cameraRay, const std::vector<std::unique_ptr<Object>>& objects) {
	auto hit = false;
	float t, t1 = std::numeric_limits<float>::infinity();
	t = t1;
	for (auto& o : objects) {
		if ((o->hit(cameraRay, t) && (t < t1)) && t < 1.0f) {
			hit = true;
			break;
		}
	}
	return hit;
}
inline Vec3f calculateEach(Vec3f vec, Vec3f reflect) {
	return Vec3f(vec.x*reflect.x, vec.y*reflect.y, vec.z*reflect.z);
}
Vec3f getColor(const Ray& ray, parser::Scene& scene, const std::vector<std::unique_ptr<Object>>& objects, const std::vector<std::unique_ptr<Light>>& lights, int depth) {
	if (depth > scene.max_recursion_depth)
		return Vec3f(scene.background_color.x, scene.background_color.y, scene.background_color.z);
	Object* hitObject = nullptr;
	HitRecord rec;
	if (traceWithRecord(ray, objects, hitObject, rec)) {
		// Ambient light
		float R = 0, G = 0, B = 0;
		R += (hitObject->material.ambientReflectance.x)*scene.ambient_light.x;
		G += (hitObject->material.ambientReflectance.y)*scene.ambient_light.y;
		B += (hitObject->material.ambientReflectance.z)*scene.ambient_light.z;
		auto P = (ray.origin + ray.direction*rec.t);
		for (auto & light : lights) {
			auto l = (light->pos - P);
			Ray shadow(P + l * scene.shadow_ray_epsilon, l);
			if (!shadowTrace(shadow, objects)) {
				l = l.normalize();
				auto v = (ray.origin - P).normalize();
				auto h = (v + l).normalize();
				auto d = ((light->pos - P).length());
				auto E = 1 / (d*d);
				auto test = rec.normal.dot_product(l);
				// Diffuse Shading
				R += E * (hitObject->material.diffuseReflectance.x)*(light->intensity.x)*std::fmax(0.0f, rec.normal.dot_product(l));
				G += E * (hitObject->material.diffuseReflectance.y)*(light->intensity.y)*std::fmax(0.0f, rec.normal.dot_product(l));
				B += E * (hitObject->material.diffuseReflectance.z)*(light->intensity.z)*std::fmax(0.0f, rec.normal.dot_product(l));
				// Blinn-Phong Shading
				R += E * (hitObject->material.specularReflectance.x)*(light->intensity.x)*std::powf(std::fmax(0.0f, rec.normal.dot_product(h)), hitObject->material.phongExponent);
				G += E * (hitObject->material.specularReflectance.y)*(light->intensity.y)*std::powf(std::fmax(0.0f, rec.normal.dot_product(h)), hitObject->material.phongExponent);
				B += E * (hitObject->material.specularReflectance.z)*(light->intensity.z)*std::powf(std::fmax(0.0f, rec.normal.dot_product(h)), hitObject->material.phongExponent);
			} else {
				continue;
			}
		}
		auto reflectionDirection = ray.direction - rec.normal*(2 * (ray.direction.dot_product(rec.normal)));
		Ray reflectionRay(P + reflectionDirection * scene.shadow_ray_epsilon, reflectionDirection);
		auto reflectColor = getColor(reflectionRay, scene, objects, lights, depth + 1);
		return Vec3f(R, G, B) + calculateEach(reflectColor, hitObject->material.mirrorReflectance);
	} else {
		return Vec3f(scene.background_color.x, scene.background_color.y, scene.background_color.z);
	}
}
void render(parser::Scene& scene, const std::vector<std::unique_ptr<Object>>& objects, const  std::vector<std::unique_ptr<Light>>& lights, unsigned char* image, const parser::Camera& cam, int hOffset, int tOffset) {
	Camera camera(Vec3f(cam.position.x, cam.position.y, cam.position.z), Vec3f(cam.gaze.x, cam.gaze.y, cam.gaze.z), Vec3f(cam.up.x, cam.up.y, cam.up.z), cam.near_distance);
	int k = 0;
	for (int j = cam.image_height - 1; j >= 0; --j) {
		float offset = int(((float)hOffset / tOffset)*cam.image_width);
		float  upperbound = int(((float)(hOffset + 1) / tOffset)*cam.image_width);
		for (int i = offset; i < upperbound; ++i) {
			long long imgIndex = k * cam.image_width * 3 + i * 3;
			float u = (cam.near_plane.x + (cam.near_plane.y - cam.near_plane.x) * (i + 0.5f) / cam.image_width);
			float v = (cam.near_plane.z + (cam.near_plane.w - cam.near_plane.z) * (j + 0.5f) / cam.image_height);
			auto ray = camera.cast_ray(u, v);
			auto colorVec = getColor(ray, scene, objects, lights, 0);
			image[imgIndex] = colorVec.x > 255.0f ? 255 : int(colorVec.x);
			image[imgIndex + 1] = colorVec.y > 255.0f ? 255 : int(colorVec.y);
			image[imgIndex + 2] = colorVec.z > 255.0f ? 255 : int(colorVec.z);

		}
		k++;
	}
}
int main(int argc, char* argv[]) {
	parser::Scene scene;
	scene.loadFromXml(argv[1]);
	auto objects = std::vector<std::unique_ptr<Object>>();
	auto lights = std::vector<std::unique_ptr<Light>>();
	loadAssets(scene, objects, lights);
	for (auto& cam : scene.cameras) {
		unsigned char* image = new unsigned char[cam.image_width*cam.image_height * 3];
		std::vector<std::thread> threadVec;
		for (int i = 0; i < 8; i++) {
			threadVec.emplace_back(std::thread(render, std::ref(scene), std::ref(objects), std::ref(lights), image, std::ref(cam), i, 8));
		}
		std::for_each(threadVec.begin(), threadVec.end(), [](std::thread&t) {t.join(); });
		write_ppm(cam.image_name.c_str(), image, cam.image_width, cam.image_height);
	}


}
