#pragma once
#include "Object3D.h"
#include "material.h"
class Plane : public Object3D {
private:
	float distance;
	Vec3f n;
	Material* material;
public:
	Plane() {}
	Plane(Vec3f& normal, float d, Material* m) :material(m), n(normal), distance(d) {};
	bool intersect(const Ray& r, Hit& h, float tmin) {
		Vec3f Rd = r.getDirection();
		Vec3f Ro = r.getOrigin();
		float t = (distance - Ro.Dot3(n)) / Rd.Dot3(n);
		if ((t > max(tmin, 0.0f)) && (t < h.getT()))
		{
			h.set(t, material, n, r);
			return true;
		}
		return false;
	}
};