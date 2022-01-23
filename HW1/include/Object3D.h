#pragma once
#include "ray.h"
#include "hit.h"
#include "material.h"
class Object3D {
public:
	Material* material;
	
	Object3D() : material(nullptr) {};
	Object3D(Material* m) : material(m) {};
	~Object3D() {
		if (material != nullptr) {
			delete material;
		}
	}
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
};

