#include <iostream>
#include "RayTracing.h"
int main(int argc, char* argv[]) {
	parser::Scene scene;
	scene.loadFromXml(argv[1]);
	auto objects = std::vector<std::unique_ptr<Object>>();
	auto lights = std::vector<std::unique_ptr<Light>>();
	loadAssets(scene, objects, lights);
	for (auto& cam : scene.cameras) {
		unsigned char* image = new unsigned char[cam.image_width*cam.image_height * 3];
		//std::cout << return_current_time_and_date() << std::endl;
		render(scene, objects, lights, image, cam);
		//std::cout << return_current_time_and_date() << std::endl;
		write_ppm(cam.image_name.c_str(), image, cam.image_width, cam.image_height);
		
	}
}