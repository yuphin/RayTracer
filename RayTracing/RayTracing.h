#pragma once
#include "parser.h"
#include "ppm.h"
#include <algorithm>
#include "Ray.h"
#include "Camera.h"
#include "Geometry.h"
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include <math.h>
#include "Light.h"
#include <thread>
#include <atomic>
#include <future>
//DATE
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
inline Vec3f parserVecToGeo(const parser::Vec3f& vec);
void loadAssets(const parser::Scene& scene, std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights);
bool traceWithRecord(const Ray& cameraRay, const std::vector<std::unique_ptr<Object>>& objects, Object*& hitObject, HitRecord& rec);
bool shadowTrace(const Ray& cameraRay, const std::vector<std::unique_ptr<Object>>& objects);
Vec3f getColor(const Ray& ray, parser::Scene& scene, const std::vector<std::unique_ptr<Object>>& objects, const std::vector<std::unique_ptr<Light>>& lights, int depth);
void render(parser::Scene& scene, const std::vector<std::unique_ptr<Object>>& objects, const  std::vector<std::unique_ptr<Light>>& lights, unsigned char* image, const parser::Camera& cam);
std::string return_current_time_and_date();