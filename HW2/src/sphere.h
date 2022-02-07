#pragma once
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "Object3D.h"

class Sphere : public Object3D {
private:
	Vec3f center;
	double radius;
	//Material* material;
public:
	Sphere() : center(0, 0, 0), radius(0) {};
	Sphere(Vec3f c, double r, Material* m) : Object3D(m), center(c), radius(r) {};
	~Sphere() {};
	bool intersect(const Ray& r, Hit& h, float tmin) {
		Vec3f Rd = r.getDirection();
		Vec3f Ro = r.getOrigin() - center;
		double a = Rd.Length() * Rd.Length();
		double b = 2 * Rd.Dot3(Ro);
		double c = Ro.Dot3(Ro) - radius * radius;
		double d = sqrt(b * b - 4 * a * c);

		if (d < 0) return false;

		double t_pos = (-1 * b + d) / (2 * a);//()!
		double t_neg = (-1 * b - d) / (2 * a);

		if (t_pos < 0) return false;
		
		double t_now = h.getT();
		if (t_neg > 0) {
			if (t_neg < t_now && t_neg > tmin) {
				Vec3f normal = Ro + t_neg * Rd;
				normal.Normalize();
				h.set(t_neg, material, normal, r);
				return true;
			}
		}
		else {
			if (t_pos < t_now && t_pos > tmin) {
				Vec3f normal = Ro + t_pos * Rd;
				normal.Normalize();
				h.set(t_pos, material, normal, r);
				return true;
			}
		}
		return false;
	}
};