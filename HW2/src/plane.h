#pragma once
#include "Object3D.h"
#include "material.h"
class Plane : public Object3D {
private:
	float distance;
	Vec3f normal;
	Material* material;
public:
	Plane() {}
	Plane(Vec3f& normal, float d, Material* m) :Object3D(m), normal(normal), distance(d) {};
	bool intersect(const Ray& r, Hit& h, float tmin) {
		Vec3f Rd = r.getDirection();
		Vec3f Ro = r.getOrigin();
		float t = -1.0f * (distance + Ro.Dot3(normal)) / Rd.Dot3(normal);
		if (t > max(tmin, 0.0f) && t < h.getT())
		{
			h.set(t, material, normal, r);
			return true;
		}
		return false;
	}
};