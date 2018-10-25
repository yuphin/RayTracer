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
typedef unsigned char RGB[3];

inline Vec3f parserVecToGeo(const parser::Vec3f& vec) {
	return Vec3f(vec.x, vec.y, vec.z);
}
bool trace(const Ray& cameraRay,const std::vector<std::unique_ptr<Object>>& objects, Object* hitObject)  {
	auto hit = false;
	float t, t1 = std::numeric_limits<float>::infinity();
	t = t1;
	for (auto& o : objects) {
		if (o->hit(cameraRay, t) && (t < t1) && t > 0.001f) {
			t1 = t;
			hitObject = o.get();
			hit = true;
		}
	}
	return hit;
}
int main(int argc, char* argv[]) {
	// Sample usage for reading an XML scene file
	parser::Scene scene;

	scene.loadFromXml(argv[1]);

	// The code below creates a test pattern and writes
	// it to a PPM file to demonstrate the usage of the
	// ppm_write function.

	const RGB BAR_COLOR[8] =
	{
		{ 255, 255, 255 },  // 100% White
		{ 255, 255,   0 },  // Yellow
		{   0, 255, 255 },  // Cyan
		{   0, 255,   0 },  // Green
		{ 255,   0, 255 },  // Magenta
		{ 255,   0,   0 },  // Red
		{   0,   0, 255 },  // Blue
		{   0,   0,   0 },  // Black
	};


	auto objects = std::vector<std::unique_ptr<Object>>();
	auto flag = 1;
	for (auto& sphere : scene.spheres) {
		objects.emplace_back(std::make_unique<Sphere>(parserVecToGeo(scene.vertex_data[sphere.center_vertex_id -1]), sphere.radius));
	}
	for (auto& triangle : scene.triangles) {
		objects.emplace_back(std::make_unique<Triangle>(parserVecToGeo(scene.vertex_data[triangle.indices.v0_id-1]), parserVecToGeo(scene.vertex_data[triangle.indices.v1_id - 1]), parserVecToGeo(scene.vertex_data[triangle.indices.v2_id - 1])));
	}
	for (auto cam : scene.cameras) {
		unsigned char* image = new unsigned char[cam.image_width*cam.image_height*3];
		float aspectRatio = (float)cam.image_width / cam.image_height;
		
		Camera camera(Vec3f(cam.position.x, cam.position.y, cam.position.z), Vec3f(cam.gaze.x, cam.gaze.y, cam.gaze.z), Vec3f(cam.up.x, cam.up.y, cam.up.z),cam.near_distance);
		int m = 0;
		for (int j = 0; j < cam.image_height; ++j) {
			for (int i = 0; i < cam.image_width; ++i) {
				float u = (cam.near_plane.x + (cam.near_plane.y - cam.near_plane.x) * (i + 0.5f) / cam.image_width)*aspectRatio;
				float v = -(cam.near_plane.z + (cam.near_plane.w - cam.near_plane.z) * (j + 0.5f) / cam.image_height);
				Object* hitObject = nullptr;
				auto ray = camera.cast_ray(u, v);
				if (trace(ray,objects,hitObject)) {
					auto index = j * cam.image_height + i;
					image[m++] = BAR_COLOR[0][0];
					image[m++] = BAR_COLOR[0][0];
					image[m++] = BAR_COLOR[0][0];
				} else {
					image[m++] = 0;
					image[m++] =0;
					image[m++] =0;
				}
				
				
			}
		}

		write_ppm(cam.image_name.c_str(), image, cam.image_width, cam.image_height);
	}
	

}
