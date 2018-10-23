#pragma once
#include "Object.h"
class Sphere :
	public Object
{
public:
	// Inherited via Object
	
	float R;
	Sphere(float x, float y, float z);
	virtual bool hit(Ray r, float t) override;

};

