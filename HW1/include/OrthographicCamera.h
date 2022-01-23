#pragma once
#include "camera.h"
#include "matrix.h"

class OrthographicCamera :public camera {
private:
	Vec3f center;
	Vec3f up;
	Vec3f hori;
	Vec3f dir;
	float size;
public:
	OrthographicCamera() {};
	OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s){
		center = c;
		dir = d;
		up = u;
		dir.Normalize();
		Vec3f::Cross3(hori, dir, up);
		up.Normalize();
		Vec3f::Cross3(up, hori, dir);
		size = s;

	}
	~OrthographicCamera() {};

	Ray generateRay(Vec2f point) {
		Vec3f dx = size * hori;
		Vec3f dy = size * up;
		Vec3f pointToImage = center - 0.5 * dx - 0.5 * dy + point.x() * dx + point.y() * dy;
	}
	float getTMin() const {
		return -1 * FLT_MAX;
	}
};