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
typedef unsigned char RGB[3];
bool trace(const Ray& cameraRay,const std::vector<std::unique_ptr<Object>>& objects, Object& hitObject)  {
	auto hit = false;
	float t, t1 = std::numeric_limits<float>::infinity();
	t = t1;

	for (auto& o : objects) {
		if (o->hit(cameraRay, t) && t < t1) {
			t1 = t;
			hitObject = *o;
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

	
	
	for (auto cam : scene.cameras) {
		unsigned char* image = new unsigned char[cam.image_width,cam.image_height];
		float aspectRatio = (float)cam.image_width / cam.image_height;
		
		Camera camera(Vec3f(cam.position.x, cam.position.y, cam.position.z), Vec3f(cam.gaze.x, cam.gaze.y, cam.gaze.z), Vec3f(cam.up.x, cam.up.y, cam.up.z),cam.near_distance);
		for (int j = 0; j < cam.image_width; ++j) {
			for (int i = 0; i < cam.image_height; ++i) {
				float u = (cam.near_plane.x + (cam.near_plane.y - cam.near_plane.x) * (i + 0.5f) / cam.image_width)*aspectRatio;
				float v = -1*(cam.near_plane.z + (cam.near_plane.w - cam.near_plane.z) * (i + 0.5f) / cam.image_height);
				 camera.cast_ray(u, v);
				
				
			}
		}

		//write_ppm(cam.image_name.c_str(), image, cam.image_width, cam.image_height);
	}
	

}
