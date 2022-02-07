#pragma once
#include "camera.h"
class PerspectiveCamera : public Camera {
private:
	Vec3f center;
	Vec3f up;
	Vec3f hori;
	Vec3f dir;
	float angle;
public:
	PerspectiveCamera() {}
	PerspectiveCamera(Vec3f &c, Vec3f &u, Vec3f &d, float a) {
		center = c;
		dir = d;
		up = u;
		dir.Normalize();
		Vec3f::Cross3(hori, dir, up);
		hori.Normalize();
		Vec3f::Cross3(up, hori, dir);
		up.Normalize();
		angle = a;
	}
	~PerspectiveCamera() {}

	Ray generateRay(Vec2f point) {
		float d = 0.5 / tan(angle / 2);
		Vec3f v = d * dir + (point.x() - 0.5) * hori + (point.y() - 0.5) * up;
		v.Normalize();
		return Ray(center, v);
	}
	float getTMin() const {
		return -1 * FLT_MAX;
	}
};