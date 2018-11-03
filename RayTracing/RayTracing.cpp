#include "RayTracing.h"
#define gV(X) parserVecToGeo(X)
inline Vec3f parserVecToGeo(const parser::Vec3f& vec) {
	return Vec3f(vec.x, vec.y, vec.z);
}
void loadAssets(const parser::Scene& scene, std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights) {
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
				R += E * (hitObject->material.specularReflectance.x)*(light->intensity.x)*pow(std::fmax(0.0f, rec.normal.dot_product(h)), hitObject->material.phongExponent);
				G += E * (hitObject->material.specularReflectance.y)*(light->intensity.y)*pow(std::fmax(0.0f, rec.normal.dot_product(h)), hitObject->material.phongExponent);
				B += E * (hitObject->material.specularReflectance.z)*(light->intensity.z)*pow(std::fmax(0.0f, rec.normal.dot_product(h)), hitObject->material.phongExponent);
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
std::string return_current_time_and_date() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}
void render(parser::Scene& scene, const std::vector<std::unique_ptr<Object>>& objects, const  std::vector<std::unique_ptr<Light>>& lights, unsigned char* image, const parser::Camera& cam) {
	Camera camera(Vec3f(cam.position.x, cam.position.y, cam.position.z), Vec3f(cam.gaze.x, cam.gaze.y, cam.gaze.z), Vec3f(cam.up.x, cam.up.y, cam.up.z), cam.near_distance);
	auto imgSize = cam.image_height* cam.image_width * 3;
	auto numCores = std::thread::hardware_concurrency();
	volatile std::atomic<std::size_t> atomicCount(0);
	std::vector<std::future<void>> futureVec;
	for (; numCores > 0; numCores--) {
		futureVec.emplace_back(std::async([&] {
			while (true) {
				auto pixIndex = (atomicCount += 3) - 3;
				if (pixIndex >= imgSize)
					break;
				int xHelper = (pixIndex / 3);
				int yHelper = xHelper / cam.image_width;
				float u = (cam.near_plane.x + (cam.near_plane.y - cam.near_plane.x) * ((((xHelper)) % cam.image_width) + 0.5f) / cam.image_width);
				float v = (cam.near_plane.z + (cam.near_plane.w - cam.near_plane.z) * (((cam.image_height - 1) - yHelper) + 0.5f) / cam.image_height);
				auto ray = camera.cast_ray(u, v);
				auto colorVec = getColor(ray, scene, objects, lights, 0);
				image[pixIndex] = colorVec.x > 255.0f ? 255 : int(colorVec.x);
				image[pixIndex + 1] = colorVec.y > 255.0f ? 255 : int(colorVec.y);
				image[pixIndex + 2] = colorVec.z > 255.0f ? 255 : int(colorVec.z);
			}
		}));
	}
	

}
