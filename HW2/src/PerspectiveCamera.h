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
	PerspectiveCamera(Vec3f &c, Vec3f &d, Vec3f &u, float a) {
		center = c;
		dir = d;
		dir.Normalize();
		Vec3f::Cross3(hori, dir, u);
		hori.Normalize();
		Vec3f::Cross3(up, hori, dir);
		angle = a;
	}
	~PerspectiveCamera() {}

	Ray generateRay(Vec2f point) {
		const float dist = 1.0;
		Vec3f dy = 2 * tan(angle / 2.0) * dist * up;
		Vec3f dx = 2 * tan(angle / 2.0) * dist * ratio * hori;
		Vec3f pt = center + dist * dir - 0.5 * dx + point.x() * dx - 0.5 * dy + point.y() * dy;
		Vec3f dir = pt - center;
		dir.Normalize();
		return Ray(center, dir);
	}
	float getTMin() const {
		return -1 * FLT_MAX;
	}
};